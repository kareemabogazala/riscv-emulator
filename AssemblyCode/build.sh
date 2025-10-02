#!/bin/bash
set -e

# Usage: ./build.sh call.s
SRC=$1
BASENAME=$(basename "$SRC" .s)

echo "🔨 Assembling asm/$BASENAME.s -> obj/$BASENAME.o"
riscv64-unknown-elf-as asm/$BASENAME.s -o obj/$BASENAME.o

echo "🔗 Linking obj/$BASENAME.o -> elf/$BASENAME.elf"
riscv64-unknown-elf-ld obj/$BASENAME.o -o elf/$BASENAME.elf -Ttext=0x0

echo "📦 Converting elf/$BASENAME.elf -> bin/$BASENAME.bin"
riscv64-unknown-elf-objcopy -O binary elf/$BASENAME.elf bin/$BASENAME.bin

echo "✅ Build complete:"
echo "  asm/$BASENAME.s -> obj/$BASENAME.o -> elf/$BASENAME.elf -> bin/$BASENAME.bin"

