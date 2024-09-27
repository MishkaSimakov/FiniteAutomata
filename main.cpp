#include <fstream>

#include "DeterministicFiniteAutomata.h"
#include "FiniteAutomataPrinter.h"

int main() {
  auto nd_auto = FiniteAutomata(Regex("1"));
  auto d_auto = DeterministicFiniteAutomata(nd_auto);

  std::ofstream os(
      "/Users/mihailsimakov/Documents/Programs/CLionProjects/FiniteAutomata/"
      "files/drawing.dot");
  // os << FiniteAutomataPrinter(first);

  return 0;
}
