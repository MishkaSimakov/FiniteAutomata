#include "NonDeterministicFiniteAutomataReader.h"

#include <sstream>

NonDeterministicFiniteAutomata NonDeterministicFiniteAutomataReader::read(
    std::istream& is) {
  NonDeterministicFiniteAutomata result;
  std::unordered_map<size_t, NonDeterministicFiniteAutomata::Node*>
      nodes_indices;

  nodes_indices.emplace(0, &result.get_start_node());

  auto get_node = [&nodes_indices, &result](size_t index) {
    auto itr = nodes_indices.find(index);

    if (itr != nodes_indices.end()) {
      return itr->second;
    }

    auto& new_node = result.add_node();
    nodes_indices.emplace(index, &new_node);
    return &new_node;
  };

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

    auto* current_node = get_node(current_index);

    if (cmd == 'f') {
      current_node->is_final = true;
    }

    char jump_symbol;
    while (line_is >> jump_symbol) {
      size_t jump_index;
      line_is >> jump_index;

      if (jump_symbol == '-') {
        jump_symbol = NonDeterministicFiniteAutomata::cEmptyChar;
      }

      current_node->jumps.emplace(jump_symbol, get_node(jump_index));
    }

    ++current_index;
  }

  return result;
}

NonDeterministicFiniteAutomata NonDeterministicFiniteAutomataReader::read(
    std::string_view string) {
  std::stringstream ss;
  ss << string;

  return read(ss);
}
