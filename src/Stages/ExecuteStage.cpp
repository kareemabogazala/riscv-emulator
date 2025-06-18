#include "Stages/ExecuteStage.h"
#include "Core/ALU.h"
#include "Core/BranchComp.h"
#include "DebugConfig.h"

#include <iostream>

void ExecuteStage::tick(RISCV & cpu)
    {
        const auto &ctrl = cpu.control_logic.controlSignals;
        const auto &id_ex = cpu.id_ex;
        auto &ex_mem = cpu.ex_mem;

        uint32_t rs1_val = id_ex.rs1_val;
        uint32_t rs2_val = id_ex.rs2_val;
        if(DEBUG.execute)
        {
            std::cout << "[RS value] rs1_val =" << rs1_val
                      << " rs2_val =" << rs2_val
                      << std::endl;
        }
        uint32_t alu_val0 = (ctrl.ASel == ALU_A::RS1) ? rs1_val : cpu.pc;
        uint32_t alu_val1 = (ctrl.BSel == ALU_B::RS2) ? rs2_val : id_ex.imm;

        uint32_t alu_result = ALU::execute(ctrl.ALUSel, alu_val0, alu_val1);

        // Store results
        cpu.ex_mem.alu_result = alu_result;
        ex_mem.rs2_val = rs2_val;
        ex_mem.rd = id_ex.rd;

        // Branch decision need a lot of changes !!!!!!!!!!!
        bool taken = BranchComp::compare(rs1_val, rs2_val);
        if (id_ex.is_jump || taken)
        {
            cpu.control_logic.controlSignals.PCSel = true;
        }
        if(DEBUG.execute){
            dump_EXMEM(cpu);
        }
}

void ExecuteStage::dump_EXMEM(RISCV &cpu) const
{

    std::cout << "[Execute] alu_result =" << cpu.ex_mem.alu_result
              << " rs2_val =" << cpu.ex_mem.rs2_val
              << " rd =" << cpu.ex_mem.rd
              << std::endl;
}