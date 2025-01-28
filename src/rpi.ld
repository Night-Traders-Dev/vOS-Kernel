ENTRY(_start)

SECTIONS
{
    . = 0x80000;  /* Kernel entry address for Raspberry Pi 4 */

    .text : {
        *(.text*)   /* All .text sections (code) */
    }

    .rodata : {
        *(.rodata*) /* Read-only data */
    }

    .data : {
        *(.data*)   /* Data sections */
    }

    .bss : {
        *(.bss*)    /* BSS (zero-initialized) sections */
        *(COMMON)   /* Common symbols */
    }

    /DISCARD/ : {
        *(.comment)
        *(.note)
    }
}
