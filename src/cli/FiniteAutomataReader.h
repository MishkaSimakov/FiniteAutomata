#pragma once
#include "FiniteAutomata.h"

#include <iostream>
#include <string_view>

class FiniteAutomataReader {
 public:
  FiniteAutomata read(std::istream& is);
  FiniteAutomata read(std::string_view string);
};
