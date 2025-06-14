#include "Memory/MemoryBus.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

static std::string fmt_addr(uint32_t addr)
{
    std::ostringstream ss;
    ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << addr;
    return ss.str();
}

bool MemoryBus::in_range(uint32_t addr) const
{
    return imem.in_range(addr) || dmem.in_range(addr);
}

uint8_t MemoryBus::read8(uint32_t addr) const
{
    if (imem.in_range(addr))
        return imem.read8(addr);
    if (dmem.in_range(addr))
        return dmem.read8(addr);
    throw std::out_of_range("MemoryBus::read8: address " + fmt_addr(addr) +
                            " not mapped");
}

uint16_t MemoryBus::read16(uint32_t addr) const
{
    if (imem.in_range(addr))
        return imem.read16(addr);
    if (dmem.in_range(addr))
        return dmem.read16(addr);
    throw std::out_of_range("MemoryBus::read16: address " + fmt_addr(addr) +
                            " not mapped");
}

uint32_t MemoryBus::read32(uint32_t addr) const
{
    if (imem.in_range(addr))
        return imem.read32(addr);
    if (dmem.in_range(addr))
        return dmem.read32(addr);
    throw std::out_of_range("MemoryBus::read32: address " + fmt_addr(addr) +
                            " not mapped");
}

void MemoryBus::write8(uint32_t addr, uint8_t val)
{
    if (dmem.in_range(addr))
    {
        dmem.write8(addr, val);
        return;
    }
    throw std::out_of_range("MemoryBus::write8: address " + fmt_addr(addr) +
                            " not in data region");
}

void MemoryBus::write16(uint32_t addr, uint16_t val)
{
    if (dmem.in_range(addr) && dmem.in_range(addr + 1))
    {
        dmem.write16(addr, val);
        return;
    }
    throw std::out_of_range("MemoryBus::write16: address " + fmt_addr(addr) +
                            " not in data region");
}

void MemoryBus::write32(uint32_t addr, uint32_t val)
{
    if (dmem.in_range(addr) && dmem.in_range(addr + 3))
    {
        dmem.write32(addr, val);
        return;
    }
    throw std::out_of_range("MemoryBus::write32: address " + fmt_addr(addr) +
                            " not in data region");
}
