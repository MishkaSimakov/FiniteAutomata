#include "FiniteAutomata.h"

#include <algorithm>
#include <queue>

#include "CharsetInfo.h"

struct FiniteAutomata::FiniteAutomataBuilderVisitor final : RegexConstNodeVisitor {
  FiniteAutomata result;

  void visit(const SymbolNode& node) override {
    auto& start_node = result.get_start_node();
    auto& end_node = result.add_node();

    start_node.jumps.emplace(node.symbol, &end_node);
    end_node.is_final = true;
  }

  void visit(const ZeroNode& node) override {}
  void visit(const OneNode& node) override {
    auto& start_node = result.get_start_node();
    auto& end_node = result.add_node();

    start_node.jumps.emplace(cEmptyChar, &end_node);
    end_node.is_final = true;
  }

  void visit(const ConcatenationNode& node) override {
    FiniteAutomata concatenated;

    node.left->accept(*this);

    std::swap(concatenated, result);

    node.right->accept(*this);

    for (Node& final_node : concatenated.get_final_nodes()) {
      final_node.is_final = false;
      final_node.jumps.emplace(cEmptyChar, &result.get_start_node());
    }

    result.nodes_.splice(result.nodes_.begin(), concatenated.nodes_);
  }

  void visit(const OrNode& node) override {
    FiniteAutomata concatenated;

    node.left->accept(*this);

    std::swap(concatenated, result);

    node.right->accept(*this);

    Node& left_front = concatenated.get_start_node();
    Node& right_front = result.get_start_node();

    Node& front = result.nodes_.emplace_front();
    front.jumps.emplace(cEmptyChar, &left_front);
    front.jumps.emplace(cEmptyChar, &right_front);

    result.nodes_.splice(result.nodes_.end(), concatenated.nodes_);
  }

  void visit(const StarNode& node) override {
    node.child->accept(*this);

    auto& old_front = result.get_start_node();
    auto& front = result.nodes_.emplace_front();

    front.jumps.emplace(cEmptyChar, &old_front);

    for (auto& final_node : result.get_final_nodes()) {
      final_node.is_final = false;
      final_node.jumps.emplace(cEmptyChar, &front);
    }

    front.is_final = true;
  }
};

void FiniteAutomata::remove_unreachable_nodes() {
  std::unordered_set<const Node*> visited;
  std::queue<const Node*> queue;

  visited.insert(&nodes_.front());
  queue.push(&nodes_.front());

  while (!queue.empty()) {
    const Node* current = queue.front();
    queue.pop();

    for (const Node* next : current->jumps | std::views::values) {
      auto [itr, was_inserted] = visited.insert(next);

      if (was_inserted) {
        queue.push(next);
      }
    }
  }

  for (auto itr = nodes_.begin(); itr != nodes_.end();) {
    if (visited.contains(&*itr)) {
      ++itr;
    } else {
      itr = nodes_.erase(itr);
    }
  }
}

FiniteAutomata::FiniteAutomata(const Regex& regex) {
  FiniteAutomataBuilderVisitor visitor;
  regex.get_root().accept(visitor);

  *this = std::move(visitor.result);
}

bool FiniteAutomata::containts_word(std::string_view word) const {
  std::unordered_set<const Node*> current;
  current.insert(&nodes_.front());

  while (!word.empty()) {
    char current_letter = word.front();

    // jump by edges with empty letter
    do_empty_jumps(current);

    // do non-empty jumps
    std::unordered_set<const Node*> new_current;
    for (const Node* node : current) {
      auto [begin, end] = node->jumps.equal_range(current_letter);

      for (; begin != end; ++begin) {
        new_current.insert(begin->second);
      }
    }

    current = std::move(new_current);
    word.remove_prefix(1);
  }

  do_empty_jumps(current);

  return std::ranges::any_of(current,
                             [](const Node* node) { return node->is_final; });
}

void FiniteAutomata::remove_empty_jumps() {
  for (auto& node : nodes_) {
    std::unordered_set<Node*> nodes;
    nodes.insert(&node);

    do_empty_jumps(nodes);

    // add appropriate jumps
    std::vector<std::pair<char, Node*>> additional_jumps;
    for (const Node* reachable_by_empty_jump : nodes) {
      if (reachable_by_empty_jump->is_final) {
        node.is_final = true;
      }

      for (const auto& [symbol, dest] : reachable_by_empty_jump->jumps) {
        if (symbol != cEmptyChar) {
          additional_jumps.emplace_back(symbol, dest);
        }
      }
    }

    for (auto& [symbol, dest] : additional_jumps) {
      node.jumps.emplace(symbol, dest);
    }

    // remove all empty jumps from node
    node.jumps.erase(cEmptyChar);
  }

  remove_unreachable_nodes();
}
