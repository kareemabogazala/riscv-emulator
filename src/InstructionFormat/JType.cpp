#include "InstructionFormat/JType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h"       // Required for fill()
#include <memory>        // Required for unique_ptr
#include "DebugConfig.h" // Required for debug

void JType::decode(uint32_t raw)
{
  
    rd = extractRd(raw);
    opcode = extractOpcode(raw);
    // Manually extract J-type immediate
    int32_t imm20 = (raw >> 31) & 0x1;
    int32_t imm10_1 = (raw >> 21) & 0x3FF;
    int32_t imm11 = (raw >> 20) & 0x1;
    int32_t imm19_12 = (raw >> 12) & 0xFF;

    imm = (imm20 << 20) |
          (imm19_12 << 12) |
          (imm11 << 11) |
          (imm10_1 << 1);

    // Sign-extend to 32 bits (if imm[20] is 1, fill upper bits with 1s)
    if (imm20)
        imm |= 0xFFF00000;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:J-Type] Extracted imm = 0x" << std::hex << imm
                  << " (" << std::dec << static_cast<int32_t>(imm) << ")" << std::endl;
    }
}

void JType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.PCSel = true;        // Select PC + imm
    signals.RegWEn = true;       // Needed for JAL (writes PC+4 to rd)
    signals.MemRW = MEMRW::READ; // Don't care
    signals.BrUn = false;        // Not a branch
    signals.Branch = false;      // Not conditional

    signals.ALUSel = ALUOp::ADD;        // PC + imm
    signals.ASel = ALU_A::PC;           // ALU input A = PC
    signals.BSel = ALU_B::IMM;          // ALU input B = immediate
    signals.ImmSelCode = ImmSel::IMM_J; // Select J-type immediate
    signals.WBSelCode = WBSel::WB_PC4;  // Write PC+4 to rd

    control.controlSignals = signals;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:J-Type] Control Signals:"
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

void JType::fill(RISCV &cpu) const
{
    cpu.id_ex.rd = rd;
    cpu.id_ex.opcode = opcode;
    cpu.id_ex.imm = imm;

    if (DEBUG.decode)
    {
        std::cout << "[Decode:J-Type] Decoded Fields:"
                  << " rd=x" << static_cast<int>(rd)
                  << " opcode=0x" << std::hex << static_cast<int>(opcode)
                  << " imm=0x" << std::hex << imm
                  << std::dec << std::endl;
    }
}
