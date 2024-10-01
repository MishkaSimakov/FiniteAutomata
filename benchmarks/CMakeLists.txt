add_executable(
        Benchmarks
        finite_automata/NonDeterministicToDeterministicBenchmark.cpp
)

target_link_libraries(Benchmarks benchmark::benchmark FiniteAutomata)