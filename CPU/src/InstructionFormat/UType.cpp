#include "InstructionFormat/UType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"
#include "DebugConfig.h"

void UType::decode(uint32_t raw)
{
    rd = extractRd(raw);
    opcode = extractOpcode(raw);
    imm = extractImmediate(raw, 'U');
}

void UType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.RegWEn = true;
    signals.MemRW = MEMRW::READ;
    signals.BrUn = false;
    signals.Branch = false;
    signals.ImmSelCode = ImmSel::IMM_U;
    signals.WBSelCode = WBSel::WB_ALU;

    if (opcode == 0x37)
    { // LUI
        signals.PCSel = false;
        signals.ALUSel = ALUOp::LUI;
        signals.ASel = ALU_A::PC; // dont care
        signals.BSel = ALU_B::IMM;
    }
    else if (opcode == 0x17)
    { // AUIPC
        signals.PCSel = false;
        signals.ALUSel = ALUOp::ADD;
        signals.ASel = ALU_A::PC;
        signals.BSel = ALU_B::IMM;
    }
    else
    {
        throw std::runtime_error("Unknown U-type opcode: 0x" + std::to_string(opcode));
    }

    control.controlSignals = signals;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:U-Type] Control Signals:"
                  << " PCSel=" << signals.PCSel
                  << " RegWEn=" << signals.RegWEn
                  << " MemRW=" << (signals.MemRW == MEMRW::WRITE ? "WRITE" : "READ")
                  << " Branch=" << signals.Branch
                  << " BrUn=" << signals.BrUn
                  << " ALUOp=" << static_cast<int>(signals.ALUSel)
                  << " ASel=" << static_cast<int>(signals.ASel)
                  << " BSel=" << static_cast<int>(signals.BSel)
                  << " ImmSel=" << static_cast<int>(signals.ImmSelCode)
                  << " WBSel=ALU"
                  << std::endl;
    }
}

void UType::fill(RISCV &cpu) const
{
    cpu.id_ex.rd = rd;
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.imm = imm;
    cpu.id_ex.rs1_val = 0; // Not used

    if (DEBUG.decode)
    {
        std::cout << "[Decode:U-Type] Decoded Fields:"
                  << " rd=x" << static_cast<int>(rd)
                  << " opcode=0x" << std::hex << static_cast<int>(opcode)
                  << " imm=0x" << imm
                  << std::dec << std::endl;
    }
}
