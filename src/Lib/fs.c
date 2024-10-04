#include "fs.h"
#include <stdio.h>
#include "syscalls.h"
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

int snprintf(char *buffer, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *p;
    int count = 0;

    for (p = (char *)format; *p != '\0' && count < size - 1; p++) {
        if (*p != '%') {
            buffer[count++] = *p;
            continue;
        }

        p++; // Move past '%'

        switch (*p) {
            case 'd': { // Integer
                int i = va_arg(args, int);
                if (i < 0) {
                    if (count < size - 1) {
                        buffer[count++] = '-';
                    }
                    i = -i;
                }
                char digits[10];
                int digit_count = 0;
                do {
                    if (digit_count < sizeof(digits)) {
                        digits[digit_count++] = (i % 10) + '0';
                    }
                    i /= 10;
                } while (i > 0 && digit_count < sizeof(digits));
                for (int j = digit_count - 1; j >= 0 && count < size - 1; j--) {
                    buffer[count++] = digits[j];
                }
                break;
            }
            case 's': { // String
                char *s = va_arg(args, char *);
                while (*s && count < size - 1) {
                    buffer[count++] = *s++;
                }
                break;
            }
            case 'c': // Character
                if (count < size - 1) {
                    buffer[count++] = (char)va_arg(args, int);
                }
                break;
            default: // Unsupported format
                if (count < size - 1) {
                    buffer[count++] = '%';
                }
                if (count < size - 1) {
                    buffer[count++] = *p;
                }
                break;
        }
    }

    buffer[count < size ? count : size - 1] = '\0'; // Null-terminate the string
    va_end(args);
    return count;
}

int sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *p;
    int count = 0;

    for (p = (char *)format; *p != '\0'; p++) {
        if (*p != '%') {
            buffer[count++] = *p;
            continue;
        }

        p++; // Move past '%'

        switch (*p) {
            case 'd': { // Integer
                int i = va_arg(args, int);
                if (i < 0) {
                    buffer[count++] = '-';
                    i = -i;
                }
                char digits[10];
                int digit_count = 0;
                do {
                    if (digit_count < sizeof(digits)) {
                        digits[digit_count++] = (i % 10) + '0';
                    }
                    i /= 10;
                } while (i > 0 && digit_count < sizeof(digits));
                for (int j = digit_count - 1; j >= 0; j--) {
                    buffer[count++] = digits[j];
                }
                break;
            }
            case 's': { // String
                char *s = va_arg(args, char *);
                while (*s) {
                    buffer[count++] = *s++;
                }
                break;
            }
            case 'c': // Character
                buffer[count++] = (char)va_arg(args, int);
                break;
            default: // Unsupported format
                buffer[count++] = '%';
                buffer[count++] = *p;
                break;
        }
    }

    buffer[count] = '\0'; // Null-terminate the string
    va_end(args);
    return count;
}

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

void fs_ls(void) {
    syscall_print_string("[kernel] Listing files:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (filesystem[i].name[0] != '\0') { // File exists
            char info[64]; // Buffer for file info
            int written = snprintf(info, sizeof(info), "[kernel] %s - Size: %d bytes\n", filesystem[i].name, filesystem[i].size);
            if (written >= sizeof(info)) {
                syscall_print_string("[kernel] Error: Info string too long.\n");
            } else {
                syscall_print_string(info); // Print file name and size
            }
        }
    }
    syscall_print_string("[kernel] Finished listing files.\n");
}
