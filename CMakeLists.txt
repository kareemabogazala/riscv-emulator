cmake_minimum_required(VERSION 3.10)
project(riscv_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# ============================
# ========== INCLUDE =========
# ============================

include_directories(
    include
    external
)

# ============================
# ========== SOURCES =========
# ============================

# Gather all emulator source files
file(GLOB_RECURSE EMULATOR_SOURCES
    "src/*.cpp"
)

# Separate out main.cpp so it can be excluded from test target
set(EMULATOR_SOURCES_NO_MAIN "")
foreach(source ${EMULATOR_SOURCES})
    if(NOT source MATCHES ".*src/main.cpp$")
        list(APPEND EMULATOR_SOURCES_NO_MAIN ${source})
    endif()
endforeach()

# ============================
# ========== EXECUTABLE ======
# ============================

# Emulator with real main.cpp
add_executable(emulator
    ${EMULATOR_SOURCES}
)

target_include_directories(emulator PRIVATE include external)

# ============================
# ========== TESTS ===========
# ============================

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    # Gather test sources
    file(GLOB_RECURSE TEST_SOURCES
        "tests/*.cpp"
    )

    # Build run_tests (without main.cpp from src)
    add_executable(run_tests
        ${TEST_SOURCES}
        ${EMULATOR_SOURCES_NO_MAIN}
    )

    target_include_directories(run_tests PRIVATE include external)

    # target_link_libraries(run_tests ...) # Add Catch2 or other libs if needed
endif()
