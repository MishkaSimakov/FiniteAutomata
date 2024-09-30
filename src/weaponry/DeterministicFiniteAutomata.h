#pragma once
#include <array>
#include <queue>

#include "CustomRegex.h"
#include "FiniteAutomata.h"

class DeterministicFiniteAutomata {
 public:
  struct Node {
    std::unordered_set<size_t> old_indices;
    bool is_final{false};
    std::array<size_t, Charset::symbols_count> jumps;
  };

  std::deque<Node> nodes;

  DeterministicFiniteAutomata();

  explicit DeterministicFiniteAutomata(const Regex& regex)
      : DeterministicFiniteAutomata(FiniteAutomata(regex)) {}
  explicit DeterministicFiniteAutomata(const FiniteAutomata& finite_automata);

  void optimize();

  DeterministicFiniteAutomata get_minimal() const;

  bool contains(std::string_view word) {
    size_t state = 0;
    for (char symbol : word) {
      state = nodes[state].jumps[Charset::get_index(symbol)];
    }

    return nodes[state].is_final;
  }

  DeterministicFiniteAutomata& complement();

  Regex get_regex() const;
};

inline auto operator&(const DeterministicFiniteAutomata& left,
                      const DeterministicFiniteAutomata& right) {
  DeterministicFiniteAutomata result;

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
