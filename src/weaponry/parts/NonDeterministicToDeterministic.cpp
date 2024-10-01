#include <map>
#include <queue>
#include <set>

#include "FiniteAutomata.h"

namespace {
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
}  // namespace

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
