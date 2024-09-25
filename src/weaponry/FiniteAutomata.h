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

constexpr size_t cLettersCount = 27;

class FiniteAutomata {
  struct FiniteAutomataBuilderVisitor;

  struct Node {
    bool is_final{false};
    std::unordered_multimap<char, Node*> jumps;
  };

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
};
