#pragma once
#include <fmt/format.h>

#include <ostream>
#include <sstream>

#include "CustomRegex.h"
#include "RegexNodes.h"

// TODO: make better parentesis
class RegexPrinter {
  const Regex& regex_;

 public:
  const RegexNode& get_root() const { return regex_.get_root(); }

  explicit RegexPrinter(const Regex& regex);
};

std::ostream& operator<<(std::ostream& os, const RegexPrinter& printer);

template <>
struct fmt::formatter<Regex> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename Context>
  constexpr auto format(const Regex& regex, Context& ctx) const {
    std::stringstream regex_string;
    regex_string << RegexPrinter(regex);

    return format_to(ctx.out(), "{}", regex_string.str());
  }
};
