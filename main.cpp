#include <fstream>

#include "DeterministicFiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
#include "RegexPrinter.h"

int main() {
  auto automata = DeterministicFiniteAutomata(Regex("(1 + (b + a(a + b))((a + b))*)"));

  std::ofstream os(
        "/Users/mihailsimakov/Documents/Programs/CLionProjects/FiniteAutomata/"
        "files/drawing.dot");
  os << FiniteAutomataPrinter(automata);
  os.close();

  automata.complement();

  auto regex = automata.get_regex();

  std::cout << RegexPrinter(regex) << std::endl;
  std::cout << RegexPrinter(regex.optimize()) << std::endl;

  return 0;
}
