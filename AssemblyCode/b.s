 .text
.globl _start
_start:
    # Set up values using ADDI
    addi x1, x0, 5          # x1 = 5
    addi x2, x0, 5          # x2 = 5
    addi x3, x0, 6          # x3 = 6
    addi x4, x0, -1         # x4 = -1 (signed)
    addi x5, x0, -1         # x5 = 0xffffffff (unsigned, same as -1)

    # BEQ taken (x1 == x2)
    beq x1, x2, label_beq_taken
    addi x10, x0, 1         # FAIL PATH
label_beq_taken:
    addi x11, x0, 11        # PASS BEQ

    # BNE taken (x1 != x3)
    bne x1, x3, label_bne_taken
    addi x10, x0, 2         # FAIL PATH
label_bne_taken:
    addi x12, x0, 12        # PASS BNE

    # BLT taken (x1 < x3)
    blt x1, x3, label_blt_taken
    addi x10, x0, 3         # FAIL PATH
label_blt_taken:
    addi x13, x0, 13        # PASS BLT

    # BGE taken (x3 >= x1)
    bge x3, x1, label_bge_taken
    addi x10, x0, 4         # FAIL PATH
label_bge_taken:
    addi x14, x0, 14        # PASS BGE

    # BLTU taken (x0 < x5) unsigned
    bltu x0, x5, label_bltu_taken
    addi x10, x0, 5         # FAIL PATH
label_bltu_taken:
    addi x15, x0, 15        # PASS BLTU

    # BGEU taken (x5 >= x0) unsigned
    bgeu x5, x0, label_bgeu_taken
    addi x10, x0, 8         # FAIL PATH
label_bgeu_taken:
    addi x16, x0, 16        # PASS BGEU

     jal x0, end 
    
end:
    jal x0, end         # Infinite loop to halt
