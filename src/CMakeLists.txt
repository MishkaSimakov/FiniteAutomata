add_library(FiniteAutomata
        weaponry/NonDeterministicFiniteAutomata.h
        weaponry/NonDeterministicFiniteAutomata.cpp
        regex/RegexParser.cpp
        regex/RegexParser.h
        regex/RegexNodes.h
        regex/RegexPrinter.cpp
        regex/RegexPrinter.h
        weaponry/FiniteAutomata.h
        regex/CustomRegex.h
        weaponry/CharsetInfo.h
        weaponry/parts/NonDeterministicToDeterministic.cpp
        weaponry/FiniteAutomataPrinter.h
        weaponry/parts/DeterministicToRegex.cpp
        weaponry/FiniteAutomata.cpp
        regex/CustomRegex.cpp
        regex/CustomRegex.cpp
        weaponry/parts/DeterministicToMinimal.cpp
        weaponry/parts/AutomataEquivalenceCheck.cpp
)

target_link_libraries(FiniteAutomata fmt::fmt)

target_include_directories(FiniteAutomata PUBLIC . weaponry regex)