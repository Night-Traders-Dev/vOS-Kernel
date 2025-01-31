#include <stdio.h>
#include "vstring.h"

void test_strlen() {
    printf("Testing v_strlen...\n");
    printf("Length of \"hello\": %zu (Expected: 5)\n", v_strlen("hello"));
    printf("Length of \"\": %zu (Expected: 0)\n", v_strlen(""));
}

void test_strcmp() {
    printf("\nTesting v_strcmp...\n");
    printf("Comparing \"hello\" and \"hello\": %d (Expected: 0)\n", v_strcmp("hello", "hello"));
    printf("Comparing \"hello\" and \"world\": %d (Expected: negative)\n", v_strcmp("hello", "world"));
    printf("Comparing \"world\" and \"hello\": %d (Expected: positive)\n", v_strcmp("world", "hello"));
}

void test_strcpy() {
    printf("\nTesting v_strcpy...\n");
    char dest[10];
    v_strcpy(dest, "test");
    printf("Copied string: %s (Expected: test)\n", dest);
}

void test_strncpy() {
    printf("\nTesting v_strncpy...\n");
    char dest[10];
    v_strncpy(dest, "hello", 3);
    dest[3] = '\0';  // Ensure null termination
    printf("Copied string: %s (Expected: hel)\n", dest);
}

void test_memcpy() {
    printf("\nTesting v_memcpy...\n");
    char src[] = "abcdef";
    char dest[10];
    v_memcpy(dest, src, 6);
    dest[6] = '\0';
    printf("Copied memory: %s (Expected: abcdef)\n", dest);
}

void test_memset() {
    printf("\nTesting v_memset...\n");
    char buffer[10];
    v_memset(buffer, 'A', 5);
    buffer[5] = '\0';
    printf("Memory set: %s (Expected: AAAAA)\n", buffer);
}

void test_v_strcat() {
    char buffer[20] = "Hello, ";
    printf("Testing v_strcat...\n");
    printf("Before: %s\n", buffer);
    v_strcat(buffer, "World!");
    printf("After: %s (Expected: Hello, World!)\n\n", buffer);
}

void test_v_strncat() {
    char buffer[20] = "Hello, ";
    
    printf("Testing v_strncat...\n");
    printf("Before: %s\n", buffer);
    
    v_strncat(buffer, "World!", 3);
    printf("After: %s (Expected: Hello, Wor)\n\n", buffer);
}


int main() {
    test_strlen();
    test_strcmp();
    test_strcpy();
    test_strncpy();
    test_memcpy();
    test_memset();
    test_v_strcat();
    test_v_strncat();
    return 0;
}
