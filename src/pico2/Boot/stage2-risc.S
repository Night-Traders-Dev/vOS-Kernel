.section .text
    .global _start

_start:
    // Set up the stack pointer
    la sp, _stack_top

    // Jump to the kernel
    la a0, KERNEL_ENTRY
    jr a0

_stack_top:
    .word 0x20001000

KERNEL_ENTRY:
    .word 0x20010000  // Kernel entry address for RISC-V
