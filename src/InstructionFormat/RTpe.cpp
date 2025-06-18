#include "InstructionFormat/RType.h"
#include "Utils/DecoderUtils.h"
#include "RISCV.h" // Required for fill()
#include <memory>  // Required for unique_ptr
#include "DebugConfig.h" //Required for debug


void RType::decode(uint32_t raw)
{
    rd = extractRd(raw);
    rs1 = extractRs1(raw);
    rs2 = extractRs2(raw);
    funct3 = extractFunct3(raw);
    funct7 = extractFunct7(raw);
    opcode = extractOpcode(raw);
}

void RType::generateControlSignals(ControlLogic &control) const
{
    ControlSignals signals{};
    signals.PCSel = false;
    signals.RegWEn = true;
    signals.MemRW = MEMRW::READ;
    signals.BrUn = false;
    signals.Branch = false;
    signals.ALUSel = determineALUOp(opcode, funct3, funct7);
    signals.ASel = ALU_A::RS1;
    signals.BSel = ALU_B::RS2;
    signals.ImmSelCode = ImmSel::IMM_I; // Could be default/dummy for R-type
    signals.WBSelCode = WBSel::WB_ALU;

    control.controlSignals = signals;

    // print the output for debug
    if (DEBUG.decode)
    {
        std::cout << "[Decode:R-Type] Control Signals:"
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

void RType::fill(RISCV &cpu) const
{
    cpu.id_ex.rd = rd;
    cpu.id_ex.rs1 = rs1;
    cpu.id_ex.rs2 = rs2;
    cpu.id_ex.funct3 = funct3;
    cpu.id_ex.funct7 = funct7;
    cpu.id_ex.opcode = opcode;

    // Load values from reg file
    cpu.id_ex.rs1_val = cpu.regs.read(rs1);
    cpu.id_ex.rs2_val = cpu.regs.read(rs2);


    //print the output for debug
    if (DEBUG.decode)
    {
        std::cout << "[Decode:R-Type] Decoded Fields:"
                  << " rd=x" << static_cast<int>(rd)
                  << " rs1=x" << static_cast<int>(rs1)
                  << " rs2=x" << static_cast<int>(rs2)
                  << " funct3=0x" << std::hex << static_cast<int>(funct3)
                  << " funct7=0x" << static_cast<int>(funct7)
                  << " opcode=0x" << static_cast<int>(opcode) << std::dec
                  << std::endl;

        std::cout << "[Decode:R-Type] Register Values:"
                  << " rs1_val=0x" << std::hex << cpu.id_ex.rs1_val
                  << " rs2_val=0x" << std::hex << cpu.id_ex.rs2_val 
                  << std::endl;
    }
}
