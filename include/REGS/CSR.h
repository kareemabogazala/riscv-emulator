#pragma once
#include <cstdint>
#include <unordered_map>
#include "REGS/CSRAddr.h"

class CSR
{
public:
    CSR();

    uint32_t read(CSRAddr addr) const;
    void write(CSRAddr addr, uint32_t value);

    void trap(uint32_t cause, uint32_t pc, uint32_t tval);
    void trap_on_ecall(uint32_t pc);
    uint32_t do_mret();

    uint32_t exec_csr(CSRAddr addr, uint32_t rs1_val, uint8_t funct3);

private:
    std::unordered_map<CSRAddr, uint32_t> csrs;
};
