#include "catch.hpp"
#include "RISCV.h"
#include "Stages/FetchStage.h"
#include "Stages/DecodeStage.h"
#include "Stages/ExecuteStage.h"
#include "Memory/DMEM.h"
#include "Memory/IMEM.h"
#include "Memory/MemoryBus.h"

TEST_CASE("ExecuteStage performs correct ALU operations", "[execute]")
{
    IMem imem(0x00000000, 64);
    DMem dmem(0x00100000, 64);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    // Set register x2 = 10, x3 = 32
    cpu.regs.write( 2, 10);
    cpu.regs.write( 3, 32);

    // add x1, x2, x3 = x1 ← x2 + x3 = 42
    imem.load_code_from_file("../AssemblyCode/add.bin", 0);

    SECTION("CPU performs ADD correctly")
    {
        cpu.step(); 
        REQUIRE(cpu.ex_mem.alu_result == 42); // 10 + 32
        REQUIRE(cpu.ex_mem.rs2_val == 32);
        REQUIRE(cpu.ex_mem.rd == 1); // Destination register = x1
    }
}

TEST_CASE("ExecuteStage computes correct ALU results (without writeback)", "[execute][pipeline]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    // Load all 10 R-type instructions
    imem.load_code_from_file("../AssemblyCode/add.bin", 0);

    // Register setup based on expected inputs
    cpu.regs.write(2, 10);                          // x2
    cpu.regs.write(3, 32);                          // x3
    cpu.regs.write(5, 100);                         // x5
    cpu.regs.write(6, 58);                          // x6
    cpu.regs.write(8, 1);                           // x8
    cpu.regs.write(9, 5);                           // x9
    cpu.regs.write(11, 10);                         // x11
    cpu.regs.write(12, 20);                         // x12
    cpu.regs.write(14, 10);                         // x14
    cpu.regs.write(15, 5);                          // x15
    cpu.regs.write(17, 0xFF00);                     // x17
    cpu.regs.write(18, 0x0F0F);                     // x18
    cpu.regs.write(20, 128);                        // x20
    cpu.regs.write(21, 2);                          // x21
    cpu.regs.write(23, static_cast<uint32_t>(-32)); // x23
    cpu.regs.write(24, 2);                          // x24
    cpu.regs.write(26, 0xF0F0);                     // x26
    cpu.regs.write(27, 0x0F0F);                     // x27
    cpu.regs.write(29, 0xFF00);                     // x29
    cpu.regs.write(30, 0x0F0F);                     // x30

    struct Expected
    {
        uint8_t rd;
        uint32_t result;
    };

    std::vector<Expected> expectations = {
        {1, 42},                               // x2 + x3
        {4, 42},                               // x5 - x6
        {7, 32},                               // x8 << x9
        {10, 1},                               // x11 < x12
        {13, 0},                               // x14 < x15 (unsigned)
        {16, 0xF00F},                          // x17 ^ x18
        {19, 32},                              // x20 >> x21 (logical)
        {22, static_cast<uint32_t>(-32 >> 2)}, // x23 >> x24 (arith)
        {25, 0xFFFF},                          // x26 | x27
        {28, 0x0F00},                          // x29 & x30
    };

    SECTION("Each instruction executes with correct ALU output and rd")
    {
        for (size_t i = 0; i < expectations.size(); ++i)
        {
            cpu.run(1);

            DYNAMIC_SECTION("Instruction " << i)
            {
                REQUIRE(cpu.ex_mem.alu_result == expectations[i].result);
                REQUIRE(cpu.ex_mem.rd == expectations[i].rd);
            }
        }
    }
}
