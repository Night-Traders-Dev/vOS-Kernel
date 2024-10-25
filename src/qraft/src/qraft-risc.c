#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

// Communication constants
#define MAX_MESSAGE_SIZE 32
#define HEARTBEAT_TIMEOUT 6000  // 6 seconds for heartbeat timeout

// Function prototypes
void uart_send(const char *message);
void uart_receive(char *buffer, size_t len);
void relay_message_to_arm(const char *message);
void check_heartbeat(uint64_t *last_heartbeat_time);

// UART setup for communication
void setup_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, false);
}

// Send UART message
void uart_send(const char *message) {
    uart_puts(UART_ID, message);
}

// Receive UART message
void uart_receive(char *buffer, size_t len) {
    uart_read_blocking(UART_ID, (uint8_t *)buffer, len);
}

// Relay messages to the ARM board
void relay_message_to_arm(const char *message) {
    uart_puts(uart0, message);  // Forwarding message to ARM
    printf("Relaying message to ARM: %s\n", message);
}

// Monitor heartbeat from leader
void check_heartbeat(uint64_t *last_heartbeat_time) {
    uint64_t current_time = time_us_64() / 1000;  // Current time in ms

    if (current_time - *last_heartbeat_time > HEARTBEAT_TIMEOUT) {
        // Heartbeat timeout - leader is considered down, request ARM to start election
        printf("Heartbeat timeout. Requesting ARM to start leader election.\n");
        relay_message_to_arm("START_ELECTION");
        *last_heartbeat_time = current_time;  // Reset the timer
    }
}

int main() {
    // Initialize UART
    stdio_init_all();
    setup_uart();

    char buffer[MAX_MESSAGE_SIZE];
    uint64_t last_heartbeat_time = time_us_64() / 1000;  // Initialize heartbeat time

    while (true) {
        // Receive messages from other nodes
        uart_receive(buffer, sizeof(buffer));

        if (strncmp(buffer, "HEARTBEAT", 9) == 0) {
            // If heartbeat received, reset the timer
            last_heartbeat_time = time_us_64() / 1000;
            printf("Received heartbeat, resetting timer.\n");

        } else if (strncmp(buffer, "REQUEST_VOTE", 12) == 0) {
            // Forward vote requests to ARM board
            printf("Received vote request, relaying to ARM.\n");
            relay_message_to_arm(buffer);

        } else if (strncmp(buffer, "LOG_ENTRY", 9) == 0) {
            // Forward log entries to ARM for replication
            printf("Received log entry, relaying to ARM.\n");
            relay_message_to_arm(buffer);

        }

        // Check for heartbeat timeout
        check_heartbeat(&last_heartbeat_time);

        sleep_ms(100);  // Delay for responsiveness
    }

    return 0;
}
