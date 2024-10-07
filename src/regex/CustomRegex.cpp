#include "CustomRegex.h"

#include <iostream>
#include <list>

#include "RegexPrinter.h"

namespace {
struct RegexOptimizeVisitor : RegexNodeVisitor {
  std::unique_ptr<RegexNode> result;

  // return true if L(big) contains L(small), false means idk
  static bool contains(const RegexNode& big, const RegexNode& small) {
    if (big == small) {
      return true;
    }

    if (small.is_of_type<ZeroNode>()) {
      return true;
    }

    if (big.is_of_type<StarNode>()) {
      if (small.is_of_type<OneNode>()) {
        return true;
      }

      auto& child = static_cast<const StarNode&>(big).child;

      if (*child == small) {
        return true;
      }
    }

    return false;
  }

  void optimize(std::unique_ptr<RegexNode>& node) {
    result = nullptr;

    auto clone = node->clone();

    node->accept(*this);

    if (result != nullptr) {
      node = std::move(result);
    }
  }

  template <typename T>
    requires std::same_as<T, OrNode> || std::same_as<T, ConcatenationNode>
  static std::list<std::unique_ptr<RegexNode>> flatten(T& node) {
    std::list<std::unique_ptr<RegexNode>> result;

    if (node.left->template is_of_type<T>()) {
      auto left_list = flatten(static_cast<T&>(*node.left));
      result.splice(result.end(), left_list);
    } else {
      result.push_back(std::move(node.left));
    }

    if (node.right->template is_of_type<T>()) {
      auto right_list = flatten(static_cast<T&>(*node.right));
      result.splice(result.end(), right_list);
    } else {
      result.push_back(std::move(node.right));
    }

    return result;
  }

  template <typename T>
    requires std::same_as<T, OrNode> || std::same_as<T, ConcatenationNode>
  static std::unique_ptr<RegexNode> make_tree(
      std::list<std::unique_ptr<RegexNode>>& list) {
    if (list.size() == 1) {
      return std::move(list.front());
    }

    auto front = std::move(list.front());
    list.pop_front();
    return std::make_unique<T>(std::move(front), make_tree<T>(list));
  }

  void visit(SymbolNode& node) override {}
  void visit(ZeroNode& node) override {}
  void visit(OneNode& node) override {}

  void visit(ConcatenationNode& node) override {
    auto list = flatten(node);

    bool is_empty = false;
    for (auto& regex_node : list) {
      optimize(regex_node);

      if (regex_node->is_of_type<ZeroNode>()) {
        is_empty = true;
      }
    }

    if (is_empty) {
      result = std::make_unique<ZeroNode>();
      return;
    }

    // remove all one nodes from list
    std::erase_if(list, [](const std::unique_ptr<RegexNode>& node) {
      return node->is_of_type<OneNode>();
    });

    if (list.empty()) {
      result = std::make_unique<OneNode>();
      return;
    }

    // build tree again
    result = make_tree<ConcatenationNode>(list);
  }

  void visit(OrNode& node) override {
    // we should flatten tree of or nodes and then remove extra ones
    auto list = flatten(node);

    for (auto itr = list.begin(); itr != list.end();) {
      optimize(*itr);

      if ((*itr)->is_of_type<ZeroNode>()) {
        itr = list.erase(itr);
      } else {
        ++itr;
      }
    }

    if (list.empty()) {
      result = std::make_unique<ZeroNode>();
      return;
    }

    for (auto left = list.begin(); left != list.end(); ++left) {
      for (auto right = list.begin(); right != list.end();) {
        if (left == right || !contains(**left, **right)) {
          ++right;
        } else {
          right = list.erase(right);
        }
      }
    }

    // build tree again
    result = make_tree<OrNode>(list);
  }

  void visit(StarNode& node) override {
    optimize(node.child);

    if (node.child->is_of_type<ZeroNode>()) {
      result = std::make_unique<OneNode>();
    } else if (node.child->is_of_type<OneNode>()) {
      result = std::move(node.child);
    }
  }
};
}  // namespace

Regex& Regex::optimize() {
  RegexOptimizeVisitor visitor;
  visitor.optimize(root_);

  return *this;
}

std::string Regex::to_string() const {
  std::stringstream result;
  result << RegexPrinter(*this);

  return result.str();
}
