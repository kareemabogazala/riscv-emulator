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
    DMem dmem(0x00100000, 64);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    // Load R-type binary code (Little Endian layout from assembler)
    imem.load_code({
        0xB3, 0x00, 0x31, 0x00, // add  x1,  x2,  x3
        0xB3, 0x13, 0x94, 0x00, // sub  x4,  x5,  x6
        0xB3, 0x36, 0xF7, 0x00, // sll  x7,  x8,  x9
        0x33, 0x82, 0x62, 0x40, // slt  x10, x11, x12
        0xB3, 0x59, 0x5A, 0x01, // sltu x13, x14, x15
        0x33, 0xA5, 0xC5, 0x00, // xor  x16, x17, x18
        0x33, 0xC8, 0x28, 0x01, // srl  x19, x20, x21
        0x33, 0xDB, 0x8B, 0x41, // sra  x22, x23, x24
        0xB3, 0x6C, 0xBD, 0x01, // or   x25, x26, x27
        0x33, 0xFE, 0xEE, 0x01  // and  x28, x29, x30
    });

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
        DYNAMIC_SECTION("Instruction " << i)
        {
            cpu.step();

            REQUIRE(cpu.id_ex.opcode == 0x33); // All are R-type
            REQUIRE(cpu.id_ex.rd == expectations[i].rd);
            REQUIRE(cpu.id_ex.rs1 == expectations[i].rs1);
            REQUIRE(cpu.id_ex.rs2 == expectations[i].rs2);
            REQUIRE(cpu.id_ex.funct3 == expectations[i].funct3);
            REQUIRE(cpu.id_ex.funct7 == expectations[i].funct7);
        }
    }
}
