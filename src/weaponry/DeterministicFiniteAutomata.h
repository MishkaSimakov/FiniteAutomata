#pragma once
#include <array>

#include "FiniteAutomata.h"

class DeterministicFiniteAutomata {
  struct Node {
    bool is_final;
    std::array<size_t, cLettersCount> jumps;
  };

  std::vector<Node> nodes_;

 public:
  explicit DeterministicFiniteAutomata(const FiniteAutomata& finite_automata);
};
