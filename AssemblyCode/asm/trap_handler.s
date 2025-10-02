.section .text
.globl trap_handler
trap_handler:
    addi sp, sp, -16       # save t0,t1
    sw   t0, 12(sp)
    sw   t1, 8(sp)

    csrr t0, mcause        # load cause
    li   t1, 11            # ecall from M-mode
    beq  t0, t1, handle_ecall

    li   t1, 2             # illegal instruction
    beq  t0, t1, handle_illegal

    j handle_default       # unknown cause

handle_ecall:
    # test action: add 42 to t3
    addi t3, t3, 42
    j trap_return

handle_illegal:
    # test action: load 0xDEAD into t3
    li   t3, 0xDEAD
    j trap_return

handle_default:
    # test action: clear t3
    li   t3, 0
    j trap_return

trap_return:
    lw   t0, 12(sp)
    lw   t1, 8(sp)
    addi sp, sp, 16
    mret
