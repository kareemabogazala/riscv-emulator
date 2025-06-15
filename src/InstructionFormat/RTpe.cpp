#include "InstructionFormat/RType.h"
#include "Utiles/DecoderUtiles.h"
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
    signals.PCSel = false; // Not a jump/branch
    signals.reg_write = true;
    signals.mem_read = false;
    signals.mem_write = false;
    signals.branch = false;
    signals.mem_to_reg = false;
    signals.alu_src = false;
    signals.alu_op = determineALUOp(opcode, funct3, funct7);

    control.controlSignals = signals;

    // print the output for debug
    if (DEBUG.decode)
    {
        std::cout << "[Decode:R-Type] Control Signals:"
                  << " reg_write=" << signals.reg_write
                  << " mem_read=" << signals.mem_read
                  << " mem_write=" << signals.mem_write
                  << " branch=" << signals.branch
                  << " mem_to_reg=" << signals.mem_to_reg
                  << " alu_src=" << signals.alu_src
                  << " alu_op=" << static_cast<int>(signals.alu_op)
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
                  << " rs2_val=0x" << cpu.id_ex.rs2_val << std::dec
                  << std::endl;
    }
}
