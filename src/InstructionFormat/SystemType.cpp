#include "InstructionFormat/SystemType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"

void SystemType::decode(uint32_t raw)
{
    opcode = extractOpcode(raw); // 0x73
    funct3 = extractFunct3(raw); // [14:12]
    imm12 = (raw >> 20) & 0xFFF; // [31:20]
}

void SystemType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals s{}; // no ALU/mem action for ECALL/MRET
    s.RegWEn = false;
    control.controlSignals = s;
}

void SystemType::fill(RISCV &cpu) const
{
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.is_system = true;
    cpu.id_ex.system.imm12 = static_cast<uint16_t>(imm12);
    // Decode runs after fetch; instr PC = pc_next - 4
    cpu.id_ex.system.pc_of_instr = cpu.if_id.pc_next - 4;
    cpu.id_ex.system.writes_rd = false; // ECALL/MRET
}
