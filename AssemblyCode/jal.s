    .text
    .globl _start
_start:

    # R-Type: Add x5 = x1 + x2
    add x5, x1, x2

    # R-Type: Sub x6 = x5 - x2
    sub x6, x5, x2

    # J-Type: Jump forward by 8 bytes (2 instructions)
    before_jump:
    jal x0, after_jump       # skip next two instructions

    # Skipped: Should NOT execute if jump works
    add x7, x0, x0    # skipped
    sub x8, x0, x0    # skipped
    after_jump:
    jal x0, before_jump

    add x10,x0 ,x0
    add x10,x0 ,x0
    add x10,x0 ,x0
    # Executed after jump
    add x9, x1, x1    # should execute
