#include "catch.hpp"
#include "Memory/MemoryBus.h"
#include <random>
#include <cstdint>
#include <vector>

static constexpr uint32_t IMEM_BASE = 0x0000'0000;
static constexpr uint32_t IMEM_SIZE = 1 << 16; // 64 KiB
static constexpr uint32_t DMEM_BASE = 0x0010'0000;
static constexpr uint32_t DMEM_SIZE = 1 << 20; // 1 MiB

TEST_CASE("MemoryBus: stress-test large memory regions", "[memorybus][stress]")
{
    // Create 1 MiB data RAM and 64 KiB code ROM
    DMem dmem(DMEM_BASE, DMEM_SIZE);
    IMem imem(IMEM_BASE, IMEM_SIZE);
    MemoryBus bus(dmem, imem); // pass by reference now

    // RNG for addresses and values
    std::mt19937_64 rng(0xCAFED00D);
    std::uniform_int_distribution<uint32_t> imem_dist(IMEM_BASE, IMEM_BASE + IMEM_SIZE - 1);
    std::uniform_int_distribution<uint32_t> dmem_dist(DMEM_BASE, DMEM_BASE + DMEM_SIZE - 1);
    std::uniform_int_distribution<uint8_t> byte_dist(0, 0xFF);
    std::uniform_int_distribution<uint16_t> half_dist(0, 0xFFFF);
    std::uniform_int_distribution<uint32_t> word_dist(0, 0xFFFFFFFF);

    // 1) Zero-init sampling in both regions
    for (int i = 0; i < 1000; ++i)
    {
        REQUIRE(bus.read8(imem_dist(rng)) == 0);
        REQUIRE(bus.read8(dmem_dist(rng)) == 0);
    }

    // 2) Random writes to data memory (8/16/32-bit) and immediate verification
    for (int i = 0; i < 10000; ++i)
    {
        uint32_t addr8 = dmem_dist(rng);
        uint8_t v8 = byte_dist(rng);
        bus.write8(addr8, v8);
        REQUIRE(bus.read8(addr8) == v8);

        if (addr8 + 1 < DMEM_BASE + DMEM_SIZE)
        {
            uint16_t v16 = half_dist(rng);
            bus.write16(addr8, v16);
            REQUIRE(bus.read16(addr8) == v16);
        }
        if (addr8 + 3 < DMEM_BASE + DMEM_SIZE)
        {
            uint32_t v32 = word_dist(rng);
            bus.write32(addr8, v32);
            REQUIRE(bus.read32(addr8) == v32);
        }
    }

    // 3) Ensure writes into IMem always throw
    for (int i = 0; i < 1000; ++i)
    {
        uint32_t addr = imem_dist(rng);
        REQUIRE_THROWS_AS(bus.write8(addr, 1), std::out_of_range);
        REQUIRE_THROWS_AS(bus.write16(addr, 1), std::out_of_range);
        REQUIRE_THROWS_AS(bus.write32(addr, 1), std::out_of_range);
    }

    // 4) Out-of-range accesses should always throw
    REQUIRE_THROWS_AS(bus.read8(IMEM_BASE - 1), std::out_of_range);
    REQUIRE_THROWS_AS(bus.read16(IMEM_BASE + IMEM_SIZE - 1), std::out_of_range);
    REQUIRE_THROWS_AS(bus.read32(IMEM_BASE + IMEM_SIZE - 3), std::out_of_range);
    REQUIRE_THROWS_AS(bus.write8(DMEM_BASE + DMEM_SIZE, 1), std::out_of_range);
    REQUIRE_THROWS_AS(bus.write16(DMEM_BASE + DMEM_SIZE - 1, 1), std::out_of_range);
    REQUIRE_THROWS_AS(bus.write32(DMEM_BASE + DMEM_SIZE - 3, 1), std::out_of_range);
}
