#include "Memory/memory.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

// Constructor: allocate storage and set flags
Memory::Memory(uint32_t base, uint32_t size, uint8_t flags)
    : base_addr(base), size(size), flags(flags), data(size) {}

// Helper to format addresses for error messages
static std::string fmt_addr(uint32_t addr)
{
    std::ostringstream ss;
    ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << addr;
    return ss.str();
}

uint8_t Memory::read8(uint32_t addr) const
{
    if (!(flags & MEM_R))
        throw std::runtime_error("read8: read not permitted");
    if (addr < base_addr || addr >= base_addr + size)
        throw std::out_of_range("read8: address " + fmt_addr(addr) + " out of range");
    return data[addr - base_addr];
}

uint16_t Memory::read16(uint32_t addr) const
{
    if (!(flags & MEM_R))
        throw std::runtime_error("read16: read not permitted");
    if (addr < base_addr || addr + 1 >= base_addr + size)
        throw std::out_of_range("read16: address " + fmt_addr(addr) + " out of range");
    uint16_t lo = read8(addr);
    uint16_t hi = read8(addr + 1);
    return static_cast<uint16_t>(lo | (hi << 8));
}

uint32_t Memory::read32(uint32_t addr) const
{
    if (!(flags & MEM_R))
        throw std::runtime_error("read32: read not permitted");
    if (addr < base_addr || addr + 3 >= base_addr + size)
        throw std::out_of_range("read32: address " + fmt_addr(addr) + " out of range");
    uint32_t b0 = read8(addr);
    uint32_t b1 = read8(addr + 1);
    uint32_t b2 = read8(addr + 2);
    uint32_t b3 = read8(addr + 3);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void Memory::write8(uint32_t addr, uint8_t val)
{
    if (!(flags & MEM_W))
        throw std::runtime_error("write8: write not permitted");
    if (addr < base_addr || addr >= base_addr + size)
        throw std::out_of_range("write8: address " + fmt_addr(addr) + " out of range");
    data[addr - base_addr] = val;
}

void Memory::write16(uint32_t addr, uint16_t val)
{
    if (!(flags & MEM_W))
        throw std::runtime_error("write16: write not permitted");
    if (addr < base_addr || addr + 1 >= base_addr + size)
        throw std::out_of_range("write16: address " + fmt_addr(addr) + " out of range");
    write8(addr, static_cast<uint8_t>(val & 0xFF));
    write8(addr + 1, static_cast<uint8_t>((val >> 8) & 0xFF));
}

void Memory::write32(uint32_t addr, uint32_t val)
{
    if (!(flags & MEM_W))
        throw std::runtime_error("write32: write not permitted");
    if (addr < base_addr || addr + 3 >= base_addr + size)
        throw std::out_of_range("write32: address " + fmt_addr(addr) + " out of range");
    write8(addr, static_cast<uint8_t>(val & 0xFF));
    write8(addr + 1, static_cast<uint8_t>((val >> 8) & 0xFF));
    write8(addr + 2, static_cast<uint8_t>((val >> 16) & 0xFF));
    write8(addr + 3, static_cast<uint8_t>((val >> 24) & 0xFF));
}

bool Memory::in_range(uint32_t addr) const
{
    return addr >= base_addr && addr < (base_addr + size);
}
