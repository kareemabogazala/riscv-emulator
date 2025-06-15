#include "InstructionFormat/RType.h"
// In future: include other types like IType, UType, etc.
#include "Utiles/DecoderUtiles.h"

std::unique_ptr<InstructionFormat> chooseFormat(uint8_t opcode)
{
    switch (opcode)
    {
    case 0x33: // R-type opcode for arithmetic instructions
        return std::make_unique<RType>();

        // Add other formats here as needed:
        // case 0x13: return std::make_unique<IType>();
        // case 0x37: return std::make_unique<UType>();
        // case 0x63: return std::make_unique<BType>();
        // ...
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
        int32_t imm = (((instr >> 8) & 0x0F) << 1) |
                      (((instr >> 25) & 0x3F) << 5) |
                      (((instr >> 7) & 0x01) << 11) |
                      (((int32_t)instr >> 31) << 12);
        return imm;
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
    return ALUOp::NONE;
}