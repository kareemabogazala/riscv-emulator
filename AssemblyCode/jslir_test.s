.text
.globl _start

_start:
    # I-Type: Initialize registers
    addi x1, x0, 42            # x1 = 42
    addi x2, x0, 100           # x2 = 100

    # Manually build x3 = 0x00100000
    addi x3, x0, 256           # x3 = 0x00000100
    slli x3, x3, 8             # x3 = 0x00010000
    slli x3, x3, 4             # x3 = 0x00100000

    # S-Type: Store x1 and x2 into memory
    sw x1, 0(x3)               # MEM[x3+0] = 42
    sw x2, 4(x3)               # MEM[x3+4] = 100

    # L-Type: Load from memory into x4 and x5
    lw x4, 0(x3)               # x4 = 42
    lw x5, 4(x3)               # x5 = 100

    # R-Type: Arithmetic operations
    add x6, x4, x5             # x6 = 42 + 100 = 142
    sub x7, x5, x4             # x7 = 100 - 42 = 58

    # JAL: jump over 2 instructions
    jal x0, after_jump
    addi x8, x0, 0             # skipped
    addi x9, x0, 0             # skipped

after_jump:
    addi x10, x0, 1            # x10 = 1
