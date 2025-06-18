#pragma once
#include <cstdint>
#include <memory> // at the top

#include "InstructionFormat/InstructionFormat.h"
#include "Utils/ALUOp.h"
class InstructionFormat; // forward declaration
std::unique_ptr<InstructionFormat> chooseFormat(uint8_t opcode);

uint8_t extractOpcode(uint32_t instr);
uint8_t extractRd(uint32_t instr);
uint8_t extractRs1(uint32_t instr);
uint8_t extractRs2(uint32_t instr);
uint8_t extractFunct3(uint32_t instr);
uint8_t extractFunct7(uint32_t instr);
int32_t extractImmediate(uint32_t instr, char format);

ALUOp determineALUOp(uint8_t opcode, uint8_t funct3, uint8_t funct7);