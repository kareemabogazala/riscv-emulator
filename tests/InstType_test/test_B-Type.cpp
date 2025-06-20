#include "catch.hpp"
#include "RISCV.h"
#include "Memory/IMEM.h"
#include "Memory/DMEM.h"
#include "Memory/MemoryBus.h"

TEST_CASE("Branch instructions execute correctly", "[branch][control_flow]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    cpu.load_program("../AssemblyCode/branch_test.bin", 0);

    SECTION("Branch results and bypass fail paths")
    {
        cpu.run(100); // enough cycles to execute all branches

        // Check that only PASS registers were written (i.e., we skipped FAIL paths)
        REQUIRE(cpu.regs.read(10) == 0); // x10 untouched (no FAIL taken)

        REQUIRE(cpu.regs.read(11) == 11); // BEQ passed
        REQUIRE(cpu.regs.read(12) == 12); // BNE passed
        REQUIRE(cpu.regs.read(13) == 13); // BLT passed
        REQUIRE(cpu.regs.read(14) == 14); // BGE passed
        REQUIRE(cpu.regs.read(15) == 15); // BLTU passed
        REQUIRE(cpu.regs.read(16) == 16); // BGEU passed
    }
}
