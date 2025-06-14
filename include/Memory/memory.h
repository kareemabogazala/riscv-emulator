#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>

// Memory permission flags
#define MEM_R (1 << 0)
#define MEM_W (1 << 1)
#define MEM_X (1 << 2)

class Memory
{
protected:
    std::vector<uint8_t> data;
    uint32_t base_addr;
    uint32_t size;
    uint8_t flags; // permission flags (MEM_R | MEM_W | MEM_X)

public:
    Memory(uint32_t base, uint32_t size, uint8_t flags);

    // Read methods
    virtual uint32_t read32(uint32_t addr) const;
    virtual uint16_t read16(uint32_t addr) const;
    virtual uint8_t read8(uint32_t addr) const;

    // Write methods
    virtual void write32(uint32_t addr, uint32_t val);
    virtual void write16(uint32_t addr, uint16_t val);
    virtual void write8(uint32_t addr, uint8_t val);

    // Address validation
    bool in_range(uint32_t addr) const;

    // Debugging
    template <typename Func>
    void dump(uint32_t start, uint32_t end, Func transform) const
    {
        if (start % 4 != 0 || end % 4 != 0)
        {
            throw std::invalid_argument("Addresses should be aligned to 4 bytes");
        }

        if (start < base_addr || end > base_addr + size || start >= end)
        {
            throw std::out_of_range("Invalid memory range for dump");
        }

        std::cout << "Memory dump [" << std::hex << start << " - " << end << "]\n";
        for (uint32_t addr = start; addr < end; addr += 4)
        {
            uint32_t val = read32(addr);
            uint32_t transformed = transform(val);
            std::cout << "| " << std::setw(8) << std::setfill('0') << std::hex << transformed << " |\n";
        }
    }

    // Permissions
    bool is_readable() const { return flags & MEM_R; }
    bool is_writable() const { return flags & MEM_W; }
    bool is_executable() const { return flags & MEM_X; }
};

#endif // MEMORY_H
