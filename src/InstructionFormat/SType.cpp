#include "InstructionFormat/SType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"
#include "DebugConfig.h"

void SType::decode(uint32_t raw)
{
    rs1 = extractRs1(raw);
    rs2 = extractRs2(raw);
    funct3 = extractFunct3(raw);
    opcode = extractOpcode(raw);
    imm = extractImmediate(raw, 'S'); // S-type split across bits [11:7] and [31:25]
}

void SType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.PCSel = false;
    signals.RegWEn = false;       // No rd to write to
    signals.MemRW = MEMRW::WRITE; // This is a store instruction
    signals.BrUn = false;
    signals.Branch = false;
    signals.ALUSel = ALUOp::ADD; // address = rs1 + imm
    signals.ASel = ALU_A::RS1;
    signals.BSel = ALU_B::IMM;
    signals.ImmSelCode = ImmSel::IMM_S;
    signals.WBSelCode = WBSel::WB_ALU; // not used for store, but set to default

    control.controlSignals = signals;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:S-Type] Control Signals:"
                  << " PCSel=" << signals.PCSel
                  << " RegWEn=" << signals.RegWEn
                  << " MemRW=WRITE"
                  << " Branch=" << signals.Branch
                  << " BrUn=" << signals.BrUn
                  << " ALUOp=ADD"
                  << " ASel=RS1"
                  << " BSel=IMM"
                  << " ImmSel=IMM_S"
                  << " WBSel=ALU"
                  << std::endl;
    }
}

void SType::fill(RISCV &cpu) const
{
    cpu.id_ex.rs1 = rs1;
    cpu.id_ex.rs2 = rs2;
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.imm = imm;

    cpu.id_ex.rs1_val = cpu.regs.read(rs1);
    cpu.id_ex.rs2_val = cpu.regs.read(rs2); // Value to store

    if (DEBUG.decode)
    {
        std::cout << "[Decode:S-Type] Decoded Fields:"
                  << " rs1=x" << static_cast<int>(rs1)
                  << " rs2=x" << static_cast<int>(rs2)
                  << " funct3=0x" << std::hex << static_cast<int>(funct3)
                  << " opcode=0x" << static_cast<int>(opcode)
                  << " imm=0x" << imm
                  << " rs1_val=0x" << cpu.id_ex.rs1_val
                  << " rs2_val=0x" << cpu.id_ex.rs2_val
                  << std::dec << std::endl;
    }
}
