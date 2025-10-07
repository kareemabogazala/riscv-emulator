#include "RISCV.h"
#include "Memory/MemoryBus.h"
#include "Memory/MemoryLayout.h"
#include "DebugConfig.h"
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
   if (argc < 2)
   {
      std::cerr << "Usage: " << argv[0] << " <program.bin>" << std::endl;
      return 1;
   }

   const char *programPath = argv[1];

   Memory ram(MemoryLayout::RAM_BASE, MemoryLayout::RAM_SIZE);
   auto bus = std::make_shared<MemoryBus>(ram);
   RISCV cpu(bus);

   cpu.csr.write(CSRAddr::MTVEC, MemoryLayout::MTVEC_BASE);
   cpu.load_program("AssemblyCode/bin/trap_handler.bin", MemoryLayout::MTVEC_BASE);
   cpu.load_program(programPath, MemoryLayout::KERNEL_BASE);

   int cycles = cpu.run(20000);
   std::cout << "\nCycles executed: " << cycles << "\n";
   std::cout << "Exit code: " << cpu.exit_code << "\n";

   return 0;
}
