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

// Maximum number of tasks
#define MAX_TASKS 5

#define STACK_SIZE 0x200 // Size of each task stack

// Task state
typedef enum {
    RUNNING,
    READY,
    BLOCKED
} task_state_t;

// Task structure
typedef struct {
    uint32_t *stack_pointer;
    uint32_t *stack_base;
    task_state_t state;
    void (*task_entry)(void);
} task_t;

// Function prototypes
void kernel_entry(void);
char uart_read_char(void);
void uart_read_string(char *buffer, int max_length);
void system_off(void);
void handle_command(const char *command);
void task_create(void (*task_entry)(void));
void scheduler(void);
void context_switch(task_t *prev_task, task_t *next_task);

#endif // KERNEL_H
