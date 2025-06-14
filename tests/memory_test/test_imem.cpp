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
}
