#pragma once
#include <fmt/ranges.h>

#include <iostream>

#include "DeterministicFiniteAutomata.h"

class FiniteAutomataPrinter {
  const DeterministicFiniteAutomata& automata_;

 public:
  explicit FiniteAutomataPrinter(const DeterministicFiniteAutomata& automata)
      : automata_(automata) {}

  void print(std::ostream& os) const {
    auto println = [&os](std::string_view string) { os << string << "\n"; };

    println("digraph finite_state_machine {");
    println("fontname=\"Helvetica,Arial,sans-serif\"");
    println("node [fontname=\"Helvetica,Arial,sans-serif\"]");
    println("edge [fontname=\"Helvetica,Arial,sans-serif\"]");
    println("rankdir=LR;");

    std::vector<size_t> final_nodes_indices;
    for (size_t i = 0; i < automata_.nodes.size(); ++i) {
      if (automata_.nodes[i].is_final) {
        final_nodes_indices.push_back(i);
      }
    }

    if (!final_nodes_indices.empty()) {
      println(fmt::format("node [shape = doublecircle]; {};",
                          fmt::join(final_nodes_indices, ", ")));
    }

    println("node [shape = circle];");

    for (size_t i = 0; i < automata_.nodes.size(); ++i) {
      const auto& node = automata_.nodes[i];

      for (size_t j = 0; j < node.jumps.size(); ++j) {
        println(fmt::format("{} -> {} [label = \"{}\"];", i, node.jumps[j],
                            Charset::get_symbols()[j]));
      }
    }

    println("}");
  }
};

std::ostream& operator<<(std::ostream& os,
                         const FiniteAutomataPrinter& printer) {
  printer.print(os);
  return os;
}
