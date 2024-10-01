add_executable(
        Tests
        regex/RegexParserTests.cpp
        regex/RegexParserTestCase.h
        finite_automata/FiniteAutomataRegexTests.cpp
        finite_automata/NonDeterministicToDeterministicTests.cpp
        finite_automata/RegexStrings.h
        finite_automata/RegexStrings.cpp
        finite_automata/LanguagesEqualityTests.cpp
        finite_automata/MinimalAutomataTests.cpp
        finite_automata/ComplementTests.cpp
        finite_automata/AutomataToRegexTests.cpp
)

target_link_libraries(Tests GTest::gtest_main FiniteAutomata)
include(GoogleTest)
gtest_discover_tests(Tests)