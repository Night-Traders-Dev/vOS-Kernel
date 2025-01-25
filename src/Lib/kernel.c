#include "command_handler.h"
#include "kernel.h"
#include "syscalls.h"
#include "vstring.h"
#include "fs.h"

static task_t tasks[MAX_TASKS]; // Array to hold tasks
static uint32_t task_count = 0; // Number of tasks created
static uint32_t current_task = 0; // Index of the currently running task

void shell_task(void) {
    char buffer[128]; // Shell input buffer
    while (1) {
        print_string("$ ");             // Display prompt
        uart_read_string(buffer, 128);  // Wait for input
        handle_command(buffer);         // Process the command
    }
}


// Kernel entry point
void kernel_entry(void) {
    char buffer[128];  // Buffer for storing the user input

    print_string("[kernel] Kernel initialized.\n");

    print_string("\033[2J\033[H");
    print_string("Welcome to vOS\n\n");

    fs_init();
    fs_mkdir("/");
    fs_create("kernel.fs");
    char *kernel_fs = "Kernel Dummy File";
    char *data_fs = "Data Dummy File";
    fs_write("kernel.fs", kernel_fs, strlength(kernel_fs));
    fs_create("data.fs");
    fs_write("data.fs", data_fs, strlength(data_fs));
    fs_dir_size("/", (strlength(kernel_fs) + strlength(data_fs)));

    task_create(shell_task);
    while (1) {
        scheduler();
    }
}

// Create a new task
void task_create(void (*task_entry)(void)) {
    if (task_count >= MAX_TASKS) return;  // Prevent creating too many tasks

    task_t *task = &tasks[task_count];
    task->task_entry = task_entry;
    task->state = TASK_READY;
    task->stack_base = (uint32_t *) 0x1000 + (task_count * 0x100); // Allocate stack
    task->stack_pointer = task->stack_base + 0x100; // Stack pointer points to top

    // Setup stack for task
    *(--task->stack_pointer) = (uint32_t) task_entry;  // Set entry point as return address
    task_count++;
}

// Simple round-robin scheduler
void scheduler(void) {
    uint32_t prev_task_idx = current_task;
    current_task = (current_task + 1) % task_count;  // Round-robin scheduler

    if (prev_task_idx != current_task) {
        task_t *prev_task = &tasks[prev_task_idx];
        task_t *next_task = &tasks[current_task];
        context_switch(prev_task, next_task);
    }
}

// Context switch between two tasks
void context_switch(task_t *prev_task, task_t *next_task) {
    // Save the state of the previous task (context saving)
    prev_task->stack_pointer = (uint32_t *)__builtin_frame_address(0);

    // Restore the state of the next task (context restoring)
    __asm__ volatile(
        "mov sp, %0\n"
        :
        : "r"(next_task->stack_pointer)
    );
}


// Function to read a single character from UART
char uart_read_char(void) {
    char c;
    do {
        while (UART_FR & (1 << 4)) {} // Wait for UART to have received data
        c = (char) UART_DR;
    } while (c == 0);  // Wait until data is available
    return c;
}

// Function to read a string from UART, waits for Enter key
void uart_read_string(char *buffer, int max_length) {
    int i = 0;
    char c;

    // Read characters until newline or max length is reached
    while (i < max_length - 1) {
        c = uart_read_char();  // Get a character from UART

        if (c == '\r' || c == '\n') {  // If Enter is pressed (carriage return or newline)
            buffer[i] = '\0';          // Null-terminate the buffer and exit
            print_string("\n");        // Echo newline for visual feedback
            break;
        }

        if (c == '\b' && i > 0) {      // Handle backspace
            i--;
            print_string("\033[D \033[D");
        } else if (c >= 32 && c < 127) { // Only allow printable characters
            buffer[i++] = c;           // Store the character in the buffer
            UART_DR = c;               // Echo the character back to UART
        }
    }

    buffer[i] = '\0';  // Null-terminate the string just in case
}

// Function to shut down QEMU (via hypervisor call)
void system_off(void) {
    print_string("[kernel] vOS Kernel Shutdown...\n");
    register const uint64_t function_id asm("x0") = QEMU_SHUTDOWN_PORT;
    asm volatile("hvc #0" :: "r"(function_id));
    while (1) asm("");
}
