#include "catch.hpp"
#include "regs.h"
#include "Memory/DMEM.h"

TEST_CASE("Regs + Memory interaction", "[integration]")
{
    Regs r;
    DMem mem(0x4000, 128);

    r.write(1, 0x12345678);
    mem.write32(0x4000, r.read(1));

    REQUIRE(mem.read32(0x4000) == 0x12345678);
}
