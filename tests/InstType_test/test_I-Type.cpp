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

TEST_CASE("All I-Type instructions are decoded and executed correctly", "[itype][pipeline]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);


   cpu.load_program("../AssemblyCode/i_type_test.bin", 0);

    SECTION("Registers contain correct results after I-type test program")
    {
        cpu.run(15); // enough cycles for 11 instructions

        REQUIRE(cpu.regs.read(1) == 42);   // addi
        REQUIRE(cpu.regs.read(2) == 32);   // addi
        REQUIRE(cpu.regs.read(3) == 0x20); // andi
        REQUIRE(cpu.regs.read(4) == 0x2F); // ori
        REQUIRE(cpu.regs.read(5) == 0xDF); // xori
        REQUIRE(cpu.regs.read(6) == 1);    // slti
        REQUIRE(cpu.regs.read(7) == 0);    // sltiu
        REQUIRE(cpu.regs.read(8) == 168);  // slli
        REQUIRE(cpu.regs.read(9) == 21);   // srli
        REQUIRE(cpu.regs.read(10) == 21);  // srai
        REQUIRE(cpu.regs.read(11) == 0);   // jal skipped this
    }
}
