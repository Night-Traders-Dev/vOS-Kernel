#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define UART_BASE 0x09000000    // UART base address for QEMU's virt machine
#define UART_DR   (*(volatile uint32_t *) (UART_BASE + 0x00))  // Data register
#define UART_FR   (*(volatile uint32_t *) (UART_BASE + 0x18))  // Flag register
#define QEMU_EXIT_PORT 0x501
#define QEMU_SHUTDOWN_PORT 0x604

// Function prototypes
void print_string(const char *str);
void kernel_entry(void);
char uart_read_char(void);
void uart_read_string(char *buffer, int max_length);
int strcmp(const char *str1, const char *str2);
void system_off(void);

// Kernel entry point
void kernel_entry(void) {
    char buffer[128];  // Buffer for storing the user input

    // Print a message indicating the kernel is running
    print_string("[kernel]Kernel initialized.\n");

    // Main kernel loop to capture input
    while (1) {
        print_string("$ ");

        uart_read_string(buffer, 128);  // Wait until Enter is pressed

        // Check if the command is "exit"
        if (strcmp(buffer, "exit") == 0) {
            print_string("[shell]Exit detected...\n");
            //qemu_shutdown();  // Exit QEMU
            system_off();
            break;
        }

        print_string("[kernel]Unrecognized command...\n");
    }
}

// Function to print a string to UART
void print_string(const char *str) {
    while (*str) {
        while (UART_FR & (1 << 5)) {} // Wait if UART is busy
        UART_DR = *str++;  // Output each character to UART
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

// Custom strcmp implementation
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

//qemu shutdown
void system_off(void)
    {
        print_string("[kernel]vOS Kernel Shutdown...\n");
        register const uint64_t function_id asm( "x0" ) = 0x84000008;
        asm volatile( "hvc #0" :: "r"(function_id) );
        while( 1 ) asm( "" );
    }
