#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <deque>
#include <list>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CharsetInfo.h"
#include "CustomRegex.h"

template <typename Charset = CharsetInfo>
class FiniteAutomata {
  struct FiniteAutomataBuilderVisitor final : RegexNodeVisitor {
    FiniteAutomata result;

    void visit(const SymbolNode& node) override {
      auto& start_node = result.get_start_node();
      auto& end_node = result.add_node();

      start_node.jumps.emplace(node.symbol, &end_node);
      end_node.is_final = true;
    }

    void visit(const ZeroNode& node) override {}
    void visit(const OneNode& node) override {
      auto& start_node = result.get_start_node();
      auto& end_node = result.add_node();

      start_node.jumps.emplace(cEmptyChar, &end_node);
      end_node.is_final = true;
    }

    void visit(const ConcatenationNode& node) override {
      FiniteAutomata concatenated;

      node.left->accept(*this);

      std::swap(concatenated, result);

      node.right->accept(*this);

      for (Node& final_node : concatenated.get_final_nodes()) {
        final_node.is_final = false;
        final_node.jumps.emplace(cEmptyChar, &result.get_start_node());
      }

      result.nodes_.splice(result.nodes_.begin(), concatenated.nodes_);
    }

    void visit(const OrNode& node) override {
      FiniteAutomata concatenated;

      node.left->accept(*this);

      std::swap(concatenated, result);

      node.right->accept(*this);

      Node& left_front = concatenated.get_start_node();
      Node& right_front = result.get_start_node();

      Node& front = result.nodes_.emplace_front();
      front.jumps.emplace(cEmptyChar, &left_front);
      front.jumps.emplace(cEmptyChar, &right_front);

      result.nodes_.splice(result.nodes_.end(), concatenated.nodes_);
    }

    void visit(const StarNode& node) override {
      node.child->accept(*this);

      auto& old_front = result.get_start_node();
      auto& front = result.nodes_.emplace_front();

      front.jumps.emplace(cEmptyChar, &old_front);

      for (auto& final_node : result.get_final_nodes()) {
        final_node.is_final = false;
        final_node.jumps.emplace(cEmptyChar, &front);
      }

      front.is_final = true;
    }
  };

 public:
  struct Node {
    bool is_final{false};
    std::unordered_multimap<char, Node*> jumps;
  };

 private:
  std::list<Node> nodes_;

  template <typename T>
    requires(std::same_as<T, const Node*> || std::same_as<T, Node*>)
  static void do_empty_jumps(std::unordered_set<T>& nodes) {
    std::list<const Node*> unprocessed;
    std::copy(nodes.begin(), nodes.end(), std::back_inserter(unprocessed));
    while (!unprocessed.empty()) {
      const Node* current = unprocessed.front();
      unprocessed.pop_front();

      for (const auto& [symbol, next] : current->jumps) {
        if (symbol == cEmptyChar && !nodes.contains(next)) {
          nodes.insert(next);
          unprocessed.push_back(next);
        }
      }
    }
  }

  void remove_unreachable_nodes();

 public:
  static constexpr char cEmptyChar = ' ';

  FiniteAutomata() {
    // emplace start node at position 0
    nodes_.emplace_back();
  }

  explicit FiniteAutomata(const Regex& regex) {
    FiniteAutomataBuilderVisitor visitor;
    regex.get_root().accept(visitor);

    *this = std::move(visitor.result);
  }

  Node& get_start_node() { return nodes_.front(); }
  Node& add_node() { return nodes_.emplace_back(); }

  auto get_final_nodes() {
    return nodes_ |
           std::views::filter([](const Node& node) { return node.is_final; });
  }

  bool containts_word(std::string_view word) const {
    std::unordered_set<const Node*> current;
    current.insert(&nodes_.front());

    while (!word.empty()) {
      char current_letter = word.front();

      // jump by edges with empty letter
      do_empty_jumps(current);

      // do non-empty jumps
      std::unordered_set<const Node*> new_current;
      for (const Node* node : current) {
        auto [begin, end] = node->jumps.equal_range(current_letter);

        for (; begin != end; ++begin) {
          new_current.insert(begin->second);
        }
      }

      current = std::move(new_current);
      word.remove_prefix(1);
    }

    do_empty_jumps(current);

    return std::ranges::any_of(current,
                               [](const Node* node) { return node->is_final; });
  }

  void remove_empty_jumps();

  size_t size() const { return nodes_.size(); }

  const std::list<Node>& get_nodes() const { return nodes_; }

  template <typename T>
    requires(std::same_as<T, const Node*> || std::same_as<T, Node*>)
  static void do_jumps(std::unordered_set<T>& nodes, char letter) {
    do_empty_jumps(nodes);

    std::unordered_set<T> next_nodes;
    for (const Node* node : nodes) {
      auto [beg, end] = node->jumps.equal_range(letter);

      for (; beg != end; ++beg) {
        next_nodes.insert(beg->second);
      }
    }

    do_empty_jumps(next_nodes);
    nodes = std::move(next_nodes);
  }
};
