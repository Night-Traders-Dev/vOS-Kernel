#ifndef VSTRING_H
#define VSTRING_H

#include <stddef.h>
#include "kernel.h"

// Function prototypes
int snprintf(char *buffer, size_t size, const char *format, ...);
int sprintf(char *buffer, const char *format, ...);

// Minimal implementations of string functions
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);
int strcmp(const char *str1, const char *str2);
void print_string(const char *str);
char* int_to_string(int num, char *buffer);

#endif // VSTRING_H
