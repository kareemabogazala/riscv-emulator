#include "regs.h"
using namespace std;
static const char *reg_names[32] = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

Regs::Regs()
{
    for (auto &reg : regs)
        reg = 0;
}
uint32_t Regs::read(uint32_t index) const {
    if ( index > 31)
    {
        throw out_of_range("Invalid register index");
    }
    return this->regs[index]; 
}

void Regs::write(uint32_t index, uint32_t val)
{
    if (index > 31)
    {
        throw out_of_range("Invalid register index");
    }
    //if is it the zero don't change the value
    this->regs[index] = (index == X_ZERO) ? 0 : val;
}

// Optional: dump all registers for logging/debugging
void Regs::dump() const
{
    // do prints in prof way
    // number   Name   value
    cout << " Reg  Name   Value\n";
    for (int i = 0; i < 32; ++i)
    {
        printf(" x%-2d  %-5s  0x%08X\n", i, reg_names[i], regs[i]);
    }
}