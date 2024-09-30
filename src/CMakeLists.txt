add_library(FiniteAutomata
        weaponry/FiniteAutomata.h
        weaponry/FiniteAutomata.cpp
        regex/RegexParser.cpp
        regex/RegexParser.h
        regex/RegexNodes.h
        regex/RegexPrinter.cpp
        regex/RegexPrinter.h
        weaponry/DeterministicFiniteAutomata.h
        regex/CustomRegex.h
        weaponry/CharsetInfo.h
        weaponry/converters/NonDeterministicToDeterministic.cpp
        weaponry/FiniteAutomataPrinter.h
        weaponry/converters/DeterministicToRegex.cpp
        weaponry/DeterministicFiniteAutomata.cpp
        regex/CustomRegex.cpp
        regex/CustomRegex.cpp
        weaponry/converters/DeterministicToMinimal.cpp
)

target_link_libraries(FiniteAutomata fmt::fmt)

target_include_directories(FiniteAutomata PUBLIC . weaponry regex)