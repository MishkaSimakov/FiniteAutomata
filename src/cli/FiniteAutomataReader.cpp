#include "FiniteAutomataReader.h"

#include <sstream>

FiniteAutomata FiniteAutomataReader::read(std::istream& is) {
  FiniteAutomata result;
  result.nodes.clear();

  char cmd;
  std::string line;
  size_t current_index = 0;
  while (true) {
    std::getline(is, line);
    std::istringstream line_is(line);

    line_is >> cmd;

    if (cmd == 'e') {
      break;
    }

    auto& current_node = result.nodes.emplace_back();

    if (cmd == 'f') {
      current_node.is_final = true;
    }

    for (size_t i = 0; i < Charset::symbols_count; ++i) {
      size_t index;
      line_is >> index;

      current_node.jumps[i] = index;
    }

    ++current_index;
  }

  return result;
}

FiniteAutomata FiniteAutomataReader::read(std::string_view string) {
  std::stringstream ss;
  ss << string;

  return read(ss);
}
