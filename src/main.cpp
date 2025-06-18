#include "RISCV.h"
#include "Memory/IMEM.h"
#include "Memory/DMEM.h"
#include "Memory/MemoryBus.h"
#include "Stages/FetchStage.h"
#include "DebugConfig.h"
#include <iostream>
#include <memory>
uint32_t default_trasform(uint32_t val){return val;}
int main()
{
    IMem imem(0x00000000, 64);
    DMem dmem(0x00100000, 64);
    auto bus = std::make_shared<MemoryBus>(dmem, imem);
    RISCV cpu(bus);

  
    cpu.regs.write(2, 10);
    cpu.regs.write(3, 32);
    cpu.regs.dump();
    imem.load_code_from_file("../AssemblyCode/add.bin", 0);

    // Run CPU for 3 cycles to fetch all 3 instructions
    cpu.run(1);
    std::cout<<"alu result is : [ 0x"<< std::hex <<cpu.ex_mem.alu_result<<" ]"<<std::endl;
    return 0;
}
