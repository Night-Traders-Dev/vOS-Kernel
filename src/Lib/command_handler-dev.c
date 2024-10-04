#include "command_handler.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "kernel.h"
#include "syscalls.h"

// Custom string functions
size_t my_strlen(const char *str) {
    const char *s = str;
    while (*s)
        s++;
    return s - str;
}

// Custom implementation of string copy
void my_strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++));
}

// A simple sscanf replacement to extract command and arguments
int my_sscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *arg1 = va_arg(args, char *);
    char *arg2 = va_arg(args, char *);
    char *arg3 = va_arg(args, char *);

    // Reset the count
    int count = 0;

    // Skip leading whitespace
    while (*str == ' ') str++;

    // Extract command
    while (*str && *str != ' ' && count < 3) {
        if (count == 0) {
            my_strcpy(arg1, str);
            while (*str && *str != ' ') str++; // Move str pointer to the next space
            count++;
        } else {
            str++; // Move to the next character
        }
    }
    arg1[my_strlen(arg1)] = '\0'; // Null-terminate the first argument

    // Skip whitespace for the second argument
    while (*str == ' ') str++;

    // Extract second argument
    while (*str && *str != ' ' && count < 3) {
        if (count == 1) {
            my_strcpy(arg2, str);
            while (*str && *str != ' ') str++; // Move str pointer to the next space
            count++;
        } else {
            str++; // Move to the next character
        }
    }
    arg2[my_strlen(arg2)] = '\0'; // Null-terminate the second argument

    // Skip whitespace for the third argument
    while (*str == ' ') str++;

    // Extract third argument
    while (*str && count < 3) {
        if (count == 2) {
            my_strcpy(arg3, str);
            while (*str && *str != ' ') str++; // Move str pointer to the next space
            count++;
        } else {
            str++; // Move to the next character
        }
    }
    arg3[my_strlen(arg3)] = '\0'; // Null-terminate the third argument

    va_end(args);
    return count;
}

// Array of commands for help function
const char *commands[] = {
    "clear - clears console",
    "version - displays version number",
    "help - displays this menu",
    "exit - shuts down vOS",
    "create <filename> - creates a new file",
    "write <filename> <data> - writes data to a file",
    "read <filename> - reads data from a file"
};

void handle_command(const char *buffer) {
    char command[256];
    char arg1[128] = {0}; // Initialize to avoid garbage values
    char arg2[128] = {0}; // Initialize to avoid garbage values
    char buffer_copy[256]; // Copy buffer to not modify original
    my_strcpy(buffer_copy, buffer); // Use my_strcpy instead of strncpy
    buffer_copy[sizeof(buffer_copy) - 1] = '\0'; // Ensure null-termination

    // Debugging print
    syscall_print_string("[DEBUG] Processing command: ");
    syscall_print_string(buffer_copy);

    // Extract command first
    int num_args = my_sscanf(buffer_copy, "%s", command); // Get only command initially

    // Debugging print
    syscall_print_string("[DEBUG] Command: ");
    syscall_print_string(command);

    if (strcmp(command, "exit") == 0) {
        syscall_exit();
    } else if (strcmp(command, "clear") == 0) {
        syscall_print_string("\033[2J\033[H");
    } else if (strcmp(command, "version") == 0) {
        syscall_print_string("[shell] Version: 0.0.1\n");
    } else if (strcmp(command, "help") == 0) {
        syscall_print_string("[shell] Commands:\n");
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            syscall_print_string(commands[i]);
            syscall_print_string("\n");
        }
    } else if (strcmp(command, "create") == 0) {
        num_args = my_sscanf(buffer_copy, "%s %s", command, arg1);
        if (num_args == 2) {
            syscall_fs_create(arg1);
        } else {
            syscall_print_string("[kernel] Missing filename for create command...\n");
        }
    } else if (strcmp(command, "write") == 0) {
        num_args = my_sscanf(buffer_copy, "%s %s %s", command, arg1, arg2);
        if (num_args == 3) {
            syscall_fs_write(arg1, arg2, my_strlen(arg2)); // Use my_strlen
        } else {
            syscall_print_string("[kernel] Missing filename or data for write command...\n");
        }
    } else if (strcmp(command, "read") == 0) {
        num_args = my_sscanf(buffer_copy, "%s %s", command, arg1);
        if (num_args == 2) {
            char read_buffer[1024]; // Adjust buffer size as needed
            syscall_fs_read(arg1, read_buffer, sizeof(read_buffer));
            syscall_print_string("[kernel] Read data:\n");
            syscall_print_string(read_buffer);
        } else {
            syscall_print_string("[kernel] Missing filename for read command...\n");
        }
    } else {
        syscall_print_string("[kernel] Unrecognized command...\nTry typing help\n");
    }
}
