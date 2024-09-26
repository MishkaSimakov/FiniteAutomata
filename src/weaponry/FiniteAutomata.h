#pragma once

#include <array>
#include <concepts>
#include <deque>
#include <list>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "RegexNodes.h"

class FiniteAutomata {
  struct FiniteAutomataBuilderVisitor;

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

  explicit FiniteAutomata(RegexNode& regex);

  Node& get_start_node() { return nodes_.front(); }
  Node& add_node() { return nodes_.emplace_back(); }

  auto get_final_nodes() {
    return nodes_ |
           std::views::filter([](const Node& node) { return node.is_final; });
  }

  bool containts_word(std::string_view word) const;

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
