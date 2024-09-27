#pragma once
#include <string_view>

#include "RegexNodes.h"
#include "RegexParser.h"

class Regex {
  std::unique_ptr<RegexNode> root_;

 public:
  explicit Regex(std::string_view string)
      : root_(RegexParser::parse(string)) {}

  const RegexNode& get_root() const { return *root_; }

  Regex(const Regex& other) : root_(other.root_->clone()) {}
  Regex(Regex&& other) = default;

  Regex& operator=(const Regex& other) {
    if (this != &other) {
      root_ = other.root_->clone();
    }

    return *this;
  }
  Regex& operator=(Regex&&) = default;

  Regex& operator+=(const Regex& other) {
    if (this != &other) {
      root_ = std::make_unique<OrNode>(std::move(root_), other.root_->clone());
    }

    return *this;
  }

  Regex& operator*=(const Regex& other) {
    if (this != &other) {
      root_ = std::make_unique<ConcatenationNode>(std::move(root_),
                                                  other.root_->clone());
    }

    return *this;
  }
};

inline Regex operator+(const Regex& left, const Regex& right) {
  Regex copy = left;
  copy += right;
  return copy;
}

inline Regex operator*(const Regex& left, const Regex& right) {
  Regex copy = left;
  copy *= right;
  return copy;
}
