#pragma once
#include <fmt/format.h>

#include <tuple>
#include <vector>

#include "CustomRegex.h"
#include "RegexPrinter.h"

extern const std::vector<
    std::tuple<Regex, std::vector<const char*>, std::vector<const char*>>>
    regex_strings;

enum RegexStringType { CORRECT, INCORRECT };

inline std::string error_message(const Regex& regex, std::string_view string,
                                 RegexStringType string_type) {
  if (string_type == CORRECT) {
    return fmt::format(
        "Non-deterministic finite automata for regex {} should accept string "
        "{:?} but it doesn't.",
        regex, string);
  }

  if (string_type == INCORRECT) {
    return fmt::format(
        "Non-deterministic finite automata for regex {} should NOT accept "
        "string {:?} but it does.",
        regex, string);
  }

  return "";
}
