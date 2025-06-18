#include "InstructionFormat/IType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"
#include "DebugConfig.h"

void IType::decode(uint32_t raw)
{
    rd = extractRd(raw);
    rs1 = extractRs1(raw);
    funct3 = extractFunct3(raw);
    opcode = extractOpcode(raw);
    imm = extractImmediate(raw, 'I');
}

void IType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.PCSel = false;
    signals.RegWEn = true;
    signals.MemRW = MEMRW::READ;
    signals.BrUn = false;
    signals.Branch = false;
    signals.ALUSel = determineALUOp(opcode, funct3, 0); // funct7 not needed
    signals.ASel = ALU_A::RS1;
    signals.BSel = ALU_B::IMM;
    signals.ImmSelCode = ImmSel::IMM_I;
    signals.WBSelCode = WBSel::WB_ALU;

    control.controlSignals = signals;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:I-Type] Control Signals:"
                  << " PCSel=" << signals.PCSel
                  << " RegWEn=" << signals.RegWEn
                  << " MemRW=" << (signals.MemRW == MEMRW::WRITE ? "WRITE" : "READ")
                  << " Branch=" << signals.Branch
                  << " BrUn=" << signals.BrUn
                  << " ALUOp=" << static_cast<int>(signals.ALUSel)
                  << " ASel=" << (signals.ASel == ALU_A::PC ? "PC" : "RS1")
                  << " BSel=" << (signals.BSel == ALU_B::IMM ? "IMM" : "RS2")
                  << " ImmSel=" << static_cast<int>(signals.ImmSelCode)
                  << " WBSel=" << (signals.WBSelCode == WBSel::WB_ALU ? "ALU" : signals.WBSelCode == WBSel::WB_MEM ? "MEM"
                                                                                                                   : "PC+4")
                  << std::endl;
    }
}

void IType::fill(RISCV &cpu) const
{
    cpu.id_ex.rd = rd;
    cpu.id_ex.rs1 = rs1;
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.imm = imm;

    cpu.id_ex.rs1_val = cpu.regs.read(rs1);

    if (DEBUG.decode)
    {
        std::cout << "[Decode:I-Type] Decoded Fields:"
                  << " rd=x" << static_cast<int>(rd)
                  << " rs1=x" << static_cast<int>(rs1)
                  << " funct3=0x" << std::hex << static_cast<int>(funct3)
                  << " opcode=0x" << static_cast<int>(opcode)
                  << " imm=0x" << imm
                  << " rs1_val=0x" << cpu.id_ex.rs1_val
                  << std::dec << std::endl;
    }
}
