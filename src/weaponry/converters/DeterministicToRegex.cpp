#include "DeterministicFiniteAutomata.h"

struct RegexAutomata {
  struct RegexAutomataNode {
    bool is_final{false};

    std::unordered_map<RegexAutomataNode*, Regex> jumps;
    bool reachable{false};
  };

  std::list<RegexAutomataNode> nodes;

  static void emplace_jump(
      std::unordered_map<RegexAutomataNode*, Regex>& from_jumps,
      RegexAutomataNode* to, Regex regex) {
    auto [itr, was_inserted] = from_jumps.emplace(to, std::move(regex));

    if (!was_inserted) {
      // then we should merge old value with new
      itr->second += std::move(regex);
    }
  }

  static Regex move_jump(RegexAutomataNode& from, RegexAutomataNode& to) {
    auto itr = from.jumps.find(&to);

    if (itr == from.jumps.end()) {
      return Regex("0");
    }

    return std::move(itr->second);
  }

  static void emplace_jump(RegexAutomataNode* from, RegexAutomataNode* to,
                           Regex regex) {
    emplace_jump(from->jumps, to, std::move(regex));
  }

  void remove_unreachable() {
    std::queue<RegexAutomataNode*> queue;
    for (auto& node : nodes) {
      if (node.is_final) {
        queue.push(&node);
        node.reachable = true;
      }
    }

    while (!queue.empty()) {
      RegexAutomataNode* current = queue.front();
      queue.pop();

      for (auto& from : nodes) {
        if (!from.reachable && from.jumps.contains(current)) {
          from.reachable = true;
          queue.push(&from);
        }
      }
    }

    nodes.front().reachable = true;

    for (auto& node : nodes) {
      for (auto itr = node.jumps.begin(); itr != node.jumps.end();) {
        if (!itr->first->reachable) {
          itr = node.jumps.erase(itr);
        } else {
          ++itr;
        }
      }
    }

    std::erase_if(
        nodes, [](const RegexAutomataNode& node) { return !node.reachable; });
  }

  explicit RegexAutomata(const DeterministicFiniteAutomata& automata) {
    const auto& other_nodes = automata.nodes;

    std::vector<RegexAutomataNode*> nodes_mapping(automata.nodes.size());
    auto symbols_view = Charset::get_symbols();

    size_t final_count = 0;

    for (size_t i = 0; i < other_nodes.size(); ++i) {
      RegexAutomataNode& regex_node = nodes.emplace_back();
      regex_node.is_final = other_nodes[i].is_final;
      nodes_mapping[i] = &regex_node;

      if (regex_node.is_final) {
        ++final_count;
      }
    }

    for (size_t i = 0; i < other_nodes.size(); ++i) {
      for (size_t j = 0; j < Charset::symbols_count; ++j) {
        RegexAutomataNode* dest = nodes_mapping[other_nodes[i].jumps[j]];
        emplace_jump(nodes_mapping[i], dest, Regex(symbols_view[j]));
      }
    }

    remove_unreachable();

    if (final_count > 1) {
      RegexAutomataNode& final_node = nodes.emplace_back();
      final_node.is_final = true;

      for (auto itr = nodes.begin(); itr != std::prev(nodes.end()); ++itr) {
        if (itr->is_final) {
          itr->is_final = false;
          itr->jumps.emplace(&final_node, Regex("1"));
        }
      }
    }
  }

  auto erase_paths_for_node(RegexAutomataNode& node) {
    std::vector<std::tuple<RegexAutomataNode*, RegexAutomataNode*, Regex>>
        new_edges;

    auto itr = node.jumps.find(&node);
    Regex cycle = itr != node.jumps.end() ? itr->second : Regex("0");
    cycle.iterate();

    for (auto& [end_ptr, end_regex] : node.jumps) {
      if (end_ptr == &node) {
        continue;
      }

      for (auto& start : nodes) {
        if (&start == &node) {
          continue;
        }

        auto start_itr = start.jumps.find(&node);
        if (start_itr == start.jumps.end()) {
          continue;
        }

        auto& [_, start_regex] = *start_itr;

        new_edges.emplace_back(&start, end_ptr,
                               start_regex * cycle * end_regex);
      }
    }

    return new_edges;
  }

  void erase_paths() {
    for (auto itr = std::next(nodes.begin()); itr != nodes.end();) {
      if (itr->is_final) {
        ++itr;
        continue;
      }

      auto new_edges = erase_paths_for_node(*itr);
      for (auto& [from, to, regex] : new_edges) {
        from->jumps.erase(&*itr);
        emplace_jump(from, to, regex);
      }

      itr = nodes.erase(itr);
    }
  }
};

DeterministicFiniteAutomata& DeterministicFiniteAutomata::complement() {
  for (auto& node: nodes) {
    node.is_final = !node.is_final;
  }

  return *this;
}

Regex DeterministicFiniteAutomata::get_regex() const {
  auto automata = RegexAutomata(*this);
  automata.erase_paths();

  auto& front = automata.nodes.front();

  if (automata.nodes.size() == 1) {
    auto itr = front.jumps.find(&front);

    if (itr != front.jumps.end()) {
      return itr->second;
    }

    return front.is_final ? Regex("1") : Regex("0");
  }

  auto& final = *std::next(automata.nodes.begin());

  Regex front_cycle = RegexAutomata::move_jump(front, front);
  Regex final_cycle = RegexAutomata::move_jump(final, final);

  front_cycle.iterate();

  Regex front_to_final = RegexAutomata::move_jump(front, final);
  Regex final_to_front = RegexAutomata::move_jump(final, front);

  Regex inner_cycle =
      final_cycle + final_to_front * front_cycle * front_to_final;
  inner_cycle.iterate();

  return front_cycle * front_to_final * inner_cycle;
}
