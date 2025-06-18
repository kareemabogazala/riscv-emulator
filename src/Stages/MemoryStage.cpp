#include "Stages/MemoryStage.h"
#include "RISCV.h"

void MemoryStage::tick(RISCV &cpu)
{
    const auto &ex_mem = cpu.ex_mem;
    auto &mem_wb = cpu.mem_wb;

    mem_wb.rd = ex_mem.rd;
    mem_wb.alu_result = ex_mem.alu_result;
    mem_wb.reg_write = cpu.control_logic.controlSignals.RegWEn;
    mem_wb.wb_sel = cpu.control_logic.controlSignals.WBSelCode;

    // Safe: only try to read if this is actually a load
    if (cpu.control_logic.controlSignals.MemRW == MEMRW::READ &&
        cpu.control_logic.controlSignals.WBSelCode == WBSel::WB_MEM)
    {
        if (cpu.bus->in_range(ex_mem.alu_result))
        {
            mem_wb.mem_data = cpu.bus->read32(ex_mem.alu_result);
        }
        else
        {
            mem_wb.mem_data = 0; // or throw/log warning if you prefer
        }
    }

    // Safe: only write if it's a store
    else if (cpu.control_logic.controlSignals.MemRW == MEMRW::WRITE)
    {
        if (cpu.bus->in_range(ex_mem.alu_result))
            cpu.bus->write32(ex_mem.alu_result, ex_mem.rs2_val);
    }
}
