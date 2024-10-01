#include <map>
#include <queue>
#include <set>

#include "FiniteAutomata.h"

#define USE_NEW_ND_TO_D_CONVERTER

#ifdef USE_NEW_ND_TO_D_CONVERTER
template <typename T, typename Compare = std::less<T>,
          typename Allocator = std::allocator<T>>
struct SetsComparator {
  [[no_unique_address]] Compare compare;

  bool operator()(const std::set<T, Compare, Allocator>& left,
                  const std::set<T, Compare, Allocator>& right) const {
    auto left_itr = left.begin();
    auto right_itr = right.begin();

    auto left_end = left.end();
    auto right_end = right.end();

    while (left_itr != left_end && right_itr != right_end) {
      if (compare(*left_itr, *right_itr)) {
        return true;
      }

      if (compare(*right_itr, *left_itr)) {
        return false;
      }

      // *left_itr == *right_itr
      ++left_itr;
      ++right_itr;
    }

    return left_itr == left_end && right_itr != right_end;
  }
};

FiniteAutomata::FiniteAutomata(const NonDeterministicFiniteAutomata& automata) {
  using NDNode = NonDeterministicFiniteAutomata::Node;
  using DNode = FiniteAutomata::Node;
  using NodesContainer =
      std::map<std::set<const NDNode*>, size_t, SetsComparator<const NDNode*>>;

  std::set<const NDNode*> start_nodes;
  start_nodes.insert(&automata.get_nodes().front());
  bool is_start_final =
      NonDeterministicFiniteAutomata::do_empty_jumps(start_nodes);

  // add new start node
  DNode& start = nodes.emplace_back();
  start.is_final = is_start_final;

  NodesContainer old_to_new;
  old_to_new.emplace(start_nodes, 0);

  std::queue<
      std::pair<size_t, std::reference_wrapper<const std::set<const NDNode*>>>>
      queue;
  queue.emplace(0, start_nodes);

  while (!queue.empty()) {
    auto [index, current_nodes] = queue.front();
    queue.pop();

    for (auto symbol : Charset::get_symbols()) {
      std::set<const NDNode*> nodes_after_jump;
      for (const NDNode* node : current_nodes.get()) {
        auto [beg, end] = node->jumps.equal_range(symbol);
        for (; beg != end; ++beg) {
          nodes_after_jump.insert(beg->second);
        }
      }

      bool is_final =
          NonDeterministicFiniteAutomata::do_empty_jumps(nodes_after_jump);

      size_t new_node_index = nodes.size();
      auto [itr, was_emplaced] =
          old_to_new.emplace(std::move(nodes_after_jump), new_node_index);

      if (was_emplaced) {
        nodes[index].jumps[Charset::get_index(symbol)] = new_node_index;

        DNode& new_node = nodes.emplace_back();
        new_node.is_final = is_final;
        queue.emplace(new_node_index, itr->first);
      } else {
        nodes[index].jumps[Charset::get_index(symbol)] = itr->second;
      }
    }
  }
}
#else
FiniteAutomata::FiniteAutomata(const NonDeterministicFiniteAutomata& automata) {
  using NDNode = typename NonDeterministicFiniteAutomata::Node;
  using NDNodeCref = std::reference_wrapper<const NDNode>;
  using DNode = FiniteAutomata::Node;

  const auto& old_nodes_list = automata.get_nodes();
  std::vector<NDNodeCref> old_nodes(old_nodes_list.begin(),
                                    old_nodes_list.end());

  std::unordered_set<const NDNode*> start_nodes;
  start_nodes.insert(&automata.get_nodes().front());
  NonDeterministicFiniteAutomata::do_empty_jumps(start_nodes);

  // add new start node
  DNode& start = nodes.emplace_back();

  std::vector start_state(automata.size(), false);
  for (size_t i = 0; i < start_state.size(); ++i) {
    const NDNode* current_node = &old_nodes[i].get();
    if (start_nodes.contains(current_node)) {
      start_state[i] = true;

      if (current_node->is_final) {
        start.is_final = true;
      }
    }
  }

  std::unordered_map<std::vector<bool>, size_t> old_to_new;
  old_to_new.emplace(start_state, 0);

  std::queue<std::vector<bool>> queue;
  queue.emplace(start_state);

  while (!queue.empty()) {
    std::vector<bool> current_nodes = queue.front();
    queue.pop();

    DNode& current_new_node = nodes[old_to_new.at(current_nodes)];

    std::unordered_set<const NDNode*> current_nodes_pointers;
    for (size_t i = 0; i < current_nodes.size(); ++i) {
      if (current_nodes[i]) {
        current_nodes_pointers.insert(&old_nodes[i].get());
      }
    }

    for (auto symbol : Charset::get_symbols()) {
      auto copy = current_nodes_pointers;
      NonDeterministicFiniteAutomata::do_jumps(copy, symbol);

      bool is_final = false;
      std::vector mask_after_jumps(automata.size(), false);
      for (size_t i = 0; i < automata.size(); ++i) {
        if (copy.contains(&old_nodes[i].get())) {
          mask_after_jumps[i] = true;

          if (old_nodes[i].get().is_final) {
            is_final = true;
          }
        }
      }

      auto [itr, was_emplaced] =
          old_to_new.emplace(mask_after_jumps, nodes.size());

      if (was_emplaced) {
        current_new_node.jumps[Charset::get_index(symbol)] = nodes.size();
        DNode& new_node = nodes.emplace_back();
        new_node.is_final = is_final;

        queue.push(mask_after_jumps);
        continue;
      }

      size_t jump_index = itr->second;
      current_new_node.jumps[Charset::get_index(symbol)] = jump_index;
    }
  }
}
#endif
