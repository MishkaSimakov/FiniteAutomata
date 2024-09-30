#include "RegexParser.h"

#include <locale>
#include <vector>

struct ParseError final : std::runtime_error {
  ParseError() : runtime_error("Error occured while parsing regex") {}
};

std::string_view::iterator RegexParser::get_matching_paren(
    std::string_view regex) {
  // TODO: check only in debug
  if (regex.empty() || regex.front() != '(') {
    throw std::runtime_error("Wrong usage of function");
  }

  int balance = 1;

  for (auto itr = std::next(regex.begin()); itr != regex.end(); ++itr) {
    if (*itr == '(') {
      ++balance;
    } else if (*itr == ')') {
      --balance;
    }

    if (balance == 0) {
      return itr;
    }
  }

  throw ParseError();
}

std::string_view RegexParser::trim(std::string_view string) {
  while (!string.empty() && std::isspace(string.front()) != 0) {
    string.remove_prefix(1);
  }

  while (!string.empty() && std::isspace(string.back()) != 0) {
    string.remove_suffix(1);
  }

  return string;
}

std::unique_ptr<RegexNode> RegexParser::parse_recursively(
    std::string_view regex) {
  // search for top-level +
  int balance = 0;
  for (auto itr = regex.begin(); itr != regex.end(); ++itr) {
    if (*itr == '(') {
      ++balance;
    } else if (*itr == ')') {
      --balance;
    } else if (*itr == '+' && balance == 0) {
      auto right = parse_recursively({std::next(itr), regex.end()});
      auto left = parse_recursively({regex.begin(), itr});

      return std::make_unique<OrNode>(std::move(left), std::move(right));
    }
  }

  // if there isn't any "+" inside we should just concatenate each part
  regex = trim(regex);

  if (regex.empty()) {
    return nullptr;
  }

  // we split regex into two parts: left + right and parse them recursively
  std::unique_ptr<RegexNode> left;

  if (regex.front() == '(') {
    auto end_itr = get_matching_paren(regex);

    if (end_itr == std::next(regex.begin())) {
      // we dont allow empty parens, e.g "() + a"
      throw ParseError();
    }

    left = parse_recursively({std::next(regex.begin()), end_itr});

    ++end_itr;
    while (*end_itr == '*') {
      left = std::make_unique<StarNode>(std::move(left));
      ++end_itr;
    }

    regex.remove_prefix(end_itr - regex.begin());
  } else if (regex.front() == '1') {
    left = std::make_unique<OneNode>();
    regex.remove_prefix(1);
  } else if (regex.front() == '0') {
    left = std::make_unique<ZeroNode>();
    regex.remove_prefix(1);
  } else {
    left = std::make_unique<SymbolNode>(regex.front());
    regex.remove_prefix(1);
  }

  if (regex.empty()) {
    return left;
  }

  auto right = parse_recursively(regex);
  return std::make_unique<ConcatenationNode>(std::move(left), std::move(right));
}

std::unique_ptr<RegexNode> RegexParser::parse(std::string_view regex) {
  return parse_recursively(regex);
}
