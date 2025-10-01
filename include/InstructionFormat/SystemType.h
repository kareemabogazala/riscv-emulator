#ifndef SYSTEM_TYPE_H
#define SYSTEM_TYPE_H

#include "InstructionFormat/InstructionFormat.h"

class SystemType : public InstructionFormat
{
public:
    void decode(uint32_t raw) override;
    void generateControlSignals(ControlLogic &control) const override;
    void fill(RISCV &cpu) const override;

private:
    uint8_t funct3 = 0; // [14:12]
    uint32_t imm12 = 0; // [31:20]
    uint8_t opcode = 0; // 0x73
};
#endif
    // SYSTEM_TYPE_H