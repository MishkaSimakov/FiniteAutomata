add_executable(
        Tests
        regex/RegexParserTests.cpp
        regex/RegexParserTestCase.h
        weaponry/NonDeterministicToDeterministicTests.cpp
        RegexStrings.h
        RegexStrings.cpp
        weaponry/LanguagesEqualityTests.cpp
        weaponry/MinimalAutomataTests.cpp
        weaponry/ComplementTests.cpp
        weaponry/AutomataToRegexTests.cpp
        weaponry/NonDeterministicTests.cpp
        regex/RegexOptimizationTests.cpp
        weaponry/AutomataPrinterTests.cpp
        cli/FAReaderTests.cpp
)
target_include_directories(Tests PRIVATE .)

set_target_properties(Tests PROPERTIES CMAKE_CXX_FLAGS "-fsanitize=address")

target_link_libraries(Tests GTest::gtest_main FiniteAutomata)
include(GoogleTest)
gtest_discover_tests(Tests)