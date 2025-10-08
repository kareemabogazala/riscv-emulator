/// DecodeStage.h
#ifndef DECODE_STAGE_H
#define DECODE_STAGE_H

#include "Stages/Stage.h"
#include "RISCV.h"
/// Decode stage: reads instruction from instruction memory via MemoryBus.
class DecodeStage : public Stage
{
public:
    void tick(RISCV &cpu) override;
};

#endif // DECODE_STAGE_H
