#ifndef DMEM_H
#define DMEM_H

#include "memory.h"

class DMem : public Memory
{
public:
    uint32_t end_address = 0;
    // Default flags: Read + Write
    DMem(uint32_t base, uint32_t size)
        : Memory(base, size, MEM_R | MEM_W) {}

    // Allow loading data during initialization only
    void load_code(const std::vector<uint8_t> &bytes, uint32_t offset = 0);
    void load_code32(const std::vector<uint32_t> &instrs, uint32_t offset);
    void load_code_from_file(const std::string &path, uint32_t offset);
};

#endif // DMEM_H
