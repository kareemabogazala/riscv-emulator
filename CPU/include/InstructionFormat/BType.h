#ifndef B_TYPE_H
#define B_TYPE_H

#include "InstructionFormat/InstructionFormat.h"

class BType : public InstructionFormat
{
public:
    uint8_t rs1;
    uint8_t rs2;
    uint8_t funct3;
    uint8_t opcode;
    int32_t imm;

    void decode(uint32_t raw) override;
    void generateControlSignals(ControlLogic &control) const override;
    void fill(RISCV &cpu) const override;
};

#endif // B_TYPE_H
