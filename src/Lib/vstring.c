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

char* int_to_string(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Generate digits in reverse order
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    // Add minus sign for negative numbers
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    return buffer;
}


static int format_string(char *buffer, size_t size, const char *format, va_list args) {
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
                    buffer[count++] = '-';
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

    buffer[count < size ? count : size - 1] = '\0'; // Null-terminate the string
    return count;
}

int snprintf(char *buffer, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = format_string(buffer, size, format, args);
    va_end(args);
    return count;
}

int sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = format_string(buffer, SIZE_MAX, format, args); // SIZE_MAX for no limit
    va_end(args);
    return count;
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
