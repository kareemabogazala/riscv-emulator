cmake_minimum_required(VERSION 3.10)
project(riscv_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ============================
# ========== SOURCES =========
# ============================

# Main emulator sources
file(GLOB EMULATOR_SOURCES
    "src/*.cpp"
    "src/Memory/*.cpp"
)

# ============================
# ========== EXECUTABLE ======
# ============================

add_executable(emulator ${EMULATOR_SOURCES})

# ============================
# ========== TESTS ===========
# ============================

# Include paths
include_directories(
    include
    external
)

# Collect test sources
file(GLOB TEST_SOURCES
    "tests/test_main.cpp"
    "tests/regs_test/*.cpp"
    "tests/memory_test/*.cpp"
)

# Test executable
add_executable(run_tests
    ${TEST_SOURCES}
    src/regs.cpp
    src/Memory/memory.cpp
    src/Memory/DMEM.cpp
    src/Memory/IMEM.cpp
)

# Include paths for run_tests
target_include_directories(run_tests PRIVATE include external)
