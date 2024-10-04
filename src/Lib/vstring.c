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

int snprintf(char *buffer, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
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

    buffer[count < size ? count : size - 1] = '\0'; // Null-terminate the string
    va_end(args);
    return count;
}

int sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *p;
    int count = 0;

    for (p = (char *)format; *p != '\0'; p++) {
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
                for (int j = digit_count - 1; j >= 0; j--) {
                    buffer[count++] = digits[j];
                }
                break;
            }
            case 's': { // String
                char *s = va_arg(args, char *);
                while (*s) {
                    buffer[count++] = *s++;
                }
                break;
            }
            case 'c': // Character
                buffer[count++] = (char)va_arg(args, int);
                break;
            default: // Unsupported format
                buffer[count++] = '%';
                buffer[count++] = *p;
                break;
        }
    }

    buffer[count] = '\0'; // Null-terminate the string
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
