#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

// Syscall numbers
#define SYSCALL_PRINT_STRING 1
#define SYSCALL_EXIT 2

// Function prototypes for syscall handlers
void syscall_handler(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3);

// System call implementations
void syscall_print_string(const char *str);
void syscall_exit(void);

#endif // SYSCALLS_H
