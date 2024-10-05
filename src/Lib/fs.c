#include "fs.h"
#include "syscalls.h"
#include "vstring.h"

// fs opcodes
// 0 = directory
// 1 = file
//
//

static File filesystem[MAX_FILES];

void fs_init(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        filesystem[i].size = 0;
        filesystem[i].is_open = 0;
        strcpy(filesystem[i].name, "");
    }
}


int fs_mkdir(const char *dirname) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (filesystem[i].name[0] == '\0') {
            strcpy(filesystem[i].name, dirname);
            filesystem[i].size = 0;
            filesystem[i].is_open = 1;
            filesystem[i].type = 0; // 0 for directory
            return 0;
        }
    }
    return -1; // No space left
}

int fs_dir_size(const char *dirname, int new_size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(filesystem[i].name, dirname) == 0 && filesystem[i].type == 0) { // Ensure it's a directory
            filesystem[i].size = new_size;
            return 0; // Success
        }
    }
    return -1; // Directory not found
}

int fs_create(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (filesystem[i].name[0] == '\0') {
            strcpy(filesystem[i].name, filename);
            filesystem[i].size = 0;
            filesystem[i].is_open = 1;
            filesystem[i].type = 1; // 1 for file
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
    syscall_print_string("[shell] Listing files and directories:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (filesystem[i].name[0] != '\0') {
            syscall_print_string(filesystem[i].name);
            syscall_print_string(" - ");
            char buffer[128];
            syscall_print_string(int_to_string(filesystem[i].size, buffer));
            syscall_print_string(" bytes");
            syscall_print_string(filesystem[i].type == 0 ? " [DIR]\n" : " [FILE]\n");
        }
    }
}

int fs_cat(const char *filename) {
    char buffer[128];
    fs_read(filename, buffer, sizeof(buffer));
    syscall_print_string(filename);
    syscall_print_string(":\n");
    syscall_print_string(buffer);
    syscall_print_string("\n");
}
