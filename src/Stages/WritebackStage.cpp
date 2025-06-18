#include "Stages/WritebackStage.h"
#include "RISCV.h"

void WritebackStage::tick(RISCV &cpu)
{
    const auto &mem_wb = cpu.mem_wb;

    if (!mem_wb.reg_write || mem_wb.rd == 0)
        return; // Skip x0 and disabled writes

    uint32_t value = 0;

    switch (mem_wb.wb_sel)
    {
    case WBSel::WB_ALU:
        value = mem_wb.alu_result;
        break;
    case WBSel::WB_MEM:
        value = mem_wb.mem_data;
        break;
    case WBSel::WB_PC4:
        value = cpu.pc + 4;
        break;
    }

    cpu.regs.write(mem_wb.rd, value);
}
