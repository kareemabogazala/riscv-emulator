#ifndef MEMORY_STAGE_H
#define MEMORY_STAGE_H

#include "Stages/Stage.h"

class MemoryStage : public Stage
{
public:
    void tick(RISCV &cpu) override;
};

#endif // MEMORY_STAGE_H
