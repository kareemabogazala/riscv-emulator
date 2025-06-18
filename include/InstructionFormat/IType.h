#ifndef I_TYPE_H
#define I_TYPE_H

#include "InstructionFormat/InstructionFormat.h"
#include "Utils/ALUOp.h"

class IType : public InstructionFormat
{
public:
    uint8_t rd, rs1, funct3, opcode;
    int32_t imm;
    uint8_t funct7 = 0; // only used for shifts

    void decode(uint32_t raw) override;
    void generateControlSignals(ControlLogic &control) const override;
    void fill(RISCV &cpu) const override;
};

#endif
