#pragma once
#include "DeterministicFiniteAutomata.h"
#include <iostream>

class FiniteAutomataPrinter {
  const DeterministicFiniteAutomata& automata_;
 public:
  explicit FiniteAutomataPrinter(const DeterministicFiniteAutomata& automata);

  void print(std::ostream& os) const;
};

inline std::ostream& operator<<(std::ostream& os,
                                const FiniteAutomataPrinter& printer) {
  printer.print(os);
  return os;
}
