#ifndef ALU_H
#define ALU_H
#include <cstdint>
#include <string>
#include "Utils/ALUOp.h"
class ALU{
    
    public:
    
    static uint32_t execute(ALUOp op, uint32_t input1, uint32_t input2);

    static std::string to_string(ALUOp op);
};










#endif