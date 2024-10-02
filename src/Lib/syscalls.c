#include "syscalls.h"
#include "kernel.h"  // Assuming this contains necessary kernel utilities like print_string and system_off

void syscall_handler(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    switch (syscall_number) {
        case SYSCALL_PRINT_STRING:
            syscall_print_string((const char *)arg1);
            break;
        case SYSCALL_EXIT:
            syscall_exit();
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
