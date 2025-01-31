#include "vstring.h"

size_t v_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int v_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char *v_strcpy(char *dest, const char *src) {
    char *orig = dest;
    while ((*dest++ = *src++));
    return orig;
}

char *v_strncpy(char *dest, const char *src, size_t n) {
    char *orig = dest;
    while (n && (*dest++ = *src++)) {
        n--;
    }
    while (n--) {
        *dest++ = '\0';
    }
    return orig;
}

void *v_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void *v_memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

char* v_strcat(char* dest, const char* src) {
    char* ptr = dest;
    
    // Move ptr to the end of dest
    while (*ptr) {
        ptr++;
    }

    // Copy src to the end of dest
    while (*src) {
        *ptr++ = *src++;
    }

    *ptr = '\0';  // Null-terminate the result

    return dest;
}

char* v_strncat(char* dest, const char* src, size_t n) {
    char* ptr = dest;

    // Move ptr to the end of dest
    while (*ptr) {
        ptr++;
    }

    // Copy at most n characters from src
    while (*src && n--) {
        *ptr++ = *src++;
    }

    *ptr = '\0';  // Null-terminate the result

    return dest;
}
