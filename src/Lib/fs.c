#include "fs.h"
#include "syscalls.h"
#include "vstring.h"


static File filesystem[MAX_FILES];

void fs_init(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        filesystem[i].size = 0;
        filesystem[i].is_open = 0;
        strcpy(filesystem[i].name, "");
    }
}

int fs_create(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (filesystem[i].name[0] == '\0') {
            strcpy(filesystem[i].name, filename);
            filesystem[i].size = 0;
            filesystem[i].is_open = 1;
            return 0;
        }
    }
    return -1; // No space left
}

int fs_write(const char *filename, const char *data, int size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(filesystem[i].name, filename) == 0 && filesystem[i].is_open) {
            if (size > MAX_FILE_SIZE) size = MAX_FILE_SIZE;
            memcpy(filesystem[i].data, data, size);
            filesystem[i].size = size;
            return size;
        }
    }
    return -1; // File not found or not open
}

int fs_read(const char *filename, char *buffer, int size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(filesystem[i].name, filename) == 0) {
            if (size > filesystem[i].size) size = filesystem[i].size;
            memcpy(buffer, filesystem[i].data, size);
            return size;
        }
    }
    return -1; // File not found
}

void fs_ls(void) {
    syscall_print_string("[kernel] Listing files:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (filesystem[i].name[0] != '\0') { // Check if file exists
            char info[128]; // Buffer for file info
            int written = snprintf(info, sizeof(info), "[kernel] %s - Size: %d bytes\n", filesystem[i].name, filesystem[i].size);
            if (written < 0 || written >= sizeof(info)) {
                syscall_print_string("[kernel] Error: Info string too long or failed to write.\n");
            } else {
                syscall_print_string(info); // Print file name and size
            }
        }
    }
    syscall_print_string("[kernel] Finished listing files.\n");
}


int fs_cat(const char *filename) {
    char buffer[128];
    fs_read(filename, buffer, sizeof(buffer));
    syscall_print_string(filename);
    syscall_print_string("\n");
    syscall_print_string(buffer);
    syscall_print_string("\n");
}
