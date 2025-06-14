#include "Memory/memory.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;
Memory::Memory(uint32_t base, uint32_t size, uint8_t flags)
    : base_addr(base), size(size), flags(flags), data(size) {}

uint32_t Memory::read32(uint32_t addr) const
{
    if (!in_range(addr) || !in_range(addr + 3))
    {
        throw std::runtime_error("write32: invalid address");
    }

    uint32_t b0 = read8(addr);
    uint32_t b1 = read8(addr + 1);
    uint32_t b2 = read8(addr + 2);
    uint32_t b3 = read8(addr + 3);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24); // Little-endian
}

uint16_t Memory::read16(uint32_t addr) const
{
    if (!in_range(addr) || !in_range(addr + 1))
    {
        throw std::runtime_error("write16: invalid address");
    }
    uint16_t low = read8(addr);
    uint16_t high = read8(addr + 1);
    return low | (high << 8);
}

uint8_t Memory::read8(uint32_t addr) const
{
    if (!in_range(addr))
    {
        throw std::runtime_error("write8: invalid address");
    }

    return data[addr - base_addr];
}
void Memory::write8(uint32_t addr, uint8_t val)
{
    if (!in_range(addr))
    {
        throw std::runtime_error("write8: invalid address");
    }
    if (!(flags & MEM_W))
    {
        throw std::runtime_error("write8: write not allowed");
    }

    data[addr - base_addr] = val;
}

void Memory::write16(uint32_t addr, uint16_t val)
{
    if (!in_range(addr) || !in_range(addr + 1))
    {
        throw std::runtime_error("write16: invalid address");
    }
    if (!(flags & MEM_W))
    {
        throw std::runtime_error("write16: write not allowed");
    }

    write8(addr, val & 0xFF);
    write8(addr + 1, (val >> 8) & 0xFF);
}

void Memory::write32(uint32_t addr, uint32_t val)
{
    if (!in_range(addr) || !in_range(addr + 3))
    {
        throw std::runtime_error("write32: invalid address");
    }
    if (!(flags & MEM_W))
    {
        throw std::runtime_error("write32: write not allowed");
    }

    write8(addr, val & 0xFF);
    write8(addr + 1, (val >> 8) & 0xFF);
    write8(addr + 2, (val >> 16) & 0xFF);
    write8(addr + 3, (val >> 24) & 0xFF);
}

bool Memory::in_range(uint32_t addr) const
{
    return addr >= base_addr && addr < (base_addr + size);
}

uint32_t default_dump(uint32_t val)
{
    return val;
}
