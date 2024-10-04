#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <stddef.h>
#include <stdint.h>

void handle_command(const char *buffer);

// Custom string functions
size_t my_strlen(const char *str);
void my_strcpy(char *dest, const char *src);
int my_sscanf(const char *str, const char *format, ...);

#endif // COMMAND_HANDLER_H
