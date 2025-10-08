#ifndef IMEM_H
#define IMEM_H

#include "memory.h"

class IMem : public Memory
{
public:
    uint32_t end_address = 0;
    // Read + Execute, no write
    IMem(uint32_t base, uint32_t size)
        : Memory(base, size, MEM_R | MEM_X) {}

    // Override writes to make read-only
    void write8(uint32_t, uint8_t) override
    {
        throw std::runtime_error("IMem is read-only");
    }

    void write16(uint32_t, uint16_t) override
    {
        throw std::runtime_error("IMem is read-only");
    }

    void write32(uint32_t, uint32_t) override
    {
        throw std::runtime_error("IMem is read-only");
    }

    // Allow loading code during initialization only
    void load_code(const std::vector<uint8_t> &bytes, uint32_t offset = 0);
    void load_code32(const std::vector<uint32_t> &instrs, uint32_t offset);
    void load_code_from_file(const std::string &path, uint32_t offset);

};
#endif // IMEM_H
