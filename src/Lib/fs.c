#include "fs.h"
#include <string.h>

// Minimal implementation of strcpy
char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++)) {}
    return ret;
}

// Minimal implementation of memcpy
void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

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
