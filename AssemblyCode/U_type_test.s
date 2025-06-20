    .text
    .globl _start
_start:
    lui x5, 0x12345       # x5 = 0x12345000
    auipc x6, 0x1         # x6 = PC + (0x1 << 12)
