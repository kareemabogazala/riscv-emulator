.text
.globl _start
_start:

    addi t1, x0, 41        # t1 = 9
    ecall                 # t1 = 42
    addi t2,x0,42         # t2 = 42
    beq t1, t2, label_beq_taken
    addi x10, x0, 1         # FAIL PATH
label_beq_taken:
    addi x11, x0, 11        # PASS BEQ

# End loop
    jal x0, 0