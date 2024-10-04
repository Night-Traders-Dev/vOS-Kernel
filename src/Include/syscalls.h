#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

// Syscall numbers
#define SYSCALL_PRINT_STRING 1
#define SYSCALL_EXIT 2
#define SYSCALL_FS_CREATE 3
#define SYSCALL_FS_WRITE 4
#define SYSCALL_FS_READ 5
#define SYSCALL_FS_LS 6

// Function prototypes for syscall handlers
void syscall_handler(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3);

// System call implementations
void syscall_print_string(const char *str);
void syscall_exit(void);

// FS syscalls prototypes
void syscall_fs_create(const char *filename);
int syscall_fs_write(const char *filename, const char *data, int size);
int syscall_fs_read(const char *filename, char *buffer, int size);
void syscall_fs_ls(void);

#endif // SYSCALLS_H
