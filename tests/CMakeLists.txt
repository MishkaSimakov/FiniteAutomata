add_executable(
        Tests
        unit/regex/RegexParserTests.cpp
        unit/regex/RegexParserTestCase.h
        unit/finite_automata/FiniteAutomataRegexTests.cpp
        unit/finite_automata/NonDeterministicToDeterministicTests.cpp
        unit/finite_automata/RegexStrings.h
        unit/finite_automata/RegexStrings.cpp
        unit/finite_automata/LanguagesEqualityTests.cpp
        unit/finite_automata/MinimalAutomataTests.cpp
)

target_link_libraries(Tests GTest::gtest_main FiniteAutomata)
include(GoogleTest)
gtest_discover_tests(Tests)