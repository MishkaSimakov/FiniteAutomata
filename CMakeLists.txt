cmake_minimum_required(VERSION 3.28)
project(FiniteAutomata)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

include(FetchContent)

# install fmt
find_package(fmt)
IF (NOT fmt_FOUND)
    message("fmt not found locally, installing from github...")

    FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt
            GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
    FetchContent_MakeAvailable(fmt)
ENDIF ()

# install GTEST
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_subdirectory(src)

# testing target
add_subdirectory(tests)

# just example target
add_executable(Example main.cpp)
target_link_libraries(Example FiniteAutomata)