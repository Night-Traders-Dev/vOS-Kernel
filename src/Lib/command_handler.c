#include "command_handler.h"

// Array of commands for help function
const char *commands[] = {
    "clear - clears console",
    "ls - shows current directory",
    "version - displays version number",
    "help - displays this menu",
    "exit - shutsdown vOS"
};

void handle_command(const char *buffer) {
    if (strcmp(buffer, "exit") == 0) {
        return;
    } else if (strcmp(buffer, "clear") == 0) {
        print_string("\033[2J\033[H");
    } else if (strcmp(buffer, "version") == 0) {
        print_string("[shell] Version: 0.0.1\n");
    } else if (strcmp(buffer, "ls") == 0) {
        return;
//        fs_ls();
    } else if (strcmp(buffer, "help") == 0) {
        print_string("[shell] Commands:\n");
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            print_string(commands[i]);
            print_string("\n");
        }
    } else {
        print_string("Unrecognized command...\nTry typing help\n");
    }
}
