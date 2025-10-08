#include "catch.hpp"
#include "Memory/memory.h"
#include <random>

static constexpr uint32_t BASE = 0x0000'0000;
static constexpr size_t LARGE_SZ = 1 << 24; // 16 MiB
static constexpr int SAMPLES = 1000;

TEST_CASE("Memory very large allocation and sparse sampling", "[memory][large]")
{
    // Allocate 16 MiB, R&W
    Memory mem(BASE, LARGE_SZ, MEM_R | MEM_W);

    // 1) Entire region should be zero‐initialized (test a few points)
    std::vector<uint32_t> check_addrs = {
        BASE,
        BASE + LARGE_SZ / 4,
        BASE + LARGE_SZ / 2,
        BASE + LARGE_SZ - 1};
    for (auto addr : check_addrs)
    {
        REQUIRE(mem.read8(addr) == 0);
    }

    // 2) Random writes + reads
    std::mt19937_64 rng(0xDEADBEEF);
    std::uniform_int_distribution<uint32_t> dist8(0, 255);
    std::uniform_int_distribution<uint32_t> dist_addr(0, LARGE_SZ - 1);

    for (int i = 0; i < SAMPLES; ++i)
    {
        uint32_t offset = dist_addr(rng);
        uint8_t v8 = static_cast<uint8_t>(dist8(rng));
        mem.write8(BASE + offset, v8);
        REQUIRE(mem.read8(BASE + offset) == v8);
    }

    // 3) Test some 16‐bit sparse writes (aligned and unaligned)
    std::uniform_int_distribution<uint32_t> dist_addr16(0, LARGE_SZ - 2);
    for (int i = 0; i < SAMPLES / 10; ++i)
    {
        uint32_t off = dist_addr16(rng);
        uint16_t v16 = static_cast<uint16_t>(dist8(rng) << 8 | dist8(rng));
        mem.write16(BASE + off, v16);
        REQUIRE(mem.read16(BASE + off) == v16);
    }

    // 4) Test some 32‐bit sparse writes (aligned and unaligned)
    std::uniform_int_distribution<uint32_t> dist_addr32(0, LARGE_SZ - 4);
    for (int i = 0; i < SAMPLES / 20; ++i)
    {
        uint32_t off = dist_addr32(rng);
        uint32_t b0 = dist8(rng);
        uint32_t b1 = dist8(rng);
        uint32_t b2 = dist8(rng);
        uint32_t b3 = dist8(rng);
        uint32_t v32 = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
        mem.write32(BASE + off, v32);
        REQUIRE(mem.read32(BASE + off) == v32);
    }

    // 5) Verify out‐of‐range still throws
    REQUIRE_THROWS_AS(mem.read8(BASE + LARGE_SZ), std::out_of_range);
    REQUIRE_THROWS_AS(mem.write8(BASE + LARGE_SZ, 0), std::out_of_range);
    REQUIRE_THROWS_AS(mem.read16(BASE + LARGE_SZ - 1), std::out_of_range);
    REQUIRE_THROWS_AS(mem.write16(BASE + LARGE_SZ - 1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(mem.read32(BASE + LARGE_SZ - 2), std::out_of_range);
    REQUIRE_THROWS_AS(mem.write32(BASE + LARGE_SZ - 2, 0), std::out_of_range);
}
