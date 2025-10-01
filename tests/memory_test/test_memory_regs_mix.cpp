#include "catch.hpp"
#include "REGS/regs.h"
#include "Memory/DMEM.h"

static constexpr uint32_t BASE = 0x5000;
static constexpr uint32_t SIZE = 256;
static constexpr uint32_t ADDR1 = BASE + 16;
static constexpr uint32_t ADDR2 = BASE + 32;
static constexpr uint32_t ADDR3 = BASE + 48;
static constexpr uint32_t ADDR4 = BASE + 64;
static constexpr uint32_t ADDR5 = BASE + 80;

TEST_CASE("Integration: Regs ↔ DMem full data path", "[integration][regs][dmem]")
{
    Regs regs;
    DMem mem(BASE, SIZE);

    // 1) All registers start at zero
    for (int i = 0; i < 32; ++i)
        REQUIRE(regs.read(i) == 0);

    // 2) 32-bit value round‐trip
    regs.write(5, 0x1111'2222);
    mem.write32(ADDR1, regs.read(5));
    regs.write(6, mem.read32(ADDR1));
    REQUIRE(regs.read(6) == 0x11112222);

    // 3) 16-bit halfword round‐trip (mixed endianness check)
    regs.write(7, 0xABCD1234);
    uint16_t half = static_cast<uint16_t>(regs.read(7) & 0xFFFF);
    mem.write16(ADDR2, half);
    regs.write(8, mem.read16(ADDR2));
    REQUIRE(regs.read(8) == half);

    // 4) 8-bit byte round‐trip
    regs.write(9, 0xDEAD'00FF);
    uint8_t byte = static_cast<uint8_t>(regs.read(9) & 0xFF);
    mem.write8(ADDR3, byte);
    regs.write(10, mem.read8(ADDR3));
    REQUIRE(regs.read(10) == byte);

    // 5) x0 always reads zero, even through memory
    regs.write(0, 0xDEAD'BEEF);
    REQUIRE(regs.read(0) == 0);
    mem.write32(ADDR4, regs.read(0)); // writes 0
    regs.write(11, mem.read32(ADDR4));
    REQUIRE(regs.read(11) == 0);

    // 6) Overflow beyond 32 bits wraps in both regs and mem
    uint64_t big = (uint64_t(1) << 33) | 0xCAFEBABE;
    regs.write(12, static_cast<uint32_t>(big)); // wraps to lower 32
    mem.write32(ADDR5, regs.read(12));
    regs.write(13, mem.read32(ADDR5));
    REQUIRE(regs.read(12) == regs.read(13));
}
