#include "RISCV.h"
#include "Memory/IMEM.h"
#include "Memory/DMEM.h"
#include "Memory/MemoryBus.h"
#include "DebugConfig.h"
#include <iostream>
#include <memory>

uint32_t default_transform(uint32_t val) { return val; }

int main()
{
   /* DEBUG.decode = true; // Decode stage tracing
    DEBUG.fetch = true;  // Fetch stage tracing
    DEBUG.reg_dump = true;       // Print register file
    DEBUG.mem_access = true;     // Print memory read/write
    DEBUG.RISCV_IDEX = true;
    DEBUG.ALU = true;
    DEBUG.execute = true;*/
  // DEBUG.dump_dmem_every_cycle = true;
   IMem imem(0x00000000, 0x20000);
   DMem dmem(0x00100000, 0x20000);
   auto bus = std::make_shared<MemoryBus>(dmem, imem);
   RISCV cpu(bus);
   cpu.csr.write(CSRAddr::MTVEC, 0x400);

   // Handler code (addi t1,t1,1 ; mret)
   cpu.load_program("../AssemblyCode/trap_handler.bin", 0x400);

   cpu.load_program("../AssemblyCode/ecall.bin", 0x0);

   std::cout << "\n[Before Execution] Register Dump:\n";
   cpu.regs.dump();

   cpu.run(20); // allow enough cycles

   std::cout << "\n[After Execution] Register Dump:\n";
   cpu.regs.dump();

   return 0;
}
