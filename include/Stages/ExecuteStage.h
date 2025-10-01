#ifndef EXECUTE_STAGE_H
#define EXECUTE_STAGE_H

#include "Stages/Stage.h"


class ExecuteStage : public Stage {

    //helper methods
    
    public:
    void tick(RISCV& cpu) override;

    bool is_system_instruction(RISCV& cpu);
    void handle_system_instruction(RISCV& cpu);
    //
    void dump_EXMEM(RISCV& cpu) const;
};
#endif // EXECUTE_STAGE_H