

class Memory{
    protected:
    vector<uint8_t> data;
    uint32_t base_addr , size; 
    public:
    Memory(unint32_t base , uint32_t size);

    virtual uint32_t read32(uint32_t addr) const;
    virtual uint16_t read16(uint16_t addr) const;
    virtual uint8_t read8(uint8_t addr) const;

    
    virtual void write32(uint32_t addr , uint32_t val) const;
    virtual void write16(uint16_t addr, uint16_t val) const;
    virtual void write8(uint8_t addr, uint8_t val) const;

    bool in_range(uint32_t addr) const;
    void dump(uint32_t start , uint32_t end) const;
    
};