#include "RISCV.h"
#include "Stages/FetchStage.h"
#include "Stages/DecodeStage.h"
#include "Stages/ExecuteStage.h"
#include "Stages/MemoryStage.h"
#include "Stages/WritebackStage.h"
#include "DebugConfig.h"
#include <iostream>

RISCV::RISCV(std::shared_ptr<MemoryBus> mem_bus) : bus(mem_bus)
{
    // Initialize the Fetch stage and add to stages vector
    stages.emplace_back(std::make_unique<FetchStage>());
    stages.emplace_back(std::make_unique<DecodeStage>());
    stages.emplace_back(std::make_unique<ExecuteStage>());
    stages.emplace_back(std::make_unique<MemoryStage>());
    stages.emplace_back(std::make_unique<WritebackStage>());
}

void RISCV::step()
{
    // Execute all pipeline stages in order
    for (auto &stage : stages)
    {
        stage->tick(*this);
    }
    if(DEBUG.RISCV_IDEX) dumpIDEX();
    // Update PC at the end of the cycle
    update_pc();
}

void RISCV::run(uint32_t max_cycles)
{
    for (uint32_t cycle = 0; cycle < max_cycles; ++cycle)
    {
      //  std::cout << "[Cycle " << cycle << "] PC = 0x" << std::hex << pc << std::dec << "\n";
        step();
    }
}

void RISCV::update_pc()
{
    // Default: PC = PC + 4 (from fetch stage output)
    pc = if_id.pc_next;
}




void RISCV::dumpIDEX() const{
    {
        std::cout << "[IDEX Dump]\n";
        std::cout << "  opcode: 0x" << std::hex << static_cast<int>(id_ex.opcode)
                  << " funct3: 0x" << static_cast<int>(id_ex.funct3)
                  << " funct7: 0x" << static_cast<int>(id_ex.funct7) << std::dec << "\n";
        std::cout << "  rd: x" << static_cast<int>(id_ex.rd)
                  << " rs1: x" << static_cast<int>(id_ex.rs1)
                  << " rs2: x" << static_cast<int>(id_ex.rs2) << "\n";
        std::cout << "  rs1_val: 0x" << std::hex << id_ex.rs1_val
                  << " rs2_val: 0x" << id_ex.rs2_val << std::dec << "\n";
        std::cout << "  imm: " << id_ex.imm<< "\n";
    }
}