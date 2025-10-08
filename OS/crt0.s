    .section .text
    .globl _start
_start:
    # Initialize stack
    li sp, 0x90000000

    # Call main()
    call main

    # After main returns → write exit code to MMIO and halt
    la t0, exit_code
    sw a0, 0(t0)         # store return value of main() into exit MMIO
    ecall                # trap (optional)

1:  j 1b                 # infinite loop

    .section .data
exit_code:
    .word 0x8F000100     # MMIO_EXIT
