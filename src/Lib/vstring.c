#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include "vstring.h"
#include "kernel.h"


// Function to print a string to UART
void print_string(const char *str) {
    while (*str) {
        while (UART_FR & (1 << 5)) {} // Wait if UART is busy
        UART_DR = *str++;  // Output each character to UART
    }
}

void printf_string(const char *format, ...) {
    char buffer[128];  // Buffer for formatted string
    va_list args;
    va_start(args, format);
    vprint(buffer, sizeof(buffer), format, args);  // Pass va_list directly
    va_end(args);

    // Output formatted string via UART
    char *str = buffer;
    while (*str) {
        while (UART_FR & (1 << 5)) {} // Wait if UART is busy
        UART_DR = *str++;  // Output each character to UART
    }
}

int vprint(char *buffer, size_t size, const char *format, va_list args) {
    char *p;
    int count = 0;

    for (p = (char *)format; *p != '\0' && count < size - 1; p++) {
        if (*p != '%') {
            buffer[count++] = *p;
            continue;
        }

        p++; // Move past '%'

        switch (*p) {
            case 'd': { // Integer
                int i = va_arg(args, int);
                if (i < 0) {
                    if (count < size - 1) {
                        buffer[count++] = '-';
                    }
                    i = -i;
                }
                char digits[10];
                int digit_count = 0;
                do {
                    if (digit_count < sizeof(digits)) {
                        digits[digit_count++] = (i % 10) + '0';
                    }
                    i /= 10;
                } while (i > 0 && digit_count < sizeof(digits));
                for (int j = digit_count - 1; j >= 0 && count < size - 1; j--) {
                    buffer[count++] = digits[j];
                }
                break;
            }
            case 's': { // String
                char *s = va_arg(args, char *);
                while (*s && count < size - 1) {
                    buffer[count++] = *s++;
                }
                break;
            }
            case 'c': // Character
                if (count < size - 1) {
                    buffer[count++] = (char)va_arg(args, int);
                }
                break;
            default: // Unsupported format
                if (count < size - 1) {
                    buffer[count++] = '%';
                }
                if (count < size - 1) {
                    buffer[count++] = *p;
                }
                break;
        }
    }

    buffer[count] = '\0'; // Null-terminate the string
//    va_end[args];
    return count;
}


int strlength(const char *str) {
    int length = 0;
    while (*str++) {
        length++;
    }
    return length;
}


// Minimal implementation of strcpy
char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++)) {}
    return ret;
}

// Minimal implementation of memcpy
void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

// Custom strcmp implementation
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}


char* int_to_string(int num, char *buffer) {
    int is_negative = 0;
    int i = 0;

    // Handle 0 explicitly
    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Convert the number to a string (reverse order)
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Add the negative sign if needed
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    return buffer;
}

void print_int(int value) {
    char buffer[12]; // Enough to store the string representation of a 32-bit integer, including the sign
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        print_string(buffer);
        return;
    }

    // Handle negative numbers
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // Convert the number to a string (reverse order)
    while (value > 0) {
        buffer[i++] = (value % 10) + '0'; // Extract the last digit and convert to char
        value /= 10;                     // Remove the last digit
    }

    // Add the negative sign if necessary
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Reverse the string to get the correct order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    // Print the string
    print_string(buffer);
}
