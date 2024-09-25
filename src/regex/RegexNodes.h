#pragma once
#include <memory>

#define REGEX_NODE_VISITOR_VISIT(T) virtual void visit(const T& node) = 0;
#define REGEX_NODE_ACCEPT_VISITOR()                       \
  void accept(RegexNodeVisitor& visitor) const override { \
    visitor.visit(*this);                                 \
  }

struct SymbolNode;
struct ZeroNode;
struct OneNode;
struct ConcatenationNode;
struct OrNode;
struct StarNode;

struct RegexNodeVisitor {
  REGEX_NODE_VISITOR_VISIT(SymbolNode)
  REGEX_NODE_VISITOR_VISIT(ZeroNode)
  REGEX_NODE_VISITOR_VISIT(OneNode)
  REGEX_NODE_VISITOR_VISIT(ConcatenationNode)
  REGEX_NODE_VISITOR_VISIT(OrNode)
  REGEX_NODE_VISITOR_VISIT(StarNode)

  virtual ~RegexNodeVisitor() = default;
};

struct RegexNode {
  virtual ~RegexNode() = default;

  virtual std::unique_ptr<RegexNode> clone() const = 0;

  template <typename T>
  bool is_of_type() const {
    return typeid(T) == typeid(*this);
  }

  virtual void accept(RegexNodeVisitor&) const = 0;
};

// match symbol
struct SymbolNode final : RegexNode {
  char symbol;

  explicit SymbolNode(char symbol) : symbol(symbol) {}

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<SymbolNode>(symbol);
  }

  REGEX_NODE_ACCEPT_VISITOR()
};

// match nothing
struct ZeroNode final : RegexNode {
  REGEX_NODE_ACCEPT_VISITOR()

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<ZeroNode>();
  }
};

// match only empty word
struct OneNode final : RegexNode {
  REGEX_NODE_ACCEPT_VISITOR()

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<OneNode>();
  }
};

// match left . right
struct ConcatenationNode final : RegexNode {
  std::unique_ptr<RegexNode> left;
  std::unique_ptr<RegexNode> right;

  ConcatenationNode(std::unique_ptr<RegexNode> left,
                    std::unique_ptr<RegexNode> right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<ConcatenationNode>(left->clone(), right->clone());
  }

  REGEX_NODE_ACCEPT_VISITOR()
};

// match left + right
struct OrNode final : RegexNode {
  OrNode(std::unique_ptr<RegexNode> left, std::unique_ptr<RegexNode> right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<RegexNode> left;
  std::unique_ptr<RegexNode> right;

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<OrNode>(left->clone(), right->clone());
  }

  REGEX_NODE_ACCEPT_VISITOR()
};

// match (child)*
struct StarNode final : RegexNode {
  std::unique_ptr<RegexNode> child;

  explicit StarNode(std::unique_ptr<RegexNode> child)
      : child(std::move(child)) {}

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<StarNode>(child->clone());
  }

  REGEX_NODE_ACCEPT_VISITOR()
};
