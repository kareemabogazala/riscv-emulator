#include "Stages/ExecuteStage.h"
#include "Core/ALU.h"
#include "Core/BranchComp.h"
#include "DebugConfig.h"
#include "RISCV.h"

#include <iostream>

void ExecuteStage::tick(RISCV &cpu)
{
    const auto &ctrl = cpu.control_logic.controlSignals;
    const auto &id_ex = cpu.id_ex;
    auto &ex_mem = cpu.ex_mem;

    uint32_t rs1_val = id_ex.rs1_val;
    uint32_t rs2_val = id_ex.rs2_val;

    if (DEBUG.execute)
    {
        std::cout << "[Execute Stage] rs1=" << std::hex << rs1_val
                  << " rs2=" << rs2_val << std::dec << "\n";
    }

    // --- 1. SYSTEM instructions (ECALL, MRET, EBREAK) ---
    if (id_ex.is_system)
    {
        handle_system_instruction(cpu);
        return;
    }

    // --- 2. CSR instructions ---
    if (id_ex.is_csr)
    {
        handle_csr_instruction(cpu);
        return;
    }

    // --- 3. Regular ALU instructions ---
    uint32_t alu_val0 = (ctrl.ASel == ALU_A::RS1) ? rs1_val : cpu.pc;
    uint32_t alu_val1 = (ctrl.BSel == ALU_B::RS2) ? rs2_val : id_ex.imm;

    uint32_t alu_result = ALU::execute(ctrl.ALUSel, alu_val0, alu_val1);

    ex_mem.alu_result = alu_result;
    ex_mem.rs2_val = rs2_val;
    ex_mem.rd = id_ex.rd;

    // --- 4. Branching ---
    bool taken = false;
    if (id_ex.is_branch)
    {
        taken = BranchComp::compare(rs1_val, rs2_val, id_ex.funct3, ctrl.BrUn);
    }
    if (id_ex.is_jump || taken)
    {
        cpu.control_logic.controlSignals.PCSel = true;
    }
    
    if (id_ex.opcode == 0x67)
    { // JALR
        cpu.control_logic.controlSignals.PCSel = true;
        cpu.ex_mem.alu_result &= ~1u; // clear LSB
    }
    if (DEBUG.execute)
    {
        dump_EXMEM(cpu);
    }
}

// =============================
//  SYSTEM instruction handler
// =============================
void ExecuteStage::handle_system_instruction(RISCV &cpu)
{
    const auto &sys = cpu.id_ex.system;

    if (sys.funct3 == 0b000)
    {
        switch (sys.imm12)
        {
        case 0x000: // ECALL
            cpu.csr.trap_on_ecall(sys.pc_of_instr);
            cpu.trap_taken = true;
            cpu.trap_target = cpu.csr.read(CSRAddr::MTVEC);
            break;

        case 0x001: // EBREAK
            cpu.halted = true;
            cpu.exit_code = 0xEE;
            break;

        case 0x302: // MRET
        {
            uint32_t target = cpu.csr.do_mret();
            cpu.trap_taken = true;
            cpu.trap_target = target;
            break;
        }

        default:
            throw std::runtime_error("Unsupported SYSTEM imm12 value");
        }
    }
    else
    {
        throw std::runtime_error("Unsupported SYSTEM funct3 value");
    }
}

// =============================
//  CSR instruction handler
// =============================
void ExecuteStage::handle_csr_instruction(RISCV &cpu)
{
    const auto &id_ex = cpu.id_ex;

    uint32_t rs1_val = id_ex.rs1_val;
    uint32_t old_val = cpu.csr.exec_csr(id_ex.csr_addr, rs1_val, id_ex.funct3);

    cpu.ex_mem.alu_result = old_val;
    cpu.ex_mem.rd = id_ex.rd;
}

// =============================
//  Debugging
// =============================
void ExecuteStage::dump_EXMEM(RISCV &cpu) const
{
    const auto &ex_mem = cpu.ex_mem;
    const auto &id_ex = cpu.id_ex;
    const auto &ctrl = cpu.control_logic.controlSignals;

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
              << "\n  BranchCmp     = "
              << BranchComp::compare(id_ex.rs1_val, id_ex.rs2_val, id_ex.funct3, ctrl.BrUn)
              << "\n  PC (current)  = 0x" << std::hex << cpu.pc
              << std::dec << "\n";
}
