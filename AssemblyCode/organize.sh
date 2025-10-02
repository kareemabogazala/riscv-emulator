#!/bin/bash
set -e

mkdir -p asm obj elf bin

# Move files by extension
find . -maxdepth 1 -name "*.s" -exec mv {} asm/ \;
find . -maxdepth 1 -name "*.o" -exec mv {} obj/ \;
find . -maxdepth 1 -name "*.elf" -exec mv {} elf/ \;
find . -maxdepth 1 -name "*.bin" -exec mv {} bin/ \;

echo "📂 Files organized into: asm/ obj/ elf/ bin/"

