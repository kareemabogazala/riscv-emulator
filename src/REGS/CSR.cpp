#include "REGS/CSR.h"
#include <stdexcept>

// mstatus bit masks (RV32)
static constexpr uint32_t MSTATUS_MIE = 1u << 3;
static constexpr uint32_t MSTATUS_MPIE = 1u << 7;
static constexpr uint32_t MSTATUS_MPP_SHIFT = 11;
static constexpr uint32_t MSTATUS_MPP_MASK = 3u << MSTATUS_MPP_SHIFT;

CSR::CSR()
{
    csrs[CSRAddr::MTVEC] = 0;
    csrs[CSRAddr::MEPC] = 0;
    csrs[CSRAddr::MCAUSE] = 0;
    csrs[CSRAddr::MSTATUS] = 0;
    csrs[CSRAddr::MTVAL] = 0; 
}

uint32_t CSR::read(CSRAddr addr) const
{
    auto it = csrs.find(addr);
    if (it == csrs.end())
        throw std::runtime_error("CSR not implemented");
    return it->second;
}

void CSR::write(CSRAddr addr, uint32_t value)
{
    auto it = csrs.find(addr);
    if (it == csrs.end())
        throw std::runtime_error("CSR not implemented");
    it->second = value;
}

// 🔹 Step-1 helpers
void CSR::trap_on_ecall(uint32_t pc)
{
    csrs[CSRAddr::MEPC] = pc;
    csrs[CSRAddr::MCAUSE] = 11; // ECALL from M-mode
    csrs[CSRAddr::MTVAL] = 0;

    uint32_t ms = csrs[CSRAddr::MSTATUS];
    uint32_t oldMIE = (ms & MSTATUS_MIE) ? 1u : 0u;

    // Update mstatus: MPIE <- MIE, MIE <- 0, MPP <- 3 (M-mode)
    ms &= ~MSTATUS_MIE;
    ms = (ms & ~MSTATUS_MPIE) | (oldMIE << 7);
    ms = (ms & ~MSTATUS_MPP_MASK) | (3u << MSTATUS_MPP_SHIFT);

    csrs[CSRAddr::MSTATUS] = ms;
}

uint32_t CSR::do_mret()
{
    uint32_t ms = csrs[CSRAddr::MSTATUS];
    uint32_t MPIE = (ms & MSTATUS_MPIE) ? 1u : 0u;

    // Restore MIE, set MPIE=1
    ms = (ms & ~MSTATUS_MIE) | (MPIE << 3);
    ms |= MSTATUS_MPIE;

    csrs[CSRAddr::MSTATUS] = ms;

    return csrs[CSRAddr::MEPC];
}

uint32_t CSR::exec_csr(CSRAddr addr, uint32_t rs1_val, uint8_t funct3)
{
    uint32_t old_val = read(addr);
    uint32_t new_val = old_val;

    switch (funct3)
    {
    case 0b001: // CSRRW
        new_val = rs1_val;
        break;
    case 0b010: // CSRRS
        if (rs1_val != 0)
            new_val = old_val | rs1_val;
        break;
    case 0b011: // CSRRC
        if (rs1_val != 0)
            new_val = old_val & ~rs1_val;
        break;
    default:
        throw std::runtime_error("Unsupported CSR funct3");
    }

    write(addr, new_val);
    return old_val; // value for rd
}
