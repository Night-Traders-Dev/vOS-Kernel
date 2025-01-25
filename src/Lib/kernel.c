#include "command_handler.h"
#include "kernel.h"
#include "syscalls.h"
#include "vstring.h"
#include "fs.h"
#include "scheduler.h"

// Kernel tasks
void shell_task(void) {
    char buffer[128];
    while (1) {
        print_string("$ ");
        uart_read_string(buffer, sizeof(buffer));
        handle_command(buffer);
        task_yield();
    }
}


// Kernel entry point
void kernel_entry(void) {
    static int kernel_initialized = 0;
    if (kernel_initialized) {
        print_string("[kernel] Kernel is already initialized.\n");
        system_off();
        task_yield();
        return;
    }

    kernel_initialized = 1;

    print_string("\033[2J\033[H");
    print_string("[kernel] Kernel initialized.\n");
    print_string("Welcome to vOS\n\n");



    char *kernel_fs = "Kernel Dummy File";
    char *data_fs = "Data Dummy File";
    fs_init();
    if (fs_mkdir("/") < 0 || fs_create("kernel.fs") < 0 || fs_create("data.fs") < 0) {
        print_string("[kernel] Filesystem operations failed.\n");
        system_off();
    }

    if (fs_write("data.fs", data_fs, strlength(data_fs)) < 0 || 
        fs_write("kernel.fs", kernel_fs, strlength(kernel_fs)) < 0 || 
        fs_dir_size("/", strlength(kernel_fs) + strlength(data_fs)) < 0) {
        print_string("[kernel] Filesystem operations failed.\n");
        system_off();
    }

    task_create(shell_task, 1);
    scheduler();
}



char uart_read_char(void) {
    char c;
    do {
        while (UART_FR & (1 << 4)) {}
        c = (char)UART_DR;
    } while (c == 0);
    return c;
}

void uart_read_string(char *buffer, int max_length) {
    int i = 0;
    char c;

    while (i < max_length - 1) {
        c = uart_read_char();

        if (c == '\r' || c == '\n') {
            buffer[i] = '\0';
            print_string("\n");
            break;
        }

        if (c == '\b' && i > 0) {
            i--;
            print_string("\033[D \033[D");
        } else if (c >= 32 && c < 127) {
            buffer[i++] = c;
            UART_DR = c;
        }
    }

    buffer[i] = '\0';
}

void system_off(void) {
    print_string("[kernel] vOS Kernel Shutdown...\n");
    register const uint64_t function_id asm("x0") = QEMU_SHUTDOWN_PORT;
    asm volatile("hvc #0" :: "r"(function_id));
    while (1)
        asm("");
}
