#ifndef VSTRING_H
#define VSTRING_H

#include <stddef.h>
#include <stdarg.h>
#include "kernel.h"

// Function prototypes
void printf_string(const char *format, ...);
void print_string(const char *str);
void print_int(int value); // Function to print an integer
int vprint(char *buffer, size_t size, const char *format, va_list args);

// Minimal implementations of string functions
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);
int strcmp(const char *str1, const char *str2);
char* int_to_string(int num, char *buffer); // Converts an integer to a string
int strlength(const char *str);

#endif // VSTRING_H
