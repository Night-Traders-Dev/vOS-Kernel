/* boot.S */
.section .text
.global _start

/* Start of the bootloader */
_start:
    /* Set up the stack pointer */
    ldr x0, =stack_top
    mov sp, x0

    /* Load the base address of the string into x0 */
    ldr x0, =hello_str

    /* Write the string to the UART (serial output) */
print_hello:
    ldrb w2, [x0], #1          /* Load a byte from the string */
    cmp w2, #0                 /* Check if null byte */
    b.eq boot_kernel           /* If null byte, finish and jump to kernel */

    /* Direct UART Output */
    mov x3, #0x09000000        /* UART base address for QEMU virt machine */
    strb w2, [x3]              /* Write the byte to UART */
    b print_hello              /* Loop until string is printed */

boot_kernel:
    /* Print a message before jumping to kernel */
    ldr x0, =jump_to_kernel_str
    bl print_string

    /* Jump to the kernel entry point */
    ldr x0, =kernel_entry      /* Address of the kernel entry */
    br x0                      /* Branch to the kernel */

    /* Infinite loop to halt */
end:
    b end                      /* Hang system */

/* Function to print a string to UART */
print_string:
    mov x3, #0x09000000        /* UART base address for QEMU virt machine */
print_loop:
    ldrb w2, [x0], #1          /* Load a byte from the string */
    cmp w2, #0                 /* Check if null byte */
    b.eq print_done            /* If null byte, finish */
    strb w2, [x3]              /* Write the byte to UART */
    b print_loop               /* Loop to next character */
print_done:
    ret

/* Data section */
.section .data
hello_str:
    .ascii "Booting vOS!\n"    /* The string to print */

jump_to_kernel_str:
    .ascii "Jumping to kernel...\n"  /* Message before jumping to kernel */

/* Stack */
.section .bss
.align 16
.stack:
    .skip 0x1000               /* 4KB stack */
stack_top: