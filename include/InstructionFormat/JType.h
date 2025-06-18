#ifndef J_TYPE_H
#define J_TYPE_H

#include "InstructionFormat/InstructionFormat.h"
#include "Utils/ALUOp.h"
class JType : public InstructionFormat
{
public:
    uint8_t rd , opcode;
    uint32_t imm;

    void decode(uint32_t raw) override;

    void generateControlSignals(ControlLogic &control) const override;

    void fill(RISCV &cpu) const override;
};

#endif // J_TYPE_H
