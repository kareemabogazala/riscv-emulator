# riscv-emulator
# 🧠 RISC-V Emulator  
*A single-cycle RISC-V RV32I CPU written entirely from scratch in modern C++*  

---

## 🚀 Overview
This project is a **from-scratch implementation of a RISC-V CPU emulator**, built as a personal exploration into **computer architecture, CPU design, and low-level systems**.  

It currently supports the **RV32I base instruction set**, full **memory mapping**, and can **run compiled C programs** without any external operating system.  

I started this project after completing the **Operating Systems Engineering (JOS)** course, where I built a basic OS.  
That experience made me curious about what happens *below* the OS — so I decided to build the CPU itself.  

The entire emulator was written in my free time, from an empty folder, as part of my journey to understand hardware/software interaction at the lowest level.

---

## 🏗️ Features
- **Single-Cycle CPU Core (RV32I)**  
  Implements all integer instructions and standard control flow.  
- **CSR & Trap Handling**  
  Supports `ECALL`, `MRET`, and basic machine-mode traps.  
- **Memory System & Bus**  
  Separate instruction and data memory with address decoding.  
- **Startup & Linker Support**  
  Custom `crt0.s` and `linker.ld` enabling bare-metal program loading.  
- **C Program Execution**  
  Executes RISC-V ELF binaries compiled with GCC toolchain.  
- **Debug Infrastructure**  
  Optional detailed tracing for fetch, decode, and execute stages.  

---


---

## 🧰 Build Instructions

### 1. Install RISC-V Toolchain
You need the GNU RISC-V toolchain (for assembling and linking test programs):

**macOS:**
```bash
brew tap riscv/riscv
brew install riscv-tools


2. Build the Emulator
mkdir build && cd build
cmake ..
make
