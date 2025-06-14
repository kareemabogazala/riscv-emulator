cmake_minimum_required(VERSION 3.10)
project(riscv_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include paths
include_directories(include)

# Emulator build
file(GLOB SOURCES "src/*.cpp")
add_executable(emulator ${SOURCES})

# ============================
# ========== TESTS ===========
# ============================

# Include Catch2 header
include_directories(external)

# Collect test files
file(GLOB TEST_SOURCES "tests/regs_test/*.cpp")

# Build test executable
add_executable(run_tests ${TEST_SOURCES} src/regs.cpp)  # <-- Manually link needed src

# Link additional include folders
target_include_directories(run_tests PRIVATE include external)
