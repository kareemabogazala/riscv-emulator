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

TEST_CASE("ALU instructions produce correct results in register file (writeback verified)", "[execute][writeback]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    // Add full pipeline
    cpu.stages.push_back(std::make_unique<FetchStage>());
    cpu.stages.push_back(std::make_unique<DecodeStage>());
    cpu.stages.push_back(std::make_unique<ExecuteStage>());
    cpu.stages.push_back(std::make_unique<MemoryStage>());
    cpu.stages.push_back(std::make_unique<WritebackStage>());

    // Set inputs for all 10 instructions
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

    // Load 10-instruction R-type test program
    imem.load_code_from_file("../AssemblyCode/add.bin", 0);

    // Expected writeback values
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

    SECTION("Registers contain correct results after full pipeline")
    {
        cpu.run(10); // Run enough cycles for 10 instructions to complete WB

        for (const auto &exp : expectations)
        {
            DYNAMIC_SECTION("Checking x" << static_cast<int>(exp.rd))
            {
                REQUIRE(cpu.regs.read(exp.rd) == exp.result);
            }
        }
    }
}


// Helper: compare all 32 registers at once
static void check_regs(const Regs &regs, const std::array<uint32_t, 32> &exp)
{
    for (int i = 0; i < 32; ++i)
    {
        DYNAMIC_SECTION("x" << i)
        {
            REQUIRE(regs.read(i) == exp[i]);
        }
    }
}

TEST_CASE("Chain of R-type adds updates registers at 3 and 9 instructions", "[rtype][chain]")
{
    IMem imem(0x00000000, 512);
    DMem dmem(0x00100000, 512);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    // 2) set up x2 (the only input register)
    cpu.regs.write(2, 5); // every add is k*x2; here k=2..10

    // 3) load our 9-instruction chain
    imem.load_code_from_file("../AssemblyCode/chain.bin", 0);

    // Expected after 3 instructions (x3,x4,x5)
    std::array<uint32_t, 32> exp_after3 = {};
    exp_after3[2] = 5;     // x2 untouched
    exp_after3[3] = 2 * 5; // x3
    exp_after3[4] = 3 * 5; // x4
    exp_after3[5] = 4 * 5; // x5
    // all other xN == 0

    cpu.run(3);
    check_regs(cpu.regs, exp_after3);

    // Expected after all 9 instructions (x3..x11)
    std::array<uint32_t, 32> exp_after9 = {};
    exp_after9[2] = 5;
    exp_after9[3] = 2 * 5;
    exp_after9[4] = 3 * 5;
    exp_after9[5] = 4 * 5;
    exp_after9[6] = 5 * 5;
    exp_after9[7] = 6 * 5;
    exp_after9[8] = 7 * 5;
    exp_after9[9] = 8 * 5;
    exp_after9[10] = 9 * 5;
    exp_after9[11] = 10 * 5;

    cpu.run(6); // total 3+6 = 9 instructions
    check_regs(cpu.regs, exp_after9);
}
