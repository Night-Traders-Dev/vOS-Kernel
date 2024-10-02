#include "command_handler.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "kernel.h"

void handle_command(const char *buffer) {
    if (strcmp(buffer, "exit") == 0) {
        print_string("[shell] Exit detected...\n");
        system_off();  // Shut down
    } else if (strcmp(buffer, "version") == 0) {
        print_string("[shell] Version: 0.0.1\n");
    } else {
        print_string("[kernel] Unrecognized command...\n");
    }
}
