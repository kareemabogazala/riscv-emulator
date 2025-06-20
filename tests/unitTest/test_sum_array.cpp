#include "catch.hpp"
#include "RISCV.h"
#include "Stages/FetchStage.h"
#include "Stages/DecodeStage.h"
#include "Stages/ExecuteStage.h"
#include "Stages/MemoryStage.h"
#include "Stages/WritebackStage.h"
#include "Memory/DMEM.h"
#include "Memory/IMEM.h"
#include "Memory/MemoryBus.h"

TEST_CASE("Array sum program runs correctly", "[loop][memory][registers]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    cpu.load_program("../AssemblyCode/array_sum.bin", 0);

    SECTION("Registers and memory match expected values after array sum")
    {
        cpu.run(100); // enough to finish the loop

        // Check sum register
        REQUIRE(cpu.regs.read(5) == 100);        // x5 = sum = 10 + 20 + 30 + 40
        REQUIRE(cpu.regs.read(2) == 0x00100000); // x2 = base address

        // Check loop counter
        REQUIRE(cpu.regs.read(4) == 4); // x4 = i after loop ends

        // Check array memory values
        REQUIRE(dmem.read32(0x00100000) == 10);
        REQUIRE(dmem.read32(0x00100004) == 20);
        REQUIRE(dmem.read32(0x00100008) == 30);
        REQUIRE(dmem.read32(0x0010000C) == 40);
    }
}
