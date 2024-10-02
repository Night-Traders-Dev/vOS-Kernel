#include "command_handler.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "kernel.h"

// Array of commands for help function
const char *commands[] = {
    "version - displays version number",
    "help - displays this menu",
    "exit - shutsdown vOS"
};

void handle_command(const char *buffer) {
    if (strcmp(buffer, "exit") == 0) {
        print_string("[shell] Exit detected...\n");
        system_off();  // Shut down
    } else if (strcmp(buffer, "version") == 0) {
        print_string("[shell] Version: 0.0.1\n");
    } else if (strcmp(buffer, "help") == 0) {
        print_string("[shell] Commands:\n");
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            print_string(commands[i]);
            print_string("\n");
        }
    } else {
        print_string("[kernel] Unrecognized command...\nTry typing help\n");
    }
}