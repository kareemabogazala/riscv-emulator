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
    control_logic.controlSignals.Branch = false;
    update_pc();
}
uint32_t default_transform2(uint32_t val) { return val; }
int RISCV::run(int max_cycles)
{
    int cycle;
    
    for (cycle = 1; cycle <= max_cycles; ++cycle)
    {
       // std::cout << "[Cycle " << static_cast<int>(cycle) << "] PC = 0x" << std::hex << pc << std::dec << "\n";
       resetPipelineRegisters();
       step();
       if (DEBUG.dump_dmem_every_cycle)
       {
           bus->dmem.dump(0x00100000, 0x00100010, default_transform2);
       }
       if (pc >= end_address) break;
    }
    return cycle;
}

void RISCV::update_pc()
{

    if (control_logic.controlSignals.PCSel)
    {
        // Use jump target calculated by ALU in Execute stage
        pc = ex_mem.alu_result;
        control_logic.controlSignals.PCSel = false;
        control_logic.controlSignals.Branch = false;
    }
    else
    {
        // Default sequential execution
        pc = if_id.pc_next;
    }
   
    if (pc % 4 != 0)
        throw std::runtime_error("Unaligned PC detected: " + std::to_string(pc));
}

void RISCV::resetPipelineRegisters()
{
    // You can just reassign default-constructed structs
    if_id = {};
    id_ex = {};
    ex_mem = {};
    mem_wb = {};
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

void RISCV::load_program(const std::string &path, uint32_t load_address = 0x0)
{
    bus->imem.load_code_from_file(path, load_address);
    pc = load_address;
    end_address = load_address + bus->imem.end_address; // Or just path size if you track it
}
