#!/bin/bash
set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <source_file.c>"
    exit 1
fi

SRC=$1
BASENAME=$(basename "$SRC" .c)

ASM_DIR="AssemblyCode"
OBJ_DIR="$ASM_DIR/obj"
ELF_DIR="$ASM_DIR/elf"
BIN_DIR="$ASM_DIR/bin"

mkdir -p $OBJ_DIR $ELF_DIR $BIN_DIR

# Paths
CRT0="$ASM_DIR/crt0.s"
LINKER="$ASM_DIR/link.ld"
BIN="$BIN_DIR/$BASENAME.bin"
ELF="$ELF_DIR/$BASENAME.elf"

echo "⚙️  Compiling $SRC -> $ELF"
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -T $LINKER $CRT0 $SRC -o $ELF

echo "📦 Converting $ELF -> $BIN"
riscv64-unknown-elf-objcopy -O binary $ELF $BIN

echo "🚀 Running on emulator"
./build/emulator $BIN
