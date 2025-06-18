.text
.globl _start
_start:
    addi x1, x0, 42        # x1 = 42
    addi x2, x1, -10       # x2 = 32
    andi x3, x2, 0xF0      # x3 = 32 & 0xF0 = 0x20
    ori  x4, x2, 0x0F      # x4 = 32 | 0x0F = 0x3F
    xori x5, x2, 0xFF      # x5 = 32 ^ 0xFF = 0xDF
    slti x6, x2, 100       # x6 = (32 < 100) = 1
    sltiu x7, x2, 10       # x7 = (32 < 10 unsigned) = 0
    slli x8, x1, 2         # x8 = 42 << 2 = 168
    srli x9, x1, 1         # x9 = 42 >> 1 = 21 (logical)
    srai x10, x1, 1        # x10 = 42 >> 1 = 21 (arithmetic)
    jal x0, 4              # skip next
    add x11, x0, x0        # skipped
