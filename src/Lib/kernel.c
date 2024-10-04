#include "command_handler.h"
#include "kernel.h"
#include "syscalls.h"
#include "vstring.h"

// Kernel entry point
void kernel_entry(void) {
    char buffer[128];  // Buffer for storing the user input

    // Print a message indicating the kernel is running
    print_string("[kernel] Kernel initialized.\n");

    // Clear console
//    print_string("\033[2J\033[H");

    // Create test file
    syscall_fs_create("kernel.fs");
    syscall_fs_write("kernel.fs", "128 byte debug test.", 128);

    // Main kernel loop to capture input
    while (1) {
        print_string("$ ");

        uart_read_string(buffer, 128);  // Wait until Enter is pressed

        handle_command(buffer);  // Process the command using handle_command
    }
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
            print_string("\b \b");     // Remove the last character from the buffer
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
