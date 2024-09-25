#pragma once
#include <ostream>

#include "RegexNodes.h"

// TODO: make better parentesis
class RegexPrinter {
  const RegexNode& node_;

 public:
  const RegexNode& get_root() const { return node_; }

  explicit RegexPrinter(const RegexNode& node);
};

std::ostream& operator<<(std::ostream& os, const RegexPrinter& printer);
