#pragma once
#include <cstdint>

enum class CSRAddr : uint16_t
{
    MSTATUS = 0x300,
    MIE = 0x304,
    MTVEC = 0x305,
    MEPC = 0x341,
    MCAUSE = 0x342,
    MTVAL = 0x343,
    MIP = 0x344,
};
