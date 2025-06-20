#ifndef S_TYPE_H
#define S_TYPE_H

#include "InstructionFormat/InstructionFormat.h"

class SType : public InstructionFormat
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

#endif
