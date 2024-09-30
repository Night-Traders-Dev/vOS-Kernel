/* kernel.ld */
ENTRY(_start)               /* Define the entry point, which is _start in boot.S */

SECTIONS
{
    /* Start the program at address 0x40000, typical for QEMU's virt machine */
    . = 0x40000;

    /* Text section for executable code */
    .text : {
        *(.text)            /* All text (code) sections from object files */
    }

    /* Data section for initialized global/static variables */
    .data : {
        *(.data)            /* All data sections */
    }

    /* BSS section for uninitialized global/static variables */
    .bss : {
        *(.bss COMMON)      /* All bss and common sections */
        . = ALIGN(16);      /* Ensure 16-byte alignment for the stack */
    }

    /* Stack */
    . = ALIGN(16);
    .stack (NOLOAD) : {
        *(.stack)           /* All stack segments */
    }

    /* Define the top of the kernel stack */
    PROVIDE(kernel_stack_top = .);
}
