#include <gtest/gtest.h>

#include <ranges>

#include "RegexParserTestCase.h"
#include "regex/RegexParser.h"

TEST_F(RegexParserTestCase, test_it_parse_simple_concatenation) {
  std::vector<std::string> regexes = {
      "abbbaa",         "(a)",   "a",          "a(b)",
      "ab(ababa)abbbb", "bbbbb", "bababababa", "(a)b  bababa"};

  for (auto& regex : regexes) {
    auto result = RegexParser::parse(regex);

    std::erase_if(regex, [](char symbol) {
      return std::isspace(symbol) != 0 || symbol == '(' || symbol == ')';
    });

    ASSERT_TRUE(is_equals_to_string(*result, regex));
  }
}

TEST_F(RegexParserTestCase, test_it_parse_simple_or_expression) {
  std::string regex = "abb + bba";

  auto result = RegexParser::parse(regex);

  {
    const auto& or_node = node_cast<OrNode>(*result);
    ASSERT_TRUE(is_equals_to_string(*or_node.left, "abb") &&
                is_equals_to_string(*or_node.right, "bba"));
  }

  regex = "babbabbab + babbab + bab";
  result = RegexParser::parse(regex);
  {
    const auto& first_or = node_cast<OrNode>(*result);
    const auto& second_or = node_cast<OrNode>(*first_or.right);

    ASSERT_TRUE(is_equals_to_string(*first_or.left, "babbabbab") &&
                is_equals_to_string(*second_or.left, "babbab") &&
                is_equals_to_string(*second_or.right, "bab"));
  }
}

TEST_F(RegexParserTestCase, test_it_parse_star) {
  {
    auto result = RegexParser::parse("(a)*");

    const auto& star_node = node_cast<StarNode>(*result);
    is_equals_to_string(*star_node.child, "a");
  }

  {
    auto result = RegexParser::parse("(ab)*(ba)*");

    const auto& concatenation = node_cast<ConcatenationNode>(*result);
    const auto& left_child = node_cast<StarNode>(*concatenation.left);
    const auto& right_child = node_cast<StarNode>(*concatenation.right);

    ASSERT_TRUE(is_equals_to_string(*left_child.child, "ab") &&
                is_equals_to_string(*right_child.child, "ba"));
  }

  {
    auto result = RegexParser::parse("(ab + ba)* + a");

    const auto& or_node = node_cast<OrNode>(*result);

    ASSERT_TRUE(is_equals_to_string(*or_node.right, "a"));

    const auto& left_child = node_cast<StarNode>(*or_node.left);
    const auto& left_child_or = node_cast<OrNode>(*left_child.child);

    ASSERT_TRUE(is_equals_to_string(*left_child_or.left, "ab") &&
                is_equals_to_string(*left_child_or.right, "ba"));
  }
}

TEST_F(RegexParserTestCase, test_it_throws_when_syntax_is_wrong) {
  std::vector wrong_regexes = {
      "() + abb",      "(a + b)(",     "((a + b)",  "a*",
      "(a + )",        "( + a)",       "+ aba",     "aba)",
      "0000 + *",      "ababa)*",      "(abababa*", "a + b + a + 0 + ",
      "(ababa + ()*)", "a + + bababa", "1 +++ 1",   "(a)*+*"};

  for (auto& regex : wrong_regexes) {
    ASSERT_THROW({ RegexParser::parse(regex); }, ParseError)
                                     << fmt::format(
                                            "Parser must have thrown when "
                                            "parsing {} but it hasn't",
                                            regex);
  }
}
