.text
.globl _start

_start:
    # Assume x2 = 0x00100000, and x3 = 42, x4 = 100
    # Store word from x3 into MEM[x2 + 0]
    sw x3, 0(x2)

    # Store word from x4 into MEM[x2 + 4]
    sw x4, 4(x2)

    # Load values back for verification
    lw x5, 0(x2)   # x5 = 42
    lw x6, 4(x2)   # x6 = 100

    # End loop
    jal x0, 0
