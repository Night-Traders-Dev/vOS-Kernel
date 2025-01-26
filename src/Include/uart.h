#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdarg.h>
#include "vstring.h"
#include "timer.h"
#include "scheduler.h"

extern char uart_read_char(void);
extern void uart_read_string(char *buffer, int max_length, uint32_t timeout_ticks);
extern void handle_uart_input(char c);
void uart_task(void);
void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *str);
extern void uart_printf(const char *fmt, ...);
#endif // UART_H
