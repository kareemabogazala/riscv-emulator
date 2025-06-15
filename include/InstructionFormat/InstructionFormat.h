#ifndef INSTRUCTION_FORMAT_H
#define INSTRUCTION_FORMAT_H

#include <cstdint>
#include "ControlLogic.h" // For filling control signals
#include "RISCV.h"
class RISCV;

class InstructionFormat
{
public:
    virtual ~InstructionFormat() = default;

    /// Decode raw instruction bits into fields (e.g., rs1, rs2, rd, imm).
    virtual void decode(uint32_t raw) = 0;

    /// Fill the control signals for this instruction
    virtual void generateControlSignals(ControlLogic &control) const = 0;

    //Fill the pipe from Decode to execute (IDEX)
    virtual void fill(RISCV &cpu) const = 0;
};

#endif // INSTRUCTION_FORMAT_H
