#include "catch.hpp"
#include "Memory/IMEM.h"

static constexpr uint32_t BASE = 0x3000;
static constexpr uint32_t SIZE = 64;

TEST_CASE("IMem: read‐only region behaves correctly", "[imem][memory]")
{
    IMem imem(BASE, SIZE);

    SECTION("Initial contents are zero")
    {
        REQUIRE(imem.read8(BASE) == 0);
        REQUIRE(imem.read16(BASE) == 0);
        REQUIRE(imem.read32(BASE) == 0);
    }

    SECTION("Out‐of‐range reads throw")
    {
        REQUIRE_THROWS_AS(imem.read8(BASE + SIZE), std::out_of_range);
        REQUIRE_THROWS_AS(imem.read16(BASE + SIZE - 1), std::out_of_range);
        REQUIRE_THROWS_AS(imem.read32(BASE + SIZE - 3), std::out_of_range);
    }

    SECTION("All write methods throw")
    {
        REQUIRE_THROWS_AS(imem.write8(BASE, 0xFF), std::runtime_error);
        REQUIRE_THROWS_AS(imem.write16(BASE, 0xFFFF), std::runtime_error);
        REQUIRE_THROWS_AS(imem.write32(BASE, 0xDEADBEEF), std::runtime_error);
    }
    SECTION("Partial in-bounds read32 near edge")
    {
        // Write directly into internal data for testing only
        IMem hack(BASE, SIZE);
        std::vector<uint8_t> code = {0x01, 0x02, 0x03, 0x04};
        hack.load_code(code, SIZE - 4); // last possible valid location
        REQUIRE(hack.read32(BASE + SIZE - 4) == 0x04030201);
    }

    SECTION("load_code properly inserts instruction bytes")
    {
        std::vector<uint8_t> program = {0x13, 0x05, 0x00, 0x00}; // addi a0, zero, 0
        imem.load_code(program);

        REQUIRE(imem.read8(BASE + 0) == 0x13);
        REQUIRE(imem.read8(BASE + 1) == 0x05);
        REQUIRE(imem.read8(BASE + 2) == 0x00);
        REQUIRE(imem.read8(BASE + 3) == 0x00);
        REQUIRE(imem.read32(BASE + 0) == 0x00000513);
    }
}
