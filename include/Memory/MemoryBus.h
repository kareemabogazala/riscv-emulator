#ifndef MEMORYBUS_H
#define MEMORYBUS_H

#include "memory.h"
#include "DMEM.h"
#include "IMEM.h"


class MemoryBus{
    DMem& dmem;
    IMem& imem;

    public:
    MemoryBus(DMem& dmem,IMem& imem):dmem(dmem),imem(imem){}

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
        if(dmem.in_range(start)) dmem.dump(start,end,transform);
        if(imem.in_range(start)) imem.dump(start,end,transform);
    }

    
};




#endif // MEMORTBUS_H