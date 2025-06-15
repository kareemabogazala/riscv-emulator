#include "RISCV.h"
#include "Memory/IMEM.h"
#include "Memory/DMEM.h"
#include "Memory/MemoryBus.h"
#include "Stages/FetchStage.h"
#include "DebugConfig.h"
#include <iostream>
#include <memory>

int main()
{
    // Create IMEM with a few instructions (each 32-bit)
    DEBUG.decode = true;
    DEBUG.fetch = true;
    IMem imem(0x00000000, 64); // 64 bytes of instruction memory
    DMem dmem(0x00100000, 64); // 64 bytes of data memory

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
    // Create MemoryBus and CPU
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

    // Only add the fetch stage for now
   // cpu.stages.emplace_back(std::make_unique<FetchStage>());

    // Run CPU for 3 cycles to fetch all 3 instructions
    cpu.run(10);

    return 0;
}
