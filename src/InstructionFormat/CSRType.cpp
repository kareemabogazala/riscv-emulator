#include "InstructionFormat/CSRType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"

void CSRType::decode(uint32_t raw)
{
    rd = extractRd(raw);         // [11:7]
    rs1 = extractRs1(raw);       // [19:15]
    csr = static_cast<CSRAddr>((raw >> 20) & 0xFFF);
    funct3 = extractFunct3(raw); // [14:12]
}

void CSRType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals s{};
    s.RegWEn = (rd != 0); // don’t write if rd = x0
    s.CSR = true;         // ✅ mark this as CSR instruction
    control.controlSignals = s;
}

void CSRType::fill(RISCV &cpu) const
{
    cpu.id_ex.opcode = 0x73; // SYSTEM opcode
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.rd = rd;
    cpu.id_ex.rs1 = rs1;
    cpu.id_ex.csr_addr = csr;
    cpu.id_ex.is_csr = true;

    cpu.id_ex.rs1_val = cpu.regs.read(rs1);
}
