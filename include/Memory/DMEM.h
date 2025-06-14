#ifndef DMEM_H
#define DMEM_H

#include "memory.h"

class DMem : public Memory
{
public:
    // Default flags: Read + Write
    DMem(uint32_t base, uint32_t size)
        : Memory(base, size, MEM_R | MEM_W) {}
};

#endif // DMEM_H
