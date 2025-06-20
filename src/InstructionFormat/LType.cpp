#include "InstructionFormat/LType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"
#include "DebugConfig.h"

void LType::decode(uint32_t raw)
{
    rd = extractRd(raw);
    rs1 = extractRs1(raw);
    funct3 = extractFunct3(raw);
    opcode = extractOpcode(raw);
    imm = extractImmediate(raw, 'I');
}

void LType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.PCSel = false;
    signals.RegWEn = true;
    signals.MemRW = MEMRW::READ;
    signals.BrUn = false;
    signals.Branch = false;
    signals.ALUSel = ALUOp::ADD; // address = rs1 + imm
    signals.ASel = ALU_A::RS1;
    signals.BSel = ALU_B::IMM;
    signals.ImmSelCode = ImmSel::IMM_I;
    signals.WBSelCode = WBSel::WB_MEM;

    control.controlSignals = signals;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:L-Type] Control Signals:"
                  << " PCSel=" << signals.PCSel
                  << " RegWEn=" << signals.RegWEn
                  << " MemRW=READ"
                  << " Branch=" << signals.Branch
                  << " BrUn=" << signals.BrUn
                  << " ALUOp=ADD"
                  << " ASel=RS1"
                  << " BSel=IMM"
                  << " ImmSel=IMM_I"
                  << " WBSel=MEM"
                  << std::endl;
    }
}

void LType::fill(RISCV &cpu) const
{
    cpu.id_ex.rd = rd;
    cpu.id_ex.rs1 = rs1;
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.imm = imm;

    cpu.id_ex.rs1_val = cpu.regs.read(rs1);

    if (DEBUG.decode)
    {
        std::cout << "[Decode:L-Type] Decoded Fields:"
                  << " rd=x" << static_cast<int>(rd)
                  << " rs1=x" << static_cast<int>(rs1)
                  << " funct3=0x" << std::hex << static_cast<int>(funct3)
                  << " opcode=0x" << static_cast<int>(opcode)
                  << " imm=0x" << imm
                  << " rs1_val=0x" << cpu.id_ex.rs1_val
                  << std::dec << std::endl;
    }
}
