#ifndef MEMORYBUS_H
#define MEMORYBUS_H

#include "Memory.h"
#include "MemoryLayout.h"

class MemoryBus
{
public:
    Memory &ram;

    explicit MemoryBus(Memory &ram) : ram(ram) {}

    // Read methods
    uint32_t read32(uint32_t addr) const;
    uint16_t read16(uint32_t addr) const;
    uint8_t read8(uint32_t addr) const;

    // Write methods
    void write32(uint32_t addr, uint32_t val);
    void write16(uint32_t addr, uint16_t val);
    void write8(uint32_t addr, uint8_t val);

    // Address validation
    bool in_range(uint32_t addr) const;

    // Debugging
    template <typename Func>
    void dump(uint32_t start, uint32_t end, Func transform) const
    {
        if (in_range(start) && in_range(end - 1))
        {
            ram.dump(start, end, transform);
        }
    }
};

#endif // MEMORYBUS_H
