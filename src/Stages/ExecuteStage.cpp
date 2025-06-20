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

        // Branch decision need a lot of changes !!!!!!!!!!!bool taken = false;

        bool taken = false;
        if (id_ex.is_branch)
        {
            taken = BranchComp::compare(rs1_val, rs2_val, id_ex.funct3, ctrl.BrUn);
        }
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
    const auto &ex_mem = cpu.ex_mem;
    const auto &id_ex = cpu.id_ex;
    const auto &ctrl = cpu.control_logic.controlSignals;
    if (DEBUG.execute)
    {
        std::cout << "\n[Execute Stage Dump]"
                  << "\n  ALU Result    = 0x" << std::hex << ex_mem.alu_result
                  << " (" << std::dec << ex_mem.alu_result << ")"
                  << "\n  rs2_val       = 0x" << std::hex << ex_mem.rs2_val
                  << " (" << std::dec << ex_mem.rs2_val << ")"
                  << "\n  rd            = x" << std::dec << static_cast<int>(ex_mem.rd)
                  << "\n  is_branch     = " << std::boolalpha << id_ex.is_branch
                  << "\n  funct3        = 0x" << std::hex << static_cast<int>(id_ex.funct3)
                  << "\n  imm           = 0x" << std::hex << id_ex.imm
                  << " (" << std::dec << static_cast<int32_t>(id_ex.imm) << ")"
                  << "\n  BranchCmp     = " << BranchComp::compare(id_ex.rs1_val, id_ex.rs2_val, id_ex.funct3, ctrl.BrUn)
                  << "\n  PC (current)  = 0x" << std::hex << cpu.pc
                  << "\n  Expected PC   = 0x" << std::hex
                  << (ctrl.ASel == ALU_A::PC ? cpu.if_id.pc_next : id_ex.rs1_val)
                  << " + 0x" << id_ex.imm
                  << " = 0x" << ALU::execute(ctrl.ALUSel, (ctrl.ASel == ALU_A::PC ? cpu.if_id.pc_next : id_ex.rs1_val), id_ex.imm)
                  << "\n";
    }
    }
