.text
.globl _start

_start:
    # Setup x2 = 0x00100000 (array base address)
    addi x2, x0, 256       # x2 = 0x100
    slli x2, x2, 8         # x2 = 0x10000
    slli x2, x2, 4         # x2 = 0x100000

    # Store values into memory: [10, 20, 30, 40]
    addi x3, x0, 10
    sw x3, 0(x2)

    addi x3, x0, 20
    sw x3, 4(x2)

    addi x3, x0, 30
    sw x3, 8(x2)

    addi x3, x0, 40
    sw x3, 12(x2)

    # Initialize loop
    addi x4, x0, 0         # i = 0
    addi x5, x0, 0         # sum = 0
    addi x6, x0, 4         # loop limit

loop:
    beq x4, x6, done       # if i == 4, exit

    slli x7, x4, 2         # offset = i * 4
    add x8, x2, x7         # addr = base + offset
    lw x9, 0(x8)           # load value
    add x5, x5, x9         # sum += value

    addi x4, x4, 1         # i++

    jal x0, loop

done:
      # --- Exit with ECALL ---
    li   a7, 93              # syscall number for exit (Linux ABI uses 93)
    li   a0, 1               # exit code 0
    ecall
