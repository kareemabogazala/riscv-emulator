#include "Stages/DecodeStage.h"
#include "RISCV.h"


void DecodeStage::tick(RISCV &cpu){
    auto format = chooseFormat(extractOpcode(cpu.if_id.instr),extractFunct3(cpu.if_id.instr));
    format->decode(cpu.if_id.instr);
    format->generateControlSignals(cpu.control_logic); // if shared
    format->fill(cpu);
}