#pragma once
#include <array>

#include "CharsetInfo.h"
#include "FiniteAutomata.h"

namespace Converters {
class NonDeterministicToDeterministic;
}

using Charset = BinaryCharsetInfo;

class DeterministicFiniteAutomata {
 public:
  struct Node {
    std::unordered_set<size_t> old_indices;
    bool is_final;
    std::array<size_t, Charset::symbols_count> jumps;
  };

  std::deque<Node> nodes;

  DeterministicFiniteAutomata();

  explicit DeterministicFiniteAutomata(const FiniteAutomata& finite_automata);

  bool contains(std::string_view word);

  friend class Converters::NonDeterministicToDeterministic;
};
