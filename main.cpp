#include <fstream>
#include <iostream>

#include "DeterministicFiniteAutomata.h"
#include "FiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
#include "regex/RegexParser.h"
#include "regex/RegexPrinter.h"

int main() {
  auto regex = RegexParser::parse("aaa | bbb");
  std::cout << RegexPrinter(*regex) << std::endl;

  FiniteAutomata automata(*regex);
  automata.remove_empty_jumps();

  auto det = DeterministicFiniteAutomata(automata);

  std::cout << automata.containts_word("ababab") << std::endl;
  std::cout << det.contains("abababa") << std::endl;

  std::ofstream os(
      "/Users/mihailsimakov/Documents/Programs/CLionProjects/FiniteAutomata/"
      "files/drawing.dot");
  os << FiniteAutomataPrinter(det);

  return 0;
}
