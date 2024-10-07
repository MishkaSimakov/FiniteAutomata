#include <fstream>

#include "FiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
#include "RegexPrinter.h"

int main() {
  auto automata = FiniteAutomata(Regex("(a + (b)*)*a"));

  std::ofstream os(
      "/Users/mihailsimakov/Documents/Programs/CLionProjects/FiniteAutomata/"
      "files/drawing.dot");
  os << FiniteAutomataPrinter(automata);
  os.close();

  return 0;
}
