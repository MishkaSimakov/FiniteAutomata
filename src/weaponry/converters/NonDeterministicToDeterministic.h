#pragma once
#include "DeterministicFiniteAutomata.h"

namespace Converters {
class NonDeterministicToDeterministic {
  using NDNode = FiniteAutomata::Node;
  using NDNodeCref = std::reference_wrapper<const NDNode>;
  using PairT = std::pair<const std::vector<bool>, NDNode*>;

  using DNode = DeterministicFiniteAutomata::Node;
public:
  DeterministicFiniteAutomata convert(const FiniteAutomata& automata);
};
} // Converters
