/* kernel.S */
.section .text                                                            .global kernel_entry

/* Interrupt Vector Table for ARM64 */
.align 11                    /* Align to 2KB (required by ARM) */
vectors:
    .xword sync_handler       /* Synchronous exceptions (e.g., system calls) */
    .xword irq_handler        /* IRQ (e.g., for Ctrl+C handling) */
    .xword fiq_handler        /* FIQ (not used yet) */
    .xword error_handler      /* Error */

/* Kernel Entry Point */
kernel_entry:
    /* Set up the stack pointer */
    ldr x0, =kernel_stack_top
    mov sp, x0

    /* Print a message indicating the kernel is running */
    ldr x0, =kernel_str
    bl print_string

    /* Set up the interrupt vector table */
    ldr x0, =vectors
    msr vbar_el1, x0          /* Set Vector Base Address Register to vector table */

    /* Unmask IRQ interrupts (needed for Ctrl+C) */
    msr daifclr, #0b10        /* Unmask IRQ only, keep FIQ and Error masked */

    /* Infinite loop (main kernel loop) */
kernel_loop:
    svc #0                    /* Trigger a software interrupt (system call) */
    wfi                       /* Wait for interrupt */
    b kernel_loop             /* Stay in the loop */

/* Print a string to UART (x0 holds the string address) */
print_string:
1:
    ldrb w1, [x0], #1         /* Load a byte from the string */
    cmp w1, #0                /* Check if null byte */
    beq return_from_print     /* If null byte, return */

    /* UART output (direct) */
    mov x2, #0x09000000       /* UART base address */
    strb w1, [x2]             /* Write the byte to UART */
    b 1b                      /* Loop to next character */

return_from_print:
    ret                       /* Return from the print function */

/* Exception Handlers */

/* Synchronous Exception Handler (System Call Handler) */
sync_handler:
    ldr x0, =sync_str         /* Load the message to print for a system call */
    bl print_string           /* Print the system call message */
    eret                      /* Return from the exception */

/* IRQ Handler (For Ctrl+C) */
irq_handler:
    ldr x0, =irq_str
    bl print_string

    /* Perform QEMU shutdown using Hypervisor Call (HVC) */
    mov x0, #0x18             /* 0x18 is QEMU's "power down" signal */
    hvc #0                    /* Hypervisor call to trigger shutdown */
    b hang                    /* Halt if shutdown fails */

/* FIQ Handler (Masked, should not be triggered) */
fiq_handler:
    ldr x0, =fiq_str
    bl print_string
    b hang                    /* Halt if FIQ is triggered unexpectedly */

/* Error Handler (Catch unexpected errors) */
error_handler:
    ldr x0, =error_str
    bl print_string
    b hang                    /* Halt if an error occurs */

/* Hang the system if an unexpected exception occurs */
hang:
    wfi                      /* Wait for interrupt (sleep state) */
    b hang                   /* Stay in the hang state */

/* Data section */
.section .data
kernel_str:
    .ascii "Kernel initialized.\n"    /* Message when kernel starts */
sync_str:
    .ascii "Synchronous exception occurred (System Call).\n"
irq_str:
    .ascii "IRQ received (Ctrl+C pressed, shutting down).\n"
fiq_str:
    .ascii "FIQ received (not used).\n"
error_str:
    .ascii "Error occurred.\n"

/* Stack */
.section .bss
.align 16
.kernel_stack:
    .skip 0x2000               /* 8KB kernel stack */
kernel_stack_top:
