#pragma once
#include <cstdint>

/// Enum representing ALU operations for RISC-V
enum class ALUOp
{
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    SLL,
    SRL,
    SRA,
    SLT,
    SLTU,
    COPY_A,
    COPY_B,
    NONE
};
