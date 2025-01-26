#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include "command_handler.h"
#include "vstring.h"
#include "fs.h"
#include "scheduler.h"
#include "timer.h"
#include "uart.h"

// QEMU Shutdown Port
#define QEMU_SHUTDOWN_PORT 0x84000008
#define SHELL_TASK_PRIORITY 100

// Function prototypes
void kernel_entry(void);
extern void system_off(void);
#endif // KERNEL_H
