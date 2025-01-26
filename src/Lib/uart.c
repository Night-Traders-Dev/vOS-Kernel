#include "uart.h"

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

void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_putc(char c) {
    uint32_t timeout = 1000000; // Timeout threshold
    while (UART_FR & (1 << 5)) {
        if (--timeout == 0) {
            uart_puts("[UART Timeout]\n");  // Optional error handling
            return;
        }
    }
    UART_DR = c;
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


// UART read character with timeout
int uart_read_char_with_timeout(char *c, uint32_t timeout_ticks) {
    uint32_t start_tick = 0;

    while (1) {
        if (!(UART_FR & (1 << 4))) { // UART FIFO is not empty
            *c = (char)UART_DR;
            return 0; // Character read successfully
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


void itoa(int num, char *str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // Append negative sign for negative numbers
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';  // Null terminate string

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void uart_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[256];  // Buffer to hold formatted output
    char temp[32];     // Temporary buffer for integers

    const char *ptr;
    for (ptr = fmt; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;  // Skip '%'

            // Handle different format specifiers
            if (*ptr == 'd') {
                // Print integer (decimal)
                int num = va_arg(args, int);
                itoa(num, temp, 10);
                uart_puts(temp);
            } else if (*ptr == 'x') {
                // Print integer (hexadecimal)
                int num = va_arg(args, int);
                itoa(num, temp, 16);
                uart_puts(temp);
            } else if (*ptr == 's') {
                // Print string
                char *str = va_arg(args, char*);
                uart_puts(str);
            } else if (*ptr == '%') {
                // Print the '%' character itself
                uart_putc('%');
            }
        } else {
            // Print regular character
            uart_putc(*ptr);
        }
    }

    va_end(args);
}
