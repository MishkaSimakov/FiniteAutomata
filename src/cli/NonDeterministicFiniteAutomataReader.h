#pragma once
#include <iostream>

#include "NonDeterministicFiniteAutomata.h"

class NonDeterministicFiniteAutomataReader {
 public:
  NonDeterministicFiniteAutomata read(std::istream& is);
  NonDeterministicFiniteAutomata read(std::string_view string);
};
