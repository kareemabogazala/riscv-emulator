#include "catch.hpp"
#include "Memory/DMEM.h"

static constexpr uint32_t BASE = 0x2000;
static constexpr uint32_t SIZE = 128;

TEST_CASE("DMem: full functionality and edge cases", "[dmem][memory]")
{
    DMem dmem(BASE, SIZE);

    SECTION("Zero initialization across region")
    {
        // Sample a few points
        REQUIRE(dmem.read8(BASE) == 0);
        REQUIRE(dmem.read8(BASE + SIZE / 2) == 0);
        REQUIRE(dmem.read8(BASE + SIZE - 1) == 0);
    }

    SECTION("8-bit read/write")
    {
        dmem.write8(BASE + 5, 0xAB);
        REQUIRE(dmem.read8(BASE + 5) == 0xAB);
        dmem.write8(BASE + 5, 0x12);
        REQUIRE(dmem.read8(BASE + 5) == 0x12);
    }

    SECTION("16-bit read/write (aligned and unaligned)")
    {
        // Aligned
        dmem.write16(BASE + 10, 0xFACE);
        REQUIRE(dmem.read16(BASE + 10) == 0xFACE);
        // Overwrite low/high byte individually
        dmem.write8(BASE + 10, 0x12);
        dmem.write8(BASE + 11, 0x34);
        REQUIRE(dmem.read16(BASE + 10) == 0x3412);

        // Unaligned should still work
        dmem.write16(BASE + 11, 0xBEEF);
        REQUIRE(dmem.read16(BASE + 11) == 0xBEEF);
    }

    SECTION("32-bit read/write (aligned and unaligned)")
    {
        // Aligned
        dmem.write32(BASE + 20, 0xCAFEBABE);
        REQUIRE(dmem.read32(BASE + 20) == 0xCAFEBABE);
        // Overwrite bytes
        dmem.write8(BASE + 20, 0x78);
        dmem.write8(BASE + 23, 0x12);
        uint32_t part = (dmem.read8(BASE + 20)) |
                        (dmem.read8(BASE + 21) << 8) |
                        (dmem.read8(BASE + 22) << 16) |
                        (dmem.read8(BASE + 23) << 24);
        REQUIRE(dmem.read32(BASE + 20) == part);

        // Unaligned
        dmem.write32(BASE + 21, 0x12345678);
        REQUIRE(dmem.read32(BASE + 21) == 0x12345678);
    }

    SECTION("Random sparse writes")
    {
        std::mt19937_64 rng(0xBEEFCAFE);
        std::uniform_int_distribution<uint32_t> addr_dist(BASE, BASE + SIZE - 1);
        std::uniform_int_distribution<uint8_t> byte_dist(0, 0xFF);

        // Write and read back 200 random bytes
        for (int i = 0; i < 200; ++i)
        {
            uint32_t addr = addr_dist(rng);
            uint8_t val = byte_dist(rng);
            dmem.write8(addr, val);
            REQUIRE(dmem.read8(addr) == val);
        }
    }

    SECTION("Out-of-range accesses throw")
    {
        REQUIRE_THROWS_AS(dmem.read8(BASE + SIZE), std::out_of_range);
        REQUIRE_THROWS_AS(dmem.write8(BASE - 1, 0x1), std::out_of_range);
        REQUIRE_THROWS_AS(dmem.read16(BASE + SIZE - 1), std::out_of_range);
        REQUIRE_THROWS_AS(dmem.write16(BASE + SIZE - 1, 0x1234), std::out_of_range);
        REQUIRE_THROWS_AS(dmem.read32(BASE + SIZE - 3), std::out_of_range);
        REQUIRE_THROWS_AS(dmem.write32(BASE + SIZE - 3, 0xDEADBEEF), std::out_of_range);
    }
}
