.text
.globl _start
_start:

    li   a7, 93              # syscall number for exit (Linux ABI uses 93)
    li   a0, 1               # exit code 0
    ecall