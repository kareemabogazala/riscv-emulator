#include "catch.hpp"
#include "RISCV.h"
#include "Memory/IMEM.h"
#include "Memory/DMEM.h"
#include "Memory/MemoryBus.h"

TEST_CASE("U-Type instructions LUI and AUIPC", "[u-type]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    cpu.load_program("../AssemblyCode/u_type_test.bin", 0);

    // Execute 2 instructions
    cpu.run(6); // enough for LUI and AUIPC to reach writeback

    SECTION("LUI loads upper 20 bits")
    {
        REQUIRE(cpu.regs.read(5) == 0x12345000); // x5 = 0x12345 << 12
    }

    SECTION("AUIPC computes PC-relative address")
    {
        uint32_t expected_pc = 0x00000000 + 4;               // PC at AUIPC
        REQUIRE(cpu.regs.read(6) == (expected_pc + 0x1000)); // x6 = PC + 0x1000
    }
}
