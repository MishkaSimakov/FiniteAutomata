#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "RegexStrings.h"

inline std::vector equivalent_regexes = {
    std::pair{"1 + a + aa + aaa + aaaa(a)*", "(a)*"},
    std::pair{"aba + baa", "(ab + ba)a"},
    std::pair{"(a + 1)*", "(a)*"},
    std::pair{"(0 + ((0 + 0(0 + 0)0) + 0))0(0 + 0)", "0"},
    std::pair{"1(1 + 1 + 1)(1 + 0)", "1"},
    std::pair{"(a + b)(a + b)", "aa + ab + ba + bb"},
    std::pair{"aaaa", "aaaa"},
    std::pair{"(1 + a)(1 + b)(1 + b)(1 + a)",
              "(1 + a + b + ab)(1 + a + b + ba)"},
    std::pair{"111a1111a", "(a + a + 1a)(a + a1 + 1a + 0)"},
};
inline std::vector not_equivalent_regexes = {
    std::pair{"a(a)*", "(a)*"},
    std::pair{"abba", "abaa"},
    std::pair{"(1 + a)(1 + b)(1 + a)", "a + b + ab + ba + aba"},
    std::pair{"ab(a + b)", "aba + abb + aaa"},
    std::pair{"0", "1"},
    std::pair{"0(0 + 1)((1 + 1) + 1)", "1"},
    std::pair{"1(1 + 0)((0 + (0 + 1)) + 0)", "0"},
    std::pair{"a", "ab"}};

void assert_languages_equal(const FiniteAutomata& left,
                            const FiniteAutomata& right) {
  ASSERT_TRUE(left.is_language_equal(right) && right.is_language_equal(left));
}

void assert_languages_not_equal(const FiniteAutomata& left,
                                const FiniteAutomata& right) {
  ASSERT_TRUE(!left.is_language_equal(right) && !right.is_language_equal(left));
}

TEST(LanguagesEqualityTests, test_languages_from_regex_are_equal) {
  for (auto& [left, right] : equivalent_regexes) {
    auto left_automata = FiniteAutomata(Regex(left));
    auto right_automata = FiniteAutomata(Regex(right));

    assert_languages_equal(left_automata, right_automata);
  }

  for (auto& [left, right] : not_equivalent_regexes) {
    auto left_automata = FiniteAutomata(Regex(left));
    auto right_automata = FiniteAutomata(Regex(right));

    assert_languages_not_equal(left_automata, right_automata);
  }
}

TEST(LanguagesEqualityTests, test_languages_from_regex_complements_are_equal) {
  // if L1 == L2 then sigma* \ L1 == sigma* \ L2

  for (auto& [left, right] : equivalent_regexes) {
    auto left_automata = FiniteAutomata(Regex(left));
    auto right_automata = FiniteAutomata(Regex(right));

    left_automata.complement();
    right_automata.complement();

    assert_languages_equal(left_automata, right_automata);
  }

  for (auto& [left, right] : not_equivalent_regexes) {
    auto left_automata = FiniteAutomata(Regex(left));
    auto right_automata = FiniteAutomata(Regex(right));

    left_automata.complement();
    right_automata.complement();

    assert_languages_not_equal(left_automata, right_automata);
  }
}

TEST(LanguagesEqualityTests, test_complement_operation_is_idempotent) {
  // L1 != sigma* - L1 and L1 == sigma* - (sigma* - L1)

  auto test_regex = [](std::string_view string) {
    auto automata = FiniteAutomata(Regex(string));

    FiniteAutomata copy = automata;
    copy.complement();

    assert_languages_not_equal(automata, copy);

    copy.complement();
    assert_languages_equal(automata, copy);
  };

  for (auto& [left, right] : equivalent_regexes) {
    test_regex(left);
    test_regex(right);
  }

  for (auto& [left, right] : not_equivalent_regexes) {
    test_regex(left);
    test_regex(right);
  }
}
