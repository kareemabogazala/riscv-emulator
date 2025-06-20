#include "Core/ALU.h"
#include "DebugConfig.h"
#include <iostream> // for std::cerr

std::string ALU::to_string(ALUOp op)
{
    switch (op)
    {
    case ALUOp::ADD:
        return "ADD";
    case ALUOp::SUB:
        return "SUB";
    case ALUOp::AND:
        return "AND";
    case ALUOp::OR:
        return "OR";
    case ALUOp::XOR:
        return "XOR";
    case ALUOp::SLL:
        return "SLL";
    case ALUOp::SRL:
        return "SRL";
    case ALUOp::SRA:
        return "SRA";
    case ALUOp::SLT:
        return "SLT";
    case ALUOp::SLTU:
        return "SLTU";
    case ALUOp::LUI:
        return "LUI";
    default:
        return "UNKNOWN";
    }
}

uint32_t ALU::execute(ALUOp op, uint32_t input1, uint32_t input2)
{
    if (DEBUG.ALU)
    {
        std::cout << "[ALU] op=" << to_string(op)
                  << " input1=" << input1
                  << " input2=" << input2
                  << std::endl;
    }

    switch (op)
    {
    case ALUOp::ADD:
        return input1 + input2;
    case ALUOp::SUB:
        return input1 - input2;
    case ALUOp::AND:
        return input1 & input2;
    case ALUOp::OR:
        return input1 | input2;
    case ALUOp::XOR:
        return input1 ^ input2;
    case ALUOp::SLL:
        return input1 << (input2 & 0x1F);
    case ALUOp::SRL:
        return input1 >> (input2 & 0x1F);
    case ALUOp::SRA:
        return static_cast<int32_t>(input1) >> (input2 & 0x1F);
    case ALUOp::SLT:
        return static_cast<int32_t>(input1) < static_cast<int32_t>(input2) ? 1 : 0;
    case ALUOp::SLTU:
        return input1 < input2 ? 1 : 0;
    case ALUOp::LUI:
        return input2; 
    default:
        return 0;
    }
}
