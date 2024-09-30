#pragma once
#include <memory>

#define REGEX_CONST_NODE_VISITOR_VISIT(T) virtual void visit(const T& node) = 0;
#define REGEX_NODE_VISITOR_VISIT(T) virtual void visit(T& node) = 0;

#define REGEX_NODE_ACCEPT_VISITORS()                           \
  void accept(RegexConstNodeVisitor& visitor) const override { \
    visitor.visit(*this);                                      \
  }                                                            \
  void accept(RegexNodeVisitor& visitor) override { visitor.visit(*this); }

struct SymbolNode;
struct ZeroNode;
struct OneNode;
struct ConcatenationNode;
struct OrNode;
struct StarNode;

struct RegexConstNodeVisitor {
  REGEX_CONST_NODE_VISITOR_VISIT(SymbolNode)
  REGEX_CONST_NODE_VISITOR_VISIT(ZeroNode)
  REGEX_CONST_NODE_VISITOR_VISIT(OneNode)
  REGEX_CONST_NODE_VISITOR_VISIT(ConcatenationNode)
  REGEX_CONST_NODE_VISITOR_VISIT(OrNode)
  REGEX_CONST_NODE_VISITOR_VISIT(StarNode)

  virtual ~RegexConstNodeVisitor() = default;
};

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
 protected:
  virtual bool equal_base(const RegexNode& node) const = 0;

 public:
  virtual ~RegexNode() = default;

  virtual std::unique_ptr<RegexNode> clone() const = 0;

  template <typename T>
  bool is_of_type() const {
    return typeid(T) == typeid(*this);
  }

  virtual void accept(RegexConstNodeVisitor&) const = 0;
  virtual void accept(RegexNodeVisitor&) = 0;

  bool operator==(const RegexNode& other) const {
    if (typeid(*this) != typeid(other)) {
      return false;
    }

    return equal_base(other);
  }
};

// match symbol
struct SymbolNode final : RegexNode {
  char symbol;

  explicit SymbolNode(char symbol) : symbol(symbol) {}

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<SymbolNode>(symbol);
  }

  REGEX_NODE_ACCEPT_VISITORS()

 protected:
  bool equal_base(const RegexNode& node) const override {
    const auto& other = static_cast<const SymbolNode&>(node);

    return symbol == other.symbol;
  }
};

// match nothing
struct ZeroNode final : RegexNode {
  REGEX_NODE_ACCEPT_VISITORS()

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<ZeroNode>();
  }

 protected:
  bool equal_base(const RegexNode& node) const override { return true; }
};

// match only empty word
struct OneNode final : RegexNode {
  REGEX_NODE_ACCEPT_VISITORS()

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<OneNode>();
  }

 protected:
  bool equal_base(const RegexNode& node) const override { return true; }
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

  REGEX_NODE_ACCEPT_VISITORS()

 protected:
  bool equal_base(const RegexNode& node) const override {
    const auto& other = static_cast<const ConcatenationNode&>(node);

    return *left == *other.left && *right == *other.right;
  }
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

  REGEX_NODE_ACCEPT_VISITORS()

 protected:
  bool equal_base(const RegexNode& node) const override {
    const auto& other = static_cast<const OrNode&>(node);

    return *left == *other.left && *right == *other.right;
  }
};

// match (child)*
struct StarNode final : RegexNode {
  std::unique_ptr<RegexNode> child;

  explicit StarNode(std::unique_ptr<RegexNode> child)
      : child(std::move(child)) {}

  std::unique_ptr<RegexNode> clone() const override {
    return std::make_unique<StarNode>(child->clone());
  }

  REGEX_NODE_ACCEPT_VISITORS()

 protected:
  bool equal_base(const RegexNode& node) const override {
    const auto& other = static_cast<const StarNode&>(node);

    return *child == *other.child;
  }
};
