.text
.globl _start

_start:
   

    # Load a word from memory at x2 (expecting 0x0000000A) into x3
    lw x3, 0(x2)           # x3 = MEM[x2] = 10

    # Load another word at x2 + 4 (expecting 0x00000014) into x4
    lw x4, 4(x2)           # x4 = MEM[x2 + 4] = 20

    # R-type: add x5 = x3 + x4
    add x5, x3, x4         # x5 = 10 + 20 = 30

    # R-type: sub x6 = x4 - x3
    sub x6, x4, x3         # x6 = 20 - 10 = 10

    # Done
    jal x0, 0              # infinite loop (or end)
