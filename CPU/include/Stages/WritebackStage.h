#ifndef WRITEBACK_STAGE_H
#define WRITEBACK_STAGE_H

#include "Stages/Stage.h"

class WritebackStage : public Stage
{
public:
    void tick(RISCV &cpu) override;
};

#endif // WRITEBACK_STAGE_H
