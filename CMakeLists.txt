cmake_minimum_required(VERSION 3.28)
project(FiniteAutomata)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# testing
enable_testing()

add_executable(
        Tests
)
target_link_libraries(Tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(Tests)


add_executable(FiniteAutomata main.cpp)