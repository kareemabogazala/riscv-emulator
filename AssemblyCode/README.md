to make an assembly code to binary so risc-v32 can read:
riscv64-elf-as -march=rv32i -o [file_name].o [file_name].s
riscv64-elf-objcopy -O binary [file_name].o [file_name].bin 
 hexdump -C [file_name].bin



 to run C code you need:

riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -T link.ld crt0.s [file_name].c -o [file_name].elf

riscv64-unknown-elf-objcopy -O binary [file_name].elf [file_name].bin