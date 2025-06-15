#include "RISCV.h"
#include "Stages/FetchStage.h"
#include "Stages/DecodeStage.h"
#include <iostream>

RISCV::RISCV(std::shared_ptr<MemoryBus> mem_bus) : bus(mem_bus)
{
    // Initialize the Fetch stage and add to stages vector
    stages.emplace_back(std::make_unique<FetchStage>());
    stages.emplace_back(std::make_unique<DecodeStage>());
}

void RISCV::step()
{
    // Execute all pipeline stages in order
    for (auto &stage : stages)
    {
        stage->tick(*this);
    }

    // Update PC at the end of the cycle
    update_pc();
}

void RISCV::run(uint32_t max_cycles)
{
    for (uint32_t cycle = 0; cycle < max_cycles; ++cycle)
    {
        std::cout << "[Cycle " << cycle << "] PC = 0x" << std::hex << pc << std::dec << "\n";
        step();
    }
}

void RISCV::update_pc()
{
    // Default: PC = PC + 4 (from fetch stage output)
    pc = if_id.pc_next;
}
