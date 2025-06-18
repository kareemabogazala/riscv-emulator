#ifndef R_TYPE_H
#define R_TYPE_H

#include "InstructionFormat/InstructionFormat.h"
#include "Utils/ALUOp.h"
class RType : public InstructionFormat
{
public:
    uint8_t rd, rs1, rs2, funct3, funct7, opcode;

    void decode(uint32_t raw) override;

    void generateControlSignals(ControlLogic &control) const override;

    void fill(RISCV &cpu) const override;

};

#endif // R_TYPE_H
