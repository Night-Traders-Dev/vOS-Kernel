/* Define the memory regions */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 4M  /* Flash memory */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 512K /* SRAM */
}

/* Define the sections */
SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))  /* Keep interrupt vectors */
        *(.text*)             /* Code */
        *(.rodata*)           /* Read-only data */
        *(.eh_frame*)         /* Exception handling tables */
    } > FLASH

    .data : AT(__data_load)
    {
        __data_start = .;
        *(.data*)             /* Initialized data */
        __data_end = .;
    } > RAM

    .bss :
    {
        __bss_start = .;
        *(.bss*)              /* Uninitialized data */
        *(COMMON)
        __bss_end = .;
    } > RAM

    _stack_top = ORIGIN(RAM) + LENGTH(RAM); /* Stack pointer */

    /* Entry point */
    _start = ORIGIN(FLASH);
}
