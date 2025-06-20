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
   DEBUG.dump_dmem_every_cycle = true;
   IMem imem(0x00000000, 0x20000);
   DMem dmem(0x00100000, 0x20000);
   auto bus = std::make_shared<MemoryBus>(dmem, imem);
   RISCV cpu(bus);

   std::cout << "[Before Execution] Initial Register Dump:\n";
   cpu.regs.write(2, 10);                          // x2
   cpu.regs.write(3, 32);                          // x3
   cpu.regs.write(5, 100);                         // x5
   cpu.regs.write(6, 58);                          // x6
   cpu.regs.write(8, 1);                           // x8
   cpu.regs.write(9, 5);                           // x9
   cpu.regs.write(11, 10);                         // x11
   cpu.regs.write(12, 20);                         // x12
   cpu.regs.write(14, 10);                         // x14
   cpu.regs.write(15, 5);                          // x15
   cpu.regs.write(17, 0xFF00);                     // x17
   cpu.regs.write(18, 0x0F0F);                     // x18
   cpu.regs.write(20, 128);                        // x20
   cpu.regs.write(21, 2);                          // x21
   cpu.regs.write(23, static_cast<uint32_t>(-32)); // x23
   cpu.regs.write(24, 2);                          // x24
   cpu.regs.write(26, 0xF0F0);                     // x26
   cpu.regs.write(27, 0x0F0F);                     // x27
   cpu.regs.write(29, 0xFF00);                     // x29
   cpu.regs.write(30, 0x0F0F);                     // x30

   // Load 10-instruction R-type test program
   cpu.load_program("AssemblyCode/add.bin", 0);

  ;
   // dump imem
   std::cout << "\n[After Loading] Instruction Memory Dump:\n";
   imem.dump(0x00000000, 0x00000040, default_transform);

   // Run emulator
   cpu.run(60); // allow enough cycles

   std::cout << "\n[After Execution] Register Dump:\n";
   cpu.regs.dump();

   std::cout << "\n[After Execution] Data Memory Dump:\n";
   dmem.dump(0x00100000, 0x00100010, default_transform);

   std::cout << "\nALU result in EX/MEM stage: 0x" << std::hex << cpu.ex_mem.alu_result << std::endl;

   return 0;
}
