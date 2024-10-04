#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>

// UART Base Address and Registers
#define UART_BASE 0x09000000
#define UART_DR   (*(volatile uint32_t *) (UART_BASE + 0x00))  // Data register
#define UART_FR   (*(volatile uint32_t *) (UART_BASE + 0x18))  // Flag register

// QEMU Shutdown Port
#define QEMU_SHUTDOWN_PORT 0x84000008

// Function prototypes
void kernel_entry(void);
char uart_read_char(void);
void uart_read_string(char *buffer, int max_length);
void system_off(void);
void handle_command(const char *command);

#endif // KERNEL_H
