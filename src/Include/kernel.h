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

#define TIMEOUT_THRESHOLD 5000

// Extern system_ticks declaration
extern volatile uint32_t system_ticks;

// Function prototypes
void kernel_entry(void);
char uart_read_char(void);
void uart_read_string(char *buffer, int max_length, uint32_t timeout_ticks);
void system_off(void);
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks);

#endif // KERNEL_H
