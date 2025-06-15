#include <bitset>  // Required for binary formatting
#include <iomanip> // Required for std::setw and std::setfill

#include "Stages/FetchStage.h"
#include "DebugConfig.h"
#include "RISCV.h"





void FetchStage::tick(RISCV &cpu)
{
    // Fetch current instruction
    uint32_t instr = cpu.bus->read32(cpu.pc);
    
    // Store to pipeline register
    cpu.if_id.instr = instr;
    cpu.if_id.pc_next = cpu.pc + 4;

    // DO NOT update cpu.pc here. Let RISCV::step() or control logic do it later.
    // Debug: print instruction with address
    if (DEBUG.fetch)
    {
        std::cout << "[Fetch] PC: 0x" << std::hex << std::setw(8) << std::setfill('0') << cpu.pc
                  << " | Instr: 0x" << std::setw(8) << instr
                  << " | Binary: " << std::bitset<32>(instr) << std::dec
                  << std::endl;
    }
}
