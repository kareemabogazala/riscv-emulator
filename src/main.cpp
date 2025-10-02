#include "RISCV.h"
#include "Memory/MemoryBus.h"
#include "Memory/MemoryLayout.h"
#include "DebugConfig.h"
#include <iostream>
#include <memory>

int main()
{
   Memory ram(MemoryLayout::RAM_BASE, MemoryLayout::RAM_SIZE);
   auto bus = std::make_shared<MemoryBus>(ram);
   RISCV cpu(bus);

   // Set mtvec to trap handler base
   cpu.csr.write(CSRAddr::MTVEC, MemoryLayout::MTVEC_BASE);

   // Load trap handler at MTVEC
   cpu.load_program("AssemblyCode/bin/trap_handler.bin", MemoryLayout::MTVEC_BASE);

   // Load test program (ecall test) at KERNEL_BASE
   cpu.load_program("AssemblyCode/bin/ecall.bin", MemoryLayout::KERNEL_BASE);

   std::cout << "\n[Before Execution] Register Dump:\n";
   cpu.regs.dump();

   cpu.run(20); // run enough cycles

   std::cout << "\n[After Execution] Register Dump:\n";
   cpu.regs.dump();

   std::cout << "\n[Stack Test Results @0x80010000]\n";
   bus->ram.dump(0x80010000, 0x80010010, [](uint32_t val)
                 { return val; });

   return 0;
}
