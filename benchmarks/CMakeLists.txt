add_executable(
        Benchmarks
        finite_automata/NonDeterministicToDeterministicBenchmark.cpp
)

target_link_libraries(Benchmarks benchmark::benchmark FiniteAutomata)

# we do benchmark on optimized code
set_target_properties(Benchmarks PROPERTIES CMAKE_CXX_FLAGS "-O3")