#pragma once
#include <array>
#include <queue>

#include "FiniteAutomata.h"
#include "CustomRegex.h"

template <typename Charset = CharsetInfo>
class DeterministicFiniteAutomata {
 public:
  struct Node {
    std::unordered_set<size_t> old_indices;
    bool is_final{false};
    std::array<size_t, Charset::symbols_count> jumps;
  };

  std::deque<Node> nodes;

  DeterministicFiniteAutomata() {
    nodes.emplace_back();

    for (size_t& jump : nodes.front().jumps) {
      jump = 0;
    }
  }

  explicit DeterministicFiniteAutomata(const Regex& regex)
      : DeterministicFiniteAutomata(FiniteAutomata<Charset>(regex)) {}
  explicit DeterministicFiniteAutomata(
      const FiniteAutomata<Charset>& finite_automata);

  void optimize() {
    // find and remove unreachable nodes
    std::vector<bool> visited(nodes.size(), false);
    visited[0] = true;

    std::queue<size_t> queue;
    queue.push(0);

    while (!queue.empty()) {
      size_t current = queue.front();
      queue.pop();

      for (size_t next : nodes[current].jumps) {
        if (!visited[next]) {
          visited[next] = true;
          queue.push(next);
        }
      }
    }

    std::vector<size_t> new_indices(nodes.size());
    size_t index = 0;
    for (size_t i = 0; i < nodes.size(); ++i) {
      if (visited[i]) {
        new_indices[i] = index;
        nodes[index] = std::move(nodes[i]);
        ++index;
      }
    }

    nodes.resize(index);

    for (Node& node : nodes) {
      for (size_t& jump : node.jumps) {
        jump = new_indices[jump];
      }
    }
  }

  bool contains(std::string_view word) {
    size_t state = 0;
    for (char symbol : word) {
      state = nodes[state].jumps[Charset::get_index(symbol)];
    }

    return nodes[state].is_final;
  }
};

template <typename Charset>
auto operator&(const DeterministicFiniteAutomata<Charset>& left,
               const DeterministicFiniteAutomata<Charset>& right) {
  DeterministicFiniteAutomata<Charset> result;

  size_t left_size = left.nodes.size();
  size_t right_size = right.nodes.size();

  result.nodes.resize(left_size * right_size);

  for (size_t i = 0; i < left_size; ++i) {
    for (size_t j = 0; j < right_size; ++j) {
      size_t index = i * right_size + j;

      const auto& left_node = left.nodes[i];
      const auto& right_node = right.nodes[j];

      result.nodes[index].is_final = left_node.is_final && right_node.is_final;

      for (size_t k = 0; k < Charset::symbols_count; ++k) {
        result.nodes[index].jumps[k] =
            left_node.jumps[k] * right_size + right_node.jumps[k];
      }
    }
  }

  result.optimize();

  return result;
}

#include "converters/NonDeterministicToDeterministic.h"
