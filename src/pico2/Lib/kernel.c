#include <stdint.h>

#define UART_BASE 0x40000000
#define UART_DR   (*(volatile uint32_t *)(UART_BASE + 0x00))
#define UART_FR   (*(volatile uint32_t *)(UART_BASE + 0x18))
#define UART_FR_TXFF 0x20

#define MODE_ADDRESS 0x20000000

void uart_putc(char c) {
    while (UART_FR & UART_FR_TXFF);
    UART_DR = c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void kernel_main(void) {
    volatile uint32_t* mode = (uint32_t*)MODE_ADDRESS;

    switch (*mode) {
        case 0x01:
            uart_puts("Hello World running in ARM mode\n");
            break;
        case 0x02:
            uart_puts("Hello World running in RISC-V mode\n");
            break;
        case 0x03:
            uart_puts("Hello World running in Hybrid mode\n");
            break;
        default:
            uart_puts("Unknown mode\n");
            break;
    }

    while (1) {
        // Infinite loop to halt the kernel
    }
}
