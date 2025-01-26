#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "vstring.h"
#include "uart.h"
#include "kernel.h"
#include "scheduler.h"

void handle_command(const char *buffer);
#endif
