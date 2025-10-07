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

# Separate out main.cpp (useful if tests are enabled)
set(EMULATOR_SOURCES_NO_MAIN "")
foreach(source ${EMULATOR_SOURCES})
    if(NOT source MATCHES ".*src/main.cpp$")
        list(APPEND EMULATOR_SOURCES_NO_MAIN ${source})
    endif()
endforeach()

# ============================
# ========== EXECUTABLE ======
# ============================

add_executable(emulator
    ${EMULATOR_SOURCES}
)

target_include_directories(emulator PRIVATE include external)

# ============================
# ========== TESTS ===========
# ============================

option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    message(STATUS "Building unit tests...")

    # Gather test sources
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

    # Build run_tests target (without main.cpp from src)
    add_executable(run_tests
        ${TEST_SOURCES}
        ${EMULATOR_SOURCES_NO_MAIN}
    )

    target_include_directories(run_tests PRIVATE include external)

    # Example (if using Catch2 or GoogleTest)
    # target_link_libraries(run_tests PRIVATE Catch2::Catch2)
else()
    message(STATUS "Skipping unit tests (BUILD_TESTS=OFF)")
endif()
