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

char* v_strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;  // Return NULL if character is not found
}

char* v_strrchr(const char* str, int c) {
    const char* last_occurrence = NULL;

    while (*str) {
        if (*str == (char)c) {
            last_occurrence = str; // Update last occurrence
        }
        str++;
    }
    
    return (char*)last_occurrence;
}


char* v_strstr(const char* haystack, const char* needle) {
    if (!*needle) {
        return (char*)haystack;  // Return haystack if needle is empty
    }

    while (*haystack) {
        const char* h = haystack;
        const char* n = needle;

        while (*h && *n && (*h == *n)) {
            h++;
            n++;
        }

        if (!*n) {
            return (char*)haystack;  // Found match
        }

        haystack++;
    }

    return NULL;  // No match found
}

char* v_strtok(char* str, const char* delim) {
    static char* next_token = NULL;

    if (str) {
        next_token = str;
    }

    if (!next_token) {
        return NULL;
    }

    // Skip leading delimiters
    while (*next_token && v_strchr(delim, *next_token)) {
        next_token++;
    }

    if (!*next_token) {
        return NULL;
    }

    char* token_start = next_token;

    // Find the end of the token
    while (*next_token && !v_strchr(delim, *next_token)) {
        next_token++;
    }

    if (*next_token) {
        *next_token = '\0';
        next_token++;
    } else {
        next_token = NULL;
    }

    return token_start;
}

int v_atoi(const char* str) {
    int result = 0;
    int sign = 1;

    // Skip leading whitespace
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') {
        str++;
    }

    // Handle optional sign
    if (*str == '-' || *str == '+') {
        if (*str == '-') {
            sign = -1;
        }
        str++;
    }

    // Convert characters to integer
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return result * sign;
}

void v_itoa(int num, char* str) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Convert integer to string (in reverse order)
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Add negative sign if necessary
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
