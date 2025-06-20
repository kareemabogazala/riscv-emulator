#include "Core/BranchComp.h"

bool BranchComp::compare(uint32_t lhs, uint32_t rhs, uint8_t funct3, bool unsignedComp)
{
    switch (funct3)
    {
    case 0x0: // BEQ
        return lhs == rhs;
    case 0x1: // BNE
        return lhs != rhs;
    case 0x4: // BLT (signed)
        return (int32_t)lhs < (int32_t)rhs;
    case 0x5: // BGE (signed)
        return (int32_t)lhs >= (int32_t)rhs;
    case 0x6: // BLTU (unsigned)
        return lhs < rhs;
    case 0x7: // BGEU (unsigned)
        return lhs >= rhs;
    default:
        return false;
    }
}
