#pragma once
#include <memory>
#include <string_view>

#include "RegexNodes.h"

struct ParseError final : std::runtime_error {
  ParseError() : runtime_error("Error occured while parsing regex") {}
};

class RegexParser {
  static std::string_view::iterator get_matching_paren(std::string_view regex);

  static std::string_view trim(std::string_view);

  static std::unique_ptr<RegexNode> parse_recursively(std::string_view regex);

 public:
  RegexParser() = default;

  static std::unique_ptr<RegexNode> parse(std::string_view regex);
};
