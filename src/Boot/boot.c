#include <stdint.h>

#define UART_BASE 0x09000000        /* UART base address for QEMU virt machine */

/* Data section */
const char boot_msg[] = "[bootloader]Boot init completed\n";
const char jump_to_kernel_msg[] = "[bootloader]Kernel init starting...\n";
void (*kernel_entry)(void);       /* Kernel entry point */

/* Stack */
uint8_t stack[0x1000];            /* 4KB stack */
uint8_t* stack_top = stack + sizeof(stack); /* Stack top */

void print_string(const char* str) {
    volatile uint8_t* uart = (volatile uint8_t*)UART_BASE;
    while (*str) {
        *uart = *str++;  /* Write byte to UART */
    }
}

void _start(void) {
    /* Set up the stack pointer */
    __asm__ volatile("mov sp, %0" : : "r"(stack_top));

    /* Print bootloader message */
    print_string(boot_msg);

    /* Print the message before jumping to the kernel */
    print_string(jump_to_kernel_msg);

    /* Jump to the kernel entry point */
    kernel_entry();  /* Jump to kernel */

    /* Infinite loop to halt */
    while (1);
}
