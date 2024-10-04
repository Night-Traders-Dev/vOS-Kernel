#include "syscalls.h"
#include "kernel.h"
#include "fs.h"
#include "vstring.h"

void syscall_handler(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    switch (syscall_number) {
        case SYSCALL_PRINT_STRING:
            syscall_print_string((const char *)arg1);
            break;
        case SYSCALL_EXIT:
            syscall_exit();
            break;
        case SYSCALL_FS_CREATE:
            syscall_fs_create((const char *)arg1);
            break;
        case SYSCALL_FS_WRITE:
            syscall_fs_write((const char *)arg1, (const char *)arg2, (int)arg3);
            break;
        case SYSCALL_FS_READ:
            syscall_fs_read((const char *)arg1, (char *)arg2, (int)arg3);
            break;
        case SYSCALL_FS_LS:
            syscall_fs_ls();
            break;
        default:
            print_string("[kernel] Invalid syscall.\n");
    }
}

void syscall_print_string(const char *str) {
    print_string(str);  // Use the kernel's print function
}

void syscall_exit(void) {
    print_string("[kernel] Shutting down...\n");
    system_off();  // Shut down the system
}

// FS syscalls implementations
void syscall_fs_create(const char *filename) {
    int result = fs_create(filename);
    if (result == 0) {
        print_string("[kernel] File created successfully.\n");
    } else {
        print_string("[kernel] Failed to create file.\n");
    }
}

int syscall_fs_write(const char *filename, const char *data, int size) {
    int result = fs_write(filename, data, size);
    if (result >= 0) {
        print_string("[kernel] File written successfully.\n");
    } else {
        print_string("[kernel] Failed to write to file.\n");
    }
    return result; // Return the number of bytes written or error code
}

int syscall_fs_read(const char *filename, char *buffer, int size) {
    int result = fs_read(filename, buffer, size);
    if (result >= 0) {
        print_string("[kernel] File read successfully.\n");
    } else {
        print_string("[kernel] Failed to read file.\n");
    }
    return result; // Return the number of bytes read or error code
}

void syscall_fs_ls(void) {
    fs_ls();
}

