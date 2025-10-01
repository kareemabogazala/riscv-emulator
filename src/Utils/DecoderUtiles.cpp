#include "InstructionFormat/RType.h"
#include "InstructionFormat/JType.h"
#include "InstructionFormat/IType.h"
#include "InstructionFormat/LType.h"
#include "InstructionFormat/SType.h"
#include "InstructionFormat/BType.h"
#include "InstructionFormat/UType.h"
#include "InstructionFormat/SystemType.h"
#include "InstructionFormat/CSRType.h"

#include "Utils/DecoderUtils.h"

std::unique_ptr<InstructionFormat> chooseFormat(uint8_t opcode, uint8_t funct3)
{
    switch (opcode)
    {
    case 0x33: // R-type
        return std::make_unique<RType>();
    case 0x6F: // JAL
        return std::make_unique<JType>();
    case 0x13: // I-type ALU
        return std::make_unique<IType>();
    case 0x03: // Loads
        return std::make_unique<LType>();
    case 0x23: // Stores
        return std::make_unique<SType>();
    case 0x63: // Branches
        return std::make_unique<BType>();
    case 0x37: // LUI
        return std::make_unique<UType>();
    case 0x17: // AUIPC
        return std::make_unique<UType>();
    case 0x73: // SYSTEM
        if (funct3 == 0b000)
        {
            return std::make_unique<SystemType>(); // ECALL, MRET, etc.
        }
        else
        {
            return std::make_unique<CSRType>(); // CSRRW, CSRRS, CSRRC
        }
    default:
        throw std::runtime_error("Unknown instruction format for opcode: " + std::to_string(opcode));
    }
}

// Extract 7-bit opcode from instruction
uint8_t extractOpcode(uint32_t instr)
{
    return instr & 0x7F; // 0x7F = 0000 0000 0111 1111
}

// Extract 5-bit rd field
uint8_t extractRd(uint32_t instr)
{
    return (instr >> 7) & 0x1F; // 0000 0000 0001 1111
}

// Extract 5-bit rs1 field
uint8_t extractRs1(uint32_t instr)
{
    return (instr >> 15) & 0x1F; // 0x1F = 0000 0000 0001 1111
}

// Extract 5-bit rs2 field
uint8_t extractRs2(uint32_t instr)
{
    return (instr >> 20) & 0x1F; // 0x1F = 0000 0000 0001 1111
}

// Extract 3-bit funct3 field
uint8_t extractFunct3(uint32_t instr)
{
    return (instr >> 12) & 0x07; //0x07 = 0000 0000 0000 0111
}

// Extract 7-bit funct7 field
uint8_t extractFunct7(uint32_t instr)
{
    return (instr >> 25) & 0x7F; // 0x7F = 0000 0000 0111 1111
}

// Extract immediate based on instruction format
int32_t extractImmediate(uint32_t instr, char format)
{
    switch (format)
    {
    case 'I': // I-type
        return static_cast<int32_t>(instr) >> 20;
    case 'S': // S-type
        return ((instr >> 7) & 0x1F) | (((int32_t)instr >> 25) << 5);
    case 'B':
    { // B-type
        uint32_t imm = 0;
        imm |= ((instr >> 8) & 0x0F) << 1;   // bits [4:1]
        imm |= ((instr >> 25) & 0x3F) << 5;  // bits [10:5]
        imm |= ((instr >> 7) & 0x01) << 11;  // bit 11
        imm |= ((instr >> 31) & 0x01) << 12; // bit 12

        // sign-extend 13-bit to 32-bit
        if (imm & (1 << 12))   // if sign bit is set
            imm |= 0xFFFFE000; // fill upper bits with 1s

        return static_cast<int32_t>(imm);
    }
    case 'U': // U-type
        return instr & 0xFFFFF000;
    case 'J':
    { // J-type
        int32_t imm = (((instr >> 21) & 0x3FF) << 1) |
                      (((instr >> 20) & 0x01) << 11) |
                      (((instr >> 12) & 0xFF) << 12) |
                      (((int32_t)instr >> 31) << 20);
        return imm;
    }
    default:
        return 0;
    }
}

// Determine ALU operation based on opcode/funct
ALUOp determineALUOp(uint8_t opcode, uint8_t funct3, uint8_t funct7)
{
    if (opcode == 0b0110011)
    { // R-type
        if (funct3 == 0b000)
            return (funct7 == 0b0100000) ? ALUOp::SUB : ALUOp::ADD;
        if (funct3 == 0b111)
            return ALUOp::AND;
        if (funct3 == 0b110)
            return ALUOp::OR;
        if (funct3 == 0b100)
            return ALUOp::XOR;
        if (funct3 == 0b001)
            return ALUOp::SLL;
        if (funct3 == 0b101)
            return (funct7 == 0b0100000) ? ALUOp::SRA : ALUOp::SRL;
        if (funct3 == 0b010)
            return ALUOp::SLT;
        if (funct3 == 0b011)
            return ALUOp::SLTU;
    }
    else if (opcode == 0b0010011)
    { // I-type ALU instructions
        switch (funct3)
        {
        case 0b000:
            return ALUOp::ADD; // ADDI
        case 0b111:
            return ALUOp::AND; // ANDI
        case 0b110:
            return ALUOp::OR; // ORI
        case 0b100:
            return ALUOp::XOR; // XORI
        case 0b010:
            return ALUOp::SLT; // SLTI
        case 0b011:
            return ALUOp::SLTU; // SLTIU
        case 0b001:
            return ALUOp::SLL; // SLLI
        case 0b101:
            return (funct7 == 0b0100000) ? ALUOp::SRA : ALUOp::SRL; // SRAI or SRLI
        }
    }

    return ALUOp::NONE;
}
