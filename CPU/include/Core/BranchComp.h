#ifndef BRANCH_COMP_H
#define BRANCH_COMP_H

#include <cstdint>

class BranchComp
{
public:
    // Later: static bool evaluate(uint32_t rs1, uint32_t rs2, funct3);
    static bool compare(uint32_t lhs, uint32_t rhs, uint8_t funct3, bool unsignedComp); // placeholder
};

#endif // BRANCH_COMP_H
