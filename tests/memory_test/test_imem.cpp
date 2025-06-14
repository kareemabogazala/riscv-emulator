#include "catch.hpp"
#include "Memory/IMEM.h"

TEST_CASE("IMem read only", "[imem]")
{
    IMem imem(0x3000, 64);

    SECTION("writing should throw")
    {
        REQUIRE_THROWS_AS(imem.write8(0x3000, 0xAB), std::runtime_error);
    }
}
