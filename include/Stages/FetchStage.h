/// FetchStage.h
#ifndef FETCH_STAGE_H
#define FETCH_STAGE_H

#include "Stages/Stage.h"

/// Fetch stage: reads instruction from instruction memory via MemoryBus.
class FetchStage : public Stage
{
public:
    void tick(RISCV &cpu) override;
};

#endif // FETCH_STAGE_H
