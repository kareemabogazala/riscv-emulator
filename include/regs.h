#ifndef REGS_H
#define REGS_H

#include <cstdint>
#include <string>
#include <array>
#include <iostream>
#include <stdexcept>
using namespace std;
// Symbolic register names (like ABI names)
#define X_ZERO 0 // hardwired zero
#define X_RA 1   // return address
#define X_SP 2   // stack pointer
#define X_GP 3   // global pointer
#define X_TP 4   // thread pointer
#define X_T0 5
#define X_T1 6
#define X_T2 7
#define X_S0 8
#define X_FP 8 // frame pointer (alias of s0)
#define X_S1 9
#define X_A0 10
#define X_A1 11
#define X_A2 12
#define X_A3 13
#define X_A4 14
#define X_A5 15
#define X_A6 16
#define X_A7 17
#define X_S2 18
#define X_S3 19
#define X_S4 20
#define X_S5 21
#define X_S6 22
#define X_S7 23
#define X_S8 24
#define X_S9 25
#define X_S10 26
#define X_S11 27
#define X_T3 28
#define X_T4 29
#define X_T5 30
#define X_T6 31

class Regs
{
private:
    uint32_t regs[32]; // General-purpose registers
   
public:
    Regs(); // Constructor: initializes all to 0

    // Read register (x0–x31)
    uint32_t read(uint32_t index) const;

    // Write register (ignores x0)
    void write(uint32_t index, uint32_t val);

    // Optional: dump all registers for logging/debugging
    void dump() const;
};

#endif // REGS_H
