#include <iostream>

#include "FiniteAutomata.h"
#include "regex/RegexParser.h"
#include "regex/RegexPrinter.h"

int main() {
  auto regex = RegexParser::parse("a(a(ab)*a(ab)* | b)*");
  std::cout << RegexPrinter(*regex) << std::endl;

  FiniteAutomata automata(*regex);
  automata.remove_empty_jumps();

  return 0;
}
