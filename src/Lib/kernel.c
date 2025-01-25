#include "command_handler.h"
#include "kernel.h"
#include "syscalls.h"
#include "vstring.h"
#include "fs.h"

#define STACK_SIZE 0x200 // Size of each task stack
static uint8_t task_stacks[MAX_TASKS][STACK_SIZE]; // Static memory for task stacks

static task_t tasks[MAX_TASKS]; // Array to hold tasks
static uint32_t task_count = 0; // Number of tasks created
static uint32_t current_task = 0; // Index of the currently running task

void shell_task(void) {
    print_string("[task] Shell Task started.\n"); // Debug message

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

void task_create(void (*task_entry)(void)) {
    if (task_count >= MAX_TASKS) {
        return;
    }

    task_t *task = &tasks[task_count];
    task->task_entry = task_entry;
    task->state = READY;

    task->stack_base = (uint32_t *)task_stacks[task_count];
    task->stack_pointer = task->stack_base + (STACK_SIZE / sizeof(uint32_t));

    *(--task->stack_pointer) = (uintptr_t)task_entry;

    task_count++;

    print_int((uintptr_t)task->stack_pointer);
}

void scheduler(void) {
    static int cycles_since_last_check = 0; // Track scheduler cycles
    const int check_interval = 5;          // How often to check task count

    uint32_t prev_task_idx = current_task;

    // Check task count periodically
    if (cycles_since_last_check >= check_interval) {
        cycles_since_last_check = 0;

        // Count active tasks
        int active_task_count = 0;
        for (int i = 0; i < task_count; i++) {
            if (tasks[i].state == READY) {
                active_task_count++;
            }
        }

        // If only one task is running, no need to switch
        if (active_task_count <= 1) {
            print_string("[kernel] Only one task active. Monitoring for new tasks...\n");
            return;
        }
    }

    cycles_since_last_check++;

    // Round-robin or focus on most recently created task
    if (task_count > 1) {
        // Find the most recently created READY task
        int next_task_idx = -1;
        for (int i = task_count - 1; i >= 0; i--) {
            if (tasks[i].state == READY) {
                next_task_idx = i;
                break;
            }
        }

        if (next_task_idx != -1) {
            current_task = next_task_idx;
        } else {
            current_task = (current_task + 1) % task_count; // Fallback to round-robin
        }
    }

    print_string("[kernel] Switching tasks...\n");
    print_string("Previous Task Index: ");
    print_int(prev_task_idx); // Debug previous task
    print_string("\nCurrent Task Index: ");
    print_int(current_task);  // Debug current task
    print_string("\n");

    // Perform context switch if needed
    if (prev_task_idx != current_task) {
        task_t *prev_task = &tasks[prev_task_idx];
        task_t *next_task = &tasks[current_task];
        context_switch(prev_task, next_task);
    }
}


void context_switch(task_t *prev_task, task_t *next_task) {
    print_string("[kernel] Context Switch: Saving state...\n");
    __asm__ volatile("mov %0, sp" : "=r"(prev_task->stack_pointer) : : "memory");

    print_string("[kernel] Restoring state...\n");
    __asm__ volatile("mov sp, %0" :: "r"(next_task->stack_pointer) : "memory");

    print_string("[kernel] Jumping to task...\n");
    ((void (*)(void))next_task->stack_pointer[-1])();
}


// Function to read a single character from UART
char uart_read_char(void) {
    char c;
    do {
        while (UART_FR & (1 << 4)) {} // Wait for UART to have received data
        c = (char)UART_DR;
    } while (c == 0); // Wait until data is available
    return c;
}

// Function to read a string from UART, waits for Enter key
void uart_read_string(char *buffer, int max_length) {
    int i = 0;
    char c;

    // Read characters until newline or max length is reached
    while (i < max_length - 1) {
        c = uart_read_char(); // Get a character from UART

        if (c == '\r' || c == '\n') { // If Enter is pressed (carriage return or newline)
            buffer[i] = '\0';         // Null-terminate the buffer and exit
            print_string("\n");       // Echo newline for visual feedback
            break;
        }

        if (c == '\b' && i > 0) {     // Handle backspace
            i--;
            print_string("\033[D \033[D");
        } else if (c >= 32 && c < 127) { // Only allow printable characters
            buffer[i++] = c;          // Store the character in the buffer
            UART_DR = c;              // Echo the character back to UART
        }
    }

    buffer[i] = '\0'; // Null-terminate the string just in case
}

// Function to shut down QEMU (via hypervisor call)
void system_off(void) {
    print_string("[kernel] vOS Kernel Shutdown...\n");
    register const uint64_t function_id asm("x0") = QEMU_SHUTDOWN_PORT;
    asm volatile("hvc #0" :: "r"(function_id));
    while (1)
        asm("");
}
