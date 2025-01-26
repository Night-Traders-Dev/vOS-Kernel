#include "kernel.h"

// Kernel tasks
void shell_task(void) {
    char buffer[128];
    const uint32_t timeout_ticks = 5000;

    while (1) {
        print_string("$ ");
        uart_read_string(buffer, sizeof(buffer), timeout_ticks);

        if (buffer[0] != '\0') {
            handle_command(buffer);
        } else {
            print_string("[kernel] No input received.\n");
        }
    }
}

void uart_task(void) {
    char c;
    while (1) {
        if (!(UART_FR & (1 << 4))) { // FIFO not empty
            c = (char)UART_DR;
            handle_uart_input(c); // Handle the character input from UART
        }
        task_yield();
    }
}

// UART input handler
void handle_uart_input(char c) {
    // For now, simply echo the character back to the UART
    if (c == '\n') {
        print_string("\n[UART Input]: New Line\n");
    } else if (c == '\r') {
        print_string("[UART Input]: Carriage Return\n");
    } else {
        print_string("[UART Input]: ");
        UART_DR = c;  // Echo the character back to UART
    }
}

// Kernel entry point
void kernel_entry(void) {
    static bool kernel_initialized = false;
    const char *kernel_fs = "Kernel Dummy File";
    const char *data_fs = "Data Dummy File";

    // Initialize filesystem
    fs_init();
    if (fs_mkdir("/") < 0) {
        print_string("[kernel] Failed to create root directory.\n");
        system_off();
    }
    if (fs_create("kernel.fs") < 0) {
        print_string("[kernel] Failed to create kernel.fs.\n");
        system_off();
    }
    if (fs_create("data.fs") < 0) {
        print_string("[kernel] Failed to create data.fs.\n");
        system_off();
    }

    if (fs_write("data.fs", data_fs, strlength(data_fs)) < 0) {
        print_string("[kernel] Failed to write to data.fs.\n");
        system_off();
    }
    if (fs_write("kernel.fs", kernel_fs, strlength(kernel_fs)) < 0) {
        print_string("[kernel] Failed to write to kernel.fs.\n");
        system_off();
    }
    if (fs_dir_size("/", strlength(kernel_fs) + strlength(data_fs)) < 0) {
        print_string("[kernel] Failed to calculate directory size.\n");
        system_off();
    }

    print_string("[kernel] Kernel initialized...starting Task and Timer\n");
    print_string("\033[2J\033[H"); // Clear screen and reset cursor
    print_string("Welcome to vOS\n\n");

    kernel_initialized = true;

    task_create(uart_task, 2);
//    init_scheduler();

    // Create the shell task
    int shell_task_idx = task_create(shell_task, 1); // Priority 1 for example
    void (*shell_task_entry)(void) = shell_task;
    execute_task_immediately(shell_task_entry);


}

// UART read character (blocking)
char uart_read_char(void) {
    char c;
    do {
        while (UART_FR & (1 << 4)) {} // Wait for UART FIFO to be not empty
        c = (char)UART_DR;
    } while (c == 0);
    return c;
}

// UART read character with timeout
int uart_read_char_with_timeout(char *c, uint32_t timeout_ticks) {
    uint32_t start_tick = system_ticks;

    while (1) {
        if (!(UART_FR & (1 << 4))) { // UART FIFO is not empty
            *c = (char)UART_DR;
            return 0; // Character read successfully
        }

        if (timeout_occurred(start_tick, timeout_ticks)) {
            return -1; // Timeout occurred
        }
    }
}

// UART read string with timeout
void uart_read_string(char *buffer, int max_length, uint32_t timeout_ticks) {
    int i = 0;
    char c;

    while (i < max_length - 1) {
        if (uart_read_char_with_timeout(&c, timeout_ticks) == 0) { // Char received
            if (c == '\r' || c == '\n') {
                buffer[i] = '\0';
                print_string("\n");
                break;
            }

            if (c == '\b' && i > 0) {
                i--;
                print_string("\b \b"); // Backspace, overwrite with space, backspace again
            } else if (c >= 32 && c < 127) { // Printable ASCII characters
                buffer[i++] = c;
                UART_DR = c; // Echo back the character
            }
        } else {
            buffer[0] = '\0'; // Timeout, return empty buffer
            print_string("[kernel] UART read timeout.\n");
            break;
        }
    }

    buffer[i] = '\0';
}

// System shutdown function
void system_off(void) {
    print_string("[kernel] vOS Kernel Shutdown...\n");

    // Platform-specific shutdown
    #if defined(TARGET_QEMU)
    register const uint64_t function_id asm("x0") = QEMU_SHUTDOWN_PORT;
    asm volatile("hvc #0" :: "r"(function_id));
    #else
    print_string("[kernel] Shutdown not supported on this platform.\n");
    while (1) {} // Infinite loop to halt execution
    #endif
}
