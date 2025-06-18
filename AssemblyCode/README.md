to make an assembly code to binary so risc-v32 can read:
riscv64-elf-as -march=rv32i -o [file_name].o [file_name].s
riscv64-elf-objcopy -O binary [file_name].o [file_name].bin 
 hexdump -C [file_name].bin