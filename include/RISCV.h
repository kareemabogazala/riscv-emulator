/// RISCV.h
#ifndef RISCV_H
#define RISCV_H

#include <cstdint>
#include <memory>
#include <vector>
#include "Stages/Stage.h"
#include "Regs.h"
#include "Memory/MemoryBus.h"
#include "Utiles/DecoderUtiles.h"
#include "Utiles/ALUOp.h"
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

            // All possible immediates (only one will be used)
            int32_t imm_i = 0;
            int32_t imm_s = 0;
            int32_t imm_b = 0;
            int32_t imm_u = 0;
            int32_t imm_j = 0;
    } id_ex;

    struct EXMEM
    { /* ALU results, branch info */
    } ex_mem;
    struct MEMWB
    { /* data to write back */
    } mem_wb;

    // Ordered pipeline stages
    std::vector<std::unique_ptr<Stage>> stages;

    /// Construct CPU with a shared MemoryBus
    RISCV(std::shared_ptr<MemoryBus> mem_bus);

    //Update the next PC
    void update_pc();
    /// Advance the pipeline by one cycle
    void step();

    /// Run the CPU for up to max_cycles
    void run(uint32_t max_cycles = 1000000);
};

#endif // RISCV_H
