.text
.globl _start
_start:
    # ADD rd, rs1, rs2
    add x1, x2, x3        # funct3 = 0x0, funct7 = 0x00
    # SUB rd, rs1, rs2
    sub x4, x5, x6        # funct3 = 0x0, funct7 = 0x20
    # SLL rd, rs1, rs2
    sll x7, x8, x9        # funct3 = 0x1, funct7 = 0x00
    # SLT rd, rs1, rs2
    slt x10, x11, x12     # funct3 = 0x2
    # SLTU rd, rs1, rs2
    sltu x13, x14, x15    # funct3 = 0x3
    # XOR rd, rs1, rs2
    xor x16, x17, x18     # funct3 = 0x4
    # SRL rd, rs1, rs2
    srl x19, x20, x21     # funct3 = 0x5, funct7 = 0x00
    # SRA rd, rs1, rs2
    sra x22, x23, x24     # funct3 = 0x5, funct7 = 0x20
    # OR rd, rs1, rs2
    or  x25, x26, x27     # funct3 = 0x6
    # AND rd, rs1, rs2
    and x28, x29, x30     # funct3 = 0x7
