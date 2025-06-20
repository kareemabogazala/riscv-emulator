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

TEST_CASE("All major instruction types (I, S, L, R, JAL) are executed correctly", "[fulltest]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    cpu.load_program("../AssemblyCode/jslir_test.bin", 0);

    SECTION("Registers and memory reflect correct execution")
    {
        cpu.run(20); // enough cycles

        // Check registers from I-Type
        REQUIRE(cpu.regs.read(1) == 42);
        REQUIRE(cpu.regs.read(2) == 100);

        // Check memory via L-Type
        REQUIRE(cpu.regs.read(4) == 42);
        REQUIRE(cpu.regs.read(5) == 100);

        // Check R-Type results
        REQUIRE(cpu.regs.read(6) == 142);
        REQUIRE(cpu.regs.read(7) == 58);

        // Check JAL skipped two instructions
        REQUIRE(cpu.regs.read(8) == 0);
        REQUIRE(cpu.regs.read(9) == 0);

        // Final instruction after JAL
        REQUIRE(cpu.regs.read(10) == 1);

        // Optionally: check memory
        REQUIRE(dmem.read32(0x00100000) == 42);
        REQUIRE(dmem.read32(0x00100004) == 100);
    }
}
