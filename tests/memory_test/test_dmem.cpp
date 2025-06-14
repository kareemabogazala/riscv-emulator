#include "catch.hpp"
#include "Memory/DMEM.h"

TEST_CASE("DMem basic", "[dmem]")
{
    DMem dmem(0x2000, 128);

    SECTION("read/write should work")
    {
        dmem.write32(0x2000, 0x12345678);
        REQUIRE(dmem.read32(0x2000) == 0x12345678);
    }
}
