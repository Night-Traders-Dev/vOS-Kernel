#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define MODE_ADDRESS 0x20000000

void jump_to_address(uint32_t address);

int main(void) {
    stdio_init_all();
    
    // Initialize UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    char buffer[16];
    bool boot_mode_selected = false;
    printf("Waiting for boot mode selection (arm, riscv, hybrid)\n");

    while (!boot_mode_selected) {
        if (uart_is_readable(UART_ID)) {
            int len = uart_read_blocking(UART_ID, (uint8_t*)buffer, sizeof(buffer) - 1);
            buffer[len] = '\0';

            printf("Received boot mode: %s\n", buffer);
            volatile uint32_t* mode = (uint32_t*)MODE_ADDRESS;

            if (strcmp(buffer, "arm") == 0) {
                *mode = 0x01;  // ARM mode
                boot_mode_selected = true;
                jump_to_address(0x10008000);  // ARM second-stage bootloader
            } else if (strcmp(buffer, "riscv") == 0) {
                *mode = 0x02;  // RISC-V mode
                boot_mode_selected = true;
                jump_to_address(0x20008000);  // RISC-V second-stage bootloader
            } else if (strcmp(buffer, "hybrid") == 0) {
                *mode = 0x03;  // Hybrid mode
                boot_mode_selected = true;
                jump_to_address(0x30008000);  // Hybrid second-stage bootloader
            } else {
                printf("Invalid boot mode. Please enter 'arm', 'riscv', or 'hybrid'.\n");
            }
        }
    }

    return 0;
}

void jump_to_address(uint32_t address) {
    void (*boot_function)(void) = (void (*)(void))address;
    boot_function();  // Jump to the selected address
}
