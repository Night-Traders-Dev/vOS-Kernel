ENTRY(_start)

SECTIONS
{
    . = 0x40000000;  /* Kernel entry address */
    
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
    }

    /DISCARD/ : {
        *(.comment)
        *(.note)
    }
}
