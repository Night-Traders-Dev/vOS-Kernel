.section .text
    .global _start

_start:
    // Set the mode to RISC-V (0x02)
    la t0, _mode_address
    li t1, 0x02
    sw t1, 0(t0)

    // Set up the stack pointer
    la sp, _stack_top

    // Load the address of the kernel
    la a0, KERNEL_ENTRY

    // Jump to the kernel
    jr a0

_stack_top:
    .word 0x20001000

_mode_address:
    .word 0x20000000

KERNEL_ENTRY:
    .word 0x10008000
