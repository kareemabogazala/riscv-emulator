#pragma once
#include <cstdint>

/*
0x8000_0000 -------------------------------
            |  Boot ROM / BIOS area (fake)
            |   - Reset PC starts here
            |   - Minimal boot stub (crt0.s, jump to kernel)
            |   - Could simulate OpenSBI later
            |   - Size: 64 KB
0x8001_0000 -------------------------------
            |  Kernel text + rodata + data + bss
            |   - Kernel image loaded here by BIOS stub
            |   - Size: ~1–4 MB
0x8010_1000 -------------------------------
            |  Trap/Interrupt Vector (mtvec)
            |   - Entry point for ecall/ebreak/interrupts
            |   - Reserve one page (4 KB)
0x8040_0000 -------------------------------
            |  Kernel heap / dynamic allocations
            |   - kmalloc, paging structures, FS buffers
            |   - Grows upward
            |
0x8800_0000 -------------------------------
            |  User space region
            |   - ELF binaries mapped here
            |   - User .text/.data, heap
            |   - Each process gets its own VA space
            |
0x8F00_0000 -------------------------------
            |  MMIO devices (future)
            |   - UART
            |   - CLINT/PLIC (timer & interrupts)
            |   - Disk controller
            |   - Reserve ~16 MB
            |
0x8FF0_0000 -------------------------------
            |  Kernel + User Stacks (top of RAM)
            |   - Kernel stack(s)
            |   - User stacks mapped downward
            |
0x9000_0000 -------------------------------
   End of 256 MB RAM
*/

namespace MemoryLayout
{
    // Base addresses
    constexpr uint32_t RAM_BASE = 0x80000000;
    constexpr uint32_t RAM_SIZE = 256 * 1024 * 1024; // 256 MB

    constexpr uint32_t BIOS_BASE = RAM_BASE;
    constexpr uint32_t BIOS_SIZE = 64 * 1024; // 64 KB

    constexpr uint32_t KERNEL_BASE = BIOS_BASE + BIOS_SIZE; // 0x80010000
    constexpr uint32_t KERNEL_SIZE = 4 * 1024 * 1024;       // reserve 4 MB

    // Trap/interrupt vector
    constexpr uint32_t MTVEC_BASE = 0x80101000; // dedicated trap entry page
    constexpr uint32_t MTVEC_SIZE = 4 * 1024;   // one page

    // Other regions
    constexpr uint32_t HEAP_BASE = 0x80400000;
    constexpr uint32_t USER_BASE = 0x88000000;
    constexpr uint32_t MMIO_BASE = 0x8F000000;
    constexpr uint32_t STACK_TOP = 0x90000000; // End of RAM
}
