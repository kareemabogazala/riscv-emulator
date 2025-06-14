#include "catch.hpp"
#include "Memory/memory.h"

TEST_CASE("Memory basic read/write", "[memory]")
{
    Memory mem(0x1000, 64, MEM_R | MEM_W);

    SECTION("write8 and read8")
    {
        mem.write8(0x1000, 0xAB);
        REQUIRE(mem.read8(0x1000) == 0xAB);
    }

    SECTION("write16 and read16")
    {
        mem.write16(0x1004, 0x1234);
        REQUIRE(mem.read16(0x1004) == 0x1234);
    }

    SECTION("write32 and read32")
    {
        mem.write32(0x1008, 0xCAFEBABE);
        REQUIRE(mem.read32(0x1008) == 0xCAFEBABE);
    }

    SECTION("out of range access throws")
    {
        REQUIRE_THROWS_AS(mem.read8(0x2000), std::runtime_error);
        REQUIRE_THROWS_AS(mem.write8(0x0FFF, 0x42), std::runtime_error);
    }
}
