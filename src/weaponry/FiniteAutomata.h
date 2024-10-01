#pragma once
#include <array>
#include <unordered_set>

#include "CustomRegex.h"
#include "NonDeterministicFiniteAutomata.h"

class FiniteAutomata {
 public:
  struct Node {
    bool is_final{false};
    std::array<size_t, Charset::symbols_count> jumps{};
  };

  std::deque<Node> nodes;

  FiniteAutomata();

  explicit FiniteAutomata(const Regex& regex)
      : FiniteAutomata(NonDeterministicFiniteAutomata(regex)) {}
  explicit FiniteAutomata(
      const NonDeterministicFiniteAutomata& finite_automata);

  void optimize();

  bool is_isomorphic_to(const FiniteAutomata& other) const;
  bool is_language_equal(const FiniteAutomata& other) const;

  FiniteAutomata get_minimal() const;

  bool contains(std::string_view word) const;

  FiniteAutomata& complement();

  Regex get_regex() const;
};

FiniteAutomata operator&(const FiniteAutomata& left,
                         const FiniteAutomata& right);
