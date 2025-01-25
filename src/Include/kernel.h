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

// Function prototypes
void kernel_entry(void);
char uart_read_char(void);
void uart_read_string(char *buffer, int max_length, uint32_t timeout_ticks);
void system_off(void);
#endif // KERNEL_H
