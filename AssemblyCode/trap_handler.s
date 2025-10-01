    .section .text
    .globl trap_handler
trap_handler:
    # t1 = t1 + 1
    addi t1, t1, 1

    # advance mepc by 4
    csrr  t0, mepc
    addi  t0, t0, 4
    csrw  mepc, t0

    # return
    mret
