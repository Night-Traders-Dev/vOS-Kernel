#ifndef VSTRING_H
#define VSTRING_H

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// UART Base Address and Registers
#define UART_BASE 0x09000000
#define UART_DR   (*(volatile uint32_t *) (UART_BASE + 0x00))  // Data register
#define UART_FR   (*(volatile uint32_t *) (UART_BASE + 0x18))  // Flag register

// Function prototypes
void printf_string(const char *format, ...);
void print_string(const char *str);
void print_int(int value);
void *memcpy(void *dest, const void *src, size_t n);
int vprint(char *buffer, size_t size, const char *format, va_list args);
int strcmp(const char *str1, const char *str2);
int strlength(const char *str);
char *strcpy(char *dest, const char *src);
char* int_to_string(int num, char *buffer);


#endif // VSTRING_H
