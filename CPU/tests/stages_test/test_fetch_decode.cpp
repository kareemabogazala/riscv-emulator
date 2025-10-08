#include "catch.hpp"
#include "RISCV.h"
#include "Stages/FetchStage.h"
#include "Stages/DecodeStage.h"
#include "Memory/DMEM.h"
#include "Memory/IMEM.h"
#include "Memory/MemoryBus.h"

    TEST_CASE("R-Type instructions fetch and decode correctly", "[fetch][decode][rtype]")
{
    IMem imem(0x00000000, 64);
    Memory ram(0x00100000, 64);
    auto bus = std::make_shared<MemoryBus>(ram);
    RISCV cpu(bus);

    // Load R-type binary code from file (assumes little-endian encoding)
    imem.load_code_from_file("../AssemblyCode/add.bin",0);

    struct Expected
    {
        uint8_t rd, rs1, rs2, funct3, funct7;
    };

    std::vector<Expected> expectations = {
        {1, 2, 3, 0x0, 0x00},    // add
        {4, 5, 6, 0x0, 0x20},    // sub
        {7, 8, 9, 0x1, 0x00},    // sll
        {10, 11, 12, 0x2, 0x00}, // slt
        {13, 14, 15, 0x3, 0x00}, // sltu
        {16, 17, 18, 0x4, 0x00}, // xor
        {19, 20, 21, 0x5, 0x00}, // srl
        {22, 23, 24, 0x5, 0x20}, // sra
        {25, 26, 27, 0x6, 0x00}, // or
        {28, 29, 30, 0x7, 0x00}  // and
    };

    for (size_t i = 0; i < expectations.size(); ++i)
    {
        cpu.step();
        DYNAMIC_SECTION("Instruction " << i)
        {
            
            REQUIRE(cpu.id_ex.opcode == 0x33); // All are R-type
            REQUIRE(cpu.id_ex.rd == expectations[i].rd);
            REQUIRE(cpu.id_ex.rs1 == expectations[i].rs1);
            REQUIRE(cpu.id_ex.rs2 == expectations[i].rs2);
            REQUIRE(cpu.id_ex.funct3 == expectations[i].funct3);
            REQUIRE(cpu.id_ex.funct7 == expectations[i].funct7);
        }
    }
}
