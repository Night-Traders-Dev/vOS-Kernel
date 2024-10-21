.section .text
    .global _start

_start:
    // Set up the stack pointer
    ldr sp, =0x20001000

    // Load the kernel address and jump
    ldr r0, =KERNEL_ENTRY
    bx r0

KERNEL_ENTRY:
    .word 0x10010000  // Kernel entry address for ARM
