#pragma once
#include "InstructionFormat/InstructionFormat.h"

class CSRType : public InstructionFormat
{
public:
    uint8_t rd;
    uint8_t rs1;
    CSRAddr csr;
    uint8_t funct3;

    void decode(uint32_t raw) override;
    void generateControlSignals(ControlLogic &control) const override;
    void fill(RISCV &cpu) const override;
};
