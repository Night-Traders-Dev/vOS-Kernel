.section .text
    .global _start

_start:
    // Set the mode to ARM (0x01)
    ldr r0, =0x20000000
    mov r1, #0x01
    str r1, [r0]

    // Set up the stack pointer
    ldr sp, =0x20001000
    
    // Load the address of the kernel
    ldr r0, =KERNEL_ENTRY

    // Jump to the kernel
    bx r0

KERNEL_ENTRY:
    .word 0x10008000
