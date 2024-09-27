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
        regex/CustomRegex.h
        weaponry/CharsetInfo.h
        weaponry/CharsetInfo.h
        weaponry/converters/NonDeterministicToDeterministic.h
        weaponry/FiniteAutomataPrinter.h
)

target_link_libraries(FiniteAutomata fmt::fmt)

target_include_directories(FiniteAutomata PUBLIC . weaponry regex)