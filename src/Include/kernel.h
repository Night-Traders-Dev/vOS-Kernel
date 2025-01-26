#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include "command_handler.h"
#include "vstring.h"
#include "fs.h"
#include "scheduler.h"

// QEMU Shutdown Port
#define QEMU_SHUTDOWN_PORT 0x84000008
#define SHELL_TASK_PRIORITY 100

// Function prototypes
void kernel_entry(void);
extern char uart_read_char(void);
extern void uart_read_string(char *buffer, int max_length, uint32_t timeout_ticks);
extern void handle_uart_input(char c);
extern void system_off(void);
#endif // KERNEL_H
