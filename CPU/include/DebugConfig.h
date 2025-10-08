#pragma once

struct DebugConfig
{
    bool decode = false;    // Decode stage tracing
    bool fetch = false;     // Fetch stage tracing
    bool reg_dump = false;   // Print register file
    bool mem_access = false; // Print memory read/write
    bool RISCV_IDEX = false;
    bool ALU = false;
    bool execute = false;
    bool dump_dmem_every_cycle = false;
};

// Global debug config instance
extern DebugConfig DEBUG;
