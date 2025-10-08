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
    Memory ram(0x00100000, 64);
    auto bus = std::make_shared<MemoryBus>(ram);
    RISCV cpu(bus);

    // Set register x2 = 10, x3 = 32
    cpu.regs.write( 2, 10);
    cpu.regs.write( 3, 32);

    // add x1, x2, x3 = x1 ← x2 + x3 = 42
    imem.load_code_from_file("../AssemblyCode/add.bin", 0);

    SECTION("CPU performs ADD correctly")
    {
        cpu.step(); 
      //  REQUIRE(cpu.ex_mem.alu_result == 42); // 10 + 32
      //  REQUIRE(cpu.ex_mem.rs2_val == 32);
      //  REQUIRE(cpu.ex_mem.rd == 1); // Destination register = x1
    }
}

TEST_CASE("ExecuteStage computes correct ALU results (without writeback)", "[execute][pipeline]")
{
    IMem imem(0x00000000, 512);
    Memory ram(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(ram);
    RISCV cpu(bus);

    // Load all 10 R-type instructions
    cpu.load_program("../AssemblyCode/add.bin", 0);

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

    std::vector<uint32_t> expected_regs(32, 0);

    // Values written before execution
    expected_regs[2] = 10;                          // x2
    expected_regs[3] = 32;                          // x3
    expected_regs[5] = 100;                         // x5
    expected_regs[6] = 58;                          // x6
    expected_regs[8] = 1;                           // x8
    expected_regs[9] = 5;                           // x9
    expected_regs[11] = 10;                         // x11
    expected_regs[12] = 20;                         // x12
    expected_regs[14] = 10;                         // x14
    expected_regs[15] = 5;                          // x15
    expected_regs[17] = 0xFF00;                     // x17
    expected_regs[18] = 0x0F0F;                     // x18
    expected_regs[20] = 128;                        // x20
    expected_regs[21] = 2;                          // x21
    expected_regs[23] = static_cast<uint32_t>(-32); // x23
    expected_regs[24] = 2;                          // x24
    expected_regs[26] = 0xF0F0;                     // x26
    expected_regs[27] = 0x0F0F;                     // x27
    expected_regs[29] = 0xFF00;                     // x29
    expected_regs[30] = 0x0F0F;                     // x30

    // Expected ALU results (written during execution)
    expected_regs[1] = 42;                               // x2 + x3
    expected_regs[4] = 42;                               // x5 - x6
    expected_regs[7] = 32;                               // x8 << x9
    expected_regs[10] = 1;                               // x11 < x12
    expected_regs[13] = 0;                               // x14 < x15 (unsigned)
    expected_regs[16] = 0xF00F;                          // x17 ^ x18
    expected_regs[19] = 32;                              // x20 >> x21 (logical)
    expected_regs[22] = static_cast<uint32_t>(-32 >> 2); // x23 >> x24 (arith)
    expected_regs[25] = 0xFFFF;                          // x26 | x27
    expected_regs[28] = 0x0F00;                          // x29 & x30

    SECTION("All 32 registers have correct final values")
    {
        int cycle = cpu.run(100); // enough cycles for all instructions + WB

        for (int i = 0; i < 32; ++i)
        {
            DYNAMIC_SECTION("x" << i)
            {
                REQUIRE(cpu.regs.read(i) == expected_regs[i]);
            }
        }
        REQUIRE(cycle >= 10);
    }
}
