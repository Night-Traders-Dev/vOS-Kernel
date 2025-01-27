#include "command_handler.h"
#include <string.h>

void clear(void) {
    print_string("\033[2J\033[H");
};



// Command structure
typedef struct {
    const char *name;   // Command name
    const char *desc;   // Command description
    void (*func)(void); // Command function pointer
} Command;

// Command functions
void cmd_exit(void) {
    system_off();
}

void cmd_clear(void) {
    clear();
}

void cmd_version(void) {
    print_string("[shell] Version: 0.0.1\n");
}

void cmd_ls(void) {
    fs_ls();
}

void cmd_ps(void) {
    show_task_info();
}

// Command lookup table (now globally visible within this file)
static const Command commands[] = {
    {"exit", "shuts down vOS", cmd_exit},
    {"clear", "clears console", cmd_clear},
    {"version", "displays version number", cmd_version},
    {"ls", "shows current directory", cmd_ls},
    {"help", "displays this menu", cmd_help},
    {"ps", "shows running processes", cmd_ps},
};

// Command help function
void cmd_help(void) {
    print_string("[shell] Commands:\n");
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        print_string(commands[i].name);
        print_string(" - ");
        print_string(commands[i].desc);
        print_string("\n");
    }
}

// Handle command input
void handle_command(const char *buffer) {
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(buffer, commands[i].name) == 0) {
            commands[i].func(); // Call the associated command function
            return;
        }
    }
    print_string("Unrecognized command...\nTry typing help\n");
}
