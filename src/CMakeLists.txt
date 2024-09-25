add_library(FiniteAutomata
        weaponry/FiniteAutomata.h
        weaponry/FiniteAutomata.cpp
        regex/RegexParser.cpp
        regex/RegexParser.h
        regex/RegexNodes.h
        regex/RegexPrinter.cpp
        regex/RegexPrinter.h
        weaponry/DeterministicFiniteAutomata.cpp
        weaponry/DeterministicFiniteAutomata.h
        regex/Regex.h
        regex/Regex.h
)

target_include_directories(FiniteAutomata PUBLIC . weaponry regex)