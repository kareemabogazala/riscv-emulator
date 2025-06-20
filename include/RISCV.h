/// RISCV.h
#ifndef RISCV_H
#define RISCV_H

#include <cstdint>
#include <memory>
#include <vector>
#include "Stages/Stage.h"
#include "Regs.h"
#include "Memory/MemoryBus.h"
#include "Utils/DecoderUtils.h"
#include "Utils/ALUOp.h"
#include "ControlLogic.h"
class Stage; // 🔁 Add this forward declaration

/// Represents the full RISC-V CPU, holding state and pipeline.
class RISCV
{
public:
    // Core state
    Regs regs;                      // Register file
    std::shared_ptr<MemoryBus> bus; // Unified memory interface
    uint32_t pc = 0;                // Program counter
    uint32_t end_address;
    ControlLogic control_logic;
    // Pipeline registers
    struct IFID
    {
        uint32_t instr;
        uint32_t pc_next;
    } if_id;
    struct IDEX
    {
            // Instruction fields (used for control or ALU decisions)
            uint8_t rd = 0;     // instr[11:7]
            uint8_t rs1 = 0;    // instr[19:15]
            uint8_t rs2 = 0;    // instr[24:20]
            uint8_t funct3 = 0; // instr[14:12]
            uint8_t funct7 = 0; // instr[31:25]
            uint8_t opcode = 0; // instr[6:0]

            // Register values read from the register file
            uint32_t rs1_val = 0;
            uint32_t rs2_val = 0;

            // immediates 
            int32_t imm = 0;

            //fot the J/ JU
            bool is_jump = false;

            // conditional branches
            bool is_branch = false;
    } id_ex;

    struct EXMEM
    {
        uint32_t alu_result = 0;
        uint32_t rs2_val = 0; // For store instructions
        uint8_t rd = 0;

    } ex_mem;
    struct MEMWB
    {
        uint32_t alu_result = 0;
        uint32_t mem_data = 0;
        uint8_t rd = 0;
        bool reg_write = false;
        WBSel wb_sel = WBSel::WB_ALU;
    } mem_wb;

    // Ordered pipeline stages
    std::vector<std::unique_ptr<Stage>> stages;

    /// Construct CPU with a shared MemoryBus
    RISCV(std::shared_ptr<MemoryBus> mem_bus);

    //Update the next PC
    void update_pc();
    //load file
    void load_program(const std::string &path, uint32_t load_address);

    /// Advance the pipeline by one cycle
    void step();

    /// Run the CPU for up to max_cycles
    int run(int max_cycles = 1000000);

    // restart pipeline
    /// Clear all pipeline registers (used after each cycle to avoid stale data)
    void resetPipelineRegisters();

    // Debugging
    /// Dump IDEX contents for debugging
    void dumpIDEX() const;
   
};

#endif // RISCV_H
