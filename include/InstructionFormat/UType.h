#ifndef U_TYPE_H
#define U_TYPE_H

#include "InstructionFormat/InstructionFormat.h"
#include "Utils/ALUOp.h"

class UType : public InstructionFormat
{
public:
    uint8_t rd, opcode;
    int32_t imm;

    void decode(uint32_t raw) override;
    void generateControlSignals(ControlLogic &control) const override;
    void fill(RISCV &cpu) const override;
};

#endif
