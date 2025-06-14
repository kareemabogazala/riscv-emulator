#include <cstdint>


class stage{

    uint32_t instr;
    

    public:
    virtual void run() = 0;


};