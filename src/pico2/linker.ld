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

    /* Define stack region for each core (optional for hybrid mode) */
    .arm_stack (NOLOAD) :
    {
        _arm_stack_start = .;
        . = . + 32K;  /* Allocate 32KB for ARM stack */
        _arm_stack_end = .;
    } > RAM

    .riscv_stack (NOLOAD) :
    {
        _riscv_stack_start = .;
        . = . + 32K;  /* Allocate 32KB for RISC-V stack */
        _riscv_stack_end = .;
    } > RAM

    _stack_top = ORIGIN(RAM) + LENGTH(RAM); /* Main Stack Pointer for ARM */

    /* Entry point */
    _start = ORIGIN(FLASH);
}

/* Assign entry point */
ENTRY(_start)
