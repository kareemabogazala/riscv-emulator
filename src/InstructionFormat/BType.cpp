#include "InstructionFormat/BType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"
#include "DebugConfig.h"

void BType::decode(uint32_t raw)
{
    rs1 = extractRs1(raw);
    rs2 = extractRs2(raw);
    funct3 = extractFunct3(raw);
    opcode = extractOpcode(raw);
    imm = extractImmediate(raw, 'B'); // Special extraction logic

    if (DEBUG.decode)
    {
        std::cout << "[Decode:B-Type] Extracted imm = 0x" << std::hex << imm
                  << " (" << std::dec << static_cast<int32_t>(imm) << ")" << std::endl;
    }
}

void BType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.PCSel = false;                           // Decision made in execute
    signals.RegWEn = false;                          // Branches don't write to registers
    signals.MemRW = MEMRW::READ;                     // Don't care
    signals.BrUn = (funct3 == 0x6 || funct3 == 0x7); // For BLTU, BGEU
    signals.Branch = true;

    signals.ALUSel = ALUOp::ADD; // ALU used for PC + offset
    signals.ASel = ALU_A::PC;
    signals.BSel = ALU_B::IMM;
    signals.ImmSelCode = ImmSel::IMM_B;
    signals.WBSelCode = WBSel::WB_ALU; // Not used for branch, default fallback

    control.controlSignals = signals;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:B-Type] Control Signals:"
                  << " PCSel=false (set in EX)"
                  << " RegWEn=" << signals.RegWEn
                  << " Branch=" << signals.Branch
                  << " BrUn=" << signals.BrUn
                  << " ALUOp=ADD"
                  << " ASel=PC"
                  << " BSel=IMM"
                  << " ImmSel=IMM_B"
                  << " WBSel=ALU"
                  << std::endl;
    }
}

void BType::fill(RISCV &cpu) const
{
    cpu.id_ex.rs1 = rs1;
    cpu.id_ex.rs2 = rs2;
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.imm = imm;

    cpu.id_ex.rs1_val = cpu.regs.read(rs1);
    cpu.id_ex.rs2_val = cpu.regs.read(rs2);
    cpu.id_ex.is_branch = true;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:B-Type] Decoded Fields:"
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
