#include <benchmark/benchmark.h>

#include <iostream>
#include <string>

#include "FiniteAutomata.h"

static void build_deterministic(benchmark::State& state) {
  size_t regex_size = state.range(0);

  // construct non-deterministic from regex
  std::vector<std::string> regexes;
  regexes.emplace_back(regex_size, 'a');

  std::string regex;
  for (size_t i = 0; i * 5 < regex_size; ++i) {
    regex += "(a + b)";
  }
  regexes.push_back(std::move(regex));

  regex.clear();
  for (size_t i = 0; i * 5 < regex_size; ++i) {
    regex += "(1 + a)";
  }
  regexes.push_back(std::move(regex));

  std::vector<NonDeterministicFiniteAutomata> non_deterministic;
  non_deterministic.reserve(regexes.size());

  for (auto& regex : regexes) {
    non_deterministic.emplace_back(Regex(regex));
  }

  while (state.KeepRunning()) {
    for (auto& automata : non_deterministic) {
      auto deterministic = FiniteAutomata(automata);
      benchmark::DoNotOptimize(deterministic);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(build_deterministic)
    ->Unit(benchmark::kSecond)
    ->RangeMultiplier(2)
    ->Range(16, 2048);

BENCHMARK_MAIN();
