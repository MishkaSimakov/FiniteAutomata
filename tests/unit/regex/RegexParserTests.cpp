#include <gtest/gtest.h>

#include <ranges>

#include "RegexParserTestCase.h"
#include "regex/RegexParser.h"

TEST_F(RegexParserTestCase, TestItParseSimpleConcatenation) {
  std::vector<std::string> regexes = {"abc(d)ef", "(a)",          "a",
                                      "a(b)",     "ab(cdefg)hjk", "abcdefg",
                                      "abcd(e)",  "(a)b  cdefg"};

  for (auto& regex : regexes) {
    auto result = RegexParser::parse(regex);

    std::erase_if(regex, [](char symbol) {
      return std::isspace(symbol) != 0 || symbol == '(' || symbol == ')';
    });

    ASSERT_TRUE(is_equals_to_string(*result, regex));
  }
}

TEST_F(RegexParserTestCase, TestItParseSimpleOrExpression) {
  std::string regex = "abc | bca";

  auto result = RegexParser::parse(regex);

  {
    const auto& or_node = node_cast<OrNode>(*result);
    ASSERT_TRUE(is_equals_to_string(*or_node.left, "abc") &&
                is_equals_to_string(*or_node.right, "bca"));
  }

  regex = "first | second | third";
  result = RegexParser::parse(regex);
  {
    const auto& first_or = node_cast<OrNode>(*result);
    const auto& second_or = node_cast<OrNode>(*first_or.right);

    ASSERT_TRUE(is_equals_to_string(*first_or.left, "first") &&
                is_equals_to_string(*second_or.left, "second") &&
                is_equals_to_string(*second_or.right, "third"));
  }
}

TEST_F(RegexParserTestCase, TestItParseStar) {
  {
    auto result = RegexParser::parse("(a)*");

    const auto& star_node = node_cast<StarNode>(*result);
    is_equals_to_string(*star_node.child, "a");
  }

  {
    auto result = RegexParser::parse("(abc)*(bca)*");

    const auto& concatenation = node_cast<ConcatenationNode>(*result);
    const auto& left_child = node_cast<StarNode>(*concatenation.left);
    const auto& right_child = node_cast<StarNode>(*concatenation.right);

    ASSERT_TRUE(is_equals_to_string(*left_child.child, "abc") &&
                is_equals_to_string(*right_child.child, "bca"));
  }

  {
    auto result = RegexParser::parse("(ab | ba)* | a");

    const auto& or_node = node_cast<OrNode>(*result);

    ASSERT_TRUE(is_equals_to_string(*or_node.right, "a"));

    const auto& left_child = node_cast<StarNode>(*or_node.left);
    const auto& left_child_or = node_cast<OrNode>(*left_child.child);

    ASSERT_TRUE(is_equals_to_string(*left_child_or.left, "ab") &&
                is_equals_to_string(*left_child_or.right, "ba"));
  }
}
