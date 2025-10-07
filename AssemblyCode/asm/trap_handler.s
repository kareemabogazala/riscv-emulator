    .section .text
    .globl trap_handler
trap_handler:
    addi sp, sp, -16       # save caller-saved regs we use
    sw   t0, 12(sp)
    sw   t1, 8(sp)

    # --- Check mcause ---
    csrr t0, mcause        # load cause
    addi   t1,x0, 11            # 11 = ECALL from M-mode
    beq  t0, t1, handle_ecall

    li   t1, 2             # 2 = Illegal instruction
    beq  t0, t1, handle_illegal

    j handle_default       # unknown cause

# -------------------------
# ECALL handler
# a7 = syscall number
# a0 = arg0 / return value
# -------------------------
handle_ecall:
    mv   t0, a7            # syscall number → t0

    li   t1, 93             # syscall 1 = exit
    beq  t0, t1, sys_exit

    li   t1, 2             # syscall 2 = write (toy example)
    beq  t0, t1, sys_write

    j sys_unknown

sys_exit:
    # Write exit code (a0) to MMIO_EXIT
    lui  t1, %hi(0x8F000100)   # MemoryLayout::MMIO_EXIT
    addi t1, t1, %lo(0x8F000100)
    sw   a0, 0(t1)
    j trap_return

sys_write:
    # Example: return a0+1 to user
    addi a0, a0, 1
    j trap_return

sys_unknown:
    # Unknown syscall → return -1
    li a0, -1
    j trap_return

# -------------------------
# Illegal instruction
# -------------------------
handle_illegal:
    li   t3, 0xDEAD
    j trap_return

# -------------------------
# Default
# -------------------------
handle_default:
    li   t3, 0
    j trap_return

# -------------------------
# Return from trap
# -------------------------
trap_return:
    lw   t0, 12(sp)
    lw   t1, 8(sp)
    addi sp, sp, 16

    # Advance MEPC by 4 to skip the ECALL
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0

    mret
