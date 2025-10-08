/// Stage.h
#ifndef STAGE_H
#define STAGE_H
#include "RISCV.h"

class RISCV; // Forward declaration
/// Abstract base class for a single pipeline stage.
class Stage
{
public:
    virtual ~Stage() = default;
    /// Perform one cycle of this stage, operating on the shared CPU state.
    virtual void tick(RISCV &cpu) = 0;
};

#endif // STAGE_H
