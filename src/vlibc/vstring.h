#ifndef VSTRING_H
#define VSTRING_H

#include <stddef.h>

size_t v_strlen(const char *str);
int v_strcmp(const char *s1, const char *s2);
char *v_strcpy(char *dest, const char *src);
char *v_strncpy(char *dest, const char *src, size_t n);
void *v_memcpy(void *dest, const void *src, size_t n);
void *v_memset(void *s, int c, size_t n);
char* v_strcat(char* dest, const char* src);
char* v_strncat(char* dest, const char* src, size_t n);
char* v_strchr(const char* str, int c);
char* v_strrchr(const char* str, int c);

#endif // VSTRING_H
