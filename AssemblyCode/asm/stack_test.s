    .section .text
    .globl _start
_start:

    # Allocate 8 bytes
    addi  sp, sp, -8

    # Load small constants
    addi  t0, x0, 0x111
    addi  t1, x0, 0x222

    # Push them
    sw    t0, 0(sp)
    sw    t1, 4(sp)

    # Clear registers
    addi  t0, x0, 0
    addi  t1, x0, 0

    # Pop them back
    lw    t0, 0(sp)
    lw    t1, 4(sp)

    # Deallocate
    addi  sp, sp, 8

    # Store results to memory
    lui   a0, %hi(0x80010000)
    addi  a0, a0, %lo(0x80010000)
    sw    t0, 0(a0)
    sw    t1, 4(a0)

    # Exit
    addi  a7, x0, 10
    ecall
