#ifndef VSTRING_H
#define VSTRING_H

#include <stddef.h>
#include "kernel.h"

// Function prototypes
int vprint(char *buffer, size_t size, const char *format, ...);
void printf_string(const char *format, ...);
void print_string(const char *str);

// Minimal implementations of string functions
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);
int strcmp(const char *str1, const char *str2);
char* int_to_string(int num, char *buffer);
int strlength(const char *str);

#endif // VSTRING_H
