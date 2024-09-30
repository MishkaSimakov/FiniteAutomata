#include <fstream>

#include "FiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
#include "RegexPrinter.h"

int main() {
  auto first = FiniteAutomata(Regex("b + (1 + a + (a)*)(1 + 1)"));
  auto second = FiniteAutomata(Regex("(a)* + b"));

  std::cout << first.is_language_equal(second) << std::endl;

  // auto regex = automata.get_regex();
  //
  // std::cout << RegexPrinter(regex) << std::endl;
  // std::cout << RegexPrinter(regex.optimize()) << std::endl;

  return 0;
}
