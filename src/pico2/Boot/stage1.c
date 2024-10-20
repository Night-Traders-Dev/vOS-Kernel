#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Define UART settings
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Memory locations for second-stage bootloaders or kernels
#define ARM_BOOT_ADDRESS 0x10000000 // Example memory address for ARM
#define RISCV_BOOT_ADDRESS 0x20000000 // Example memory address for RISC-V
#define HYBRID_BOOT_ADDRESS 0x30000000 // Example for hybrid

void jump_to_address(uint32_t address);

int main(void) {
    stdio_init_all();

    // Initialize UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    char buffer[16];
    bool boot_mode_selected = false;

    printf("Bootloader: Waiting for boot mode selection (arm, riscv, hybrid)\n");

    while (!boot_mode_selected) {
        if (uart_is_readable(UART_ID)) {
            // Read the boot mode from UART
            int len = uart_read_blocking(UART_ID, (uint8_t*)buffer, sizeof(buffer) - 1);
            buffer[len] = '\0'; // Null-terminate the string

            printf("Received boot mode: %s\n", buffer);

            if (strcmp(buffer, "arm") == 0) {
                printf("Booting into ARM mode...\n");
                boot_mode_selected = true;
                jump_to_address(ARM_BOOT_ADDRESS); // Jump to ARM second-stage
            } else if (strcmp(buffer, "riscv") == 0) {
                printf("Booting into RISC-V mode...\n");
                boot_mode_selected = true;
                jump_to_address(RISCV_BOOT_ADDRESS); // Jump to RISC-V second-stage
            } else if (strcmp(buffer, "hybrid") == 0) {
                printf("Booting into Hybrid mode...\n");
                boot_mode_selected = true;
                jump_to_address(HYBRID_BOOT_ADDRESS); // Jump to hybrid second-stage
            } else {
                printf("Invalid boot mode. Please enter 'arm', 'riscv', or 'hybrid'.\n");
            }
        }
    }

    return 0;
}

// Simple function to jump to the second stage (ARM/RISC-V/Hybrid)
void jump_to_address(uint32_t address) {
    void (*boot_function)(void) = (void (*)(void))address;
    boot_function(); // Jump to the selected address
}
