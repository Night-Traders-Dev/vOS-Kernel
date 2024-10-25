#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Raft constants
#define ELECTION_TIMEOUT 5000 // 5 seconds
#define HEARTBEAT_INTERVAL 1000 // 1 second
#define MAX_LOG_ENTRIES 100
#define NODE_ID 1 // Unique ID for this node
#define TOTAL_NODES 3 // Total number of nodes in the system

// Raft states
typedef enum {
    FOLLOWER,
    CANDIDATE,
    LEADER
} raft_state_t;

// Raft log entry structure
typedef struct {
    uint32_t term;
    char data[32];  // Simplified transaction data
} raft_log_entry_t;

// Raft node structure
typedef struct {
    raft_state_t state;
    uint32_t current_term;
    int voted_for;
    uint32_t commit_index;
    raft_log_entry_t log[MAX_LOG_ENTRIES];
    uint32_t last_applied;
    uint32_t last_log_index;
    uint32_t votes_received;
} raft_node_t;

// Function prototypes
void start_election(raft_node_t *node);
void send_heartbeat(raft_node_t *node);
void apply_log(raft_node_t *node, raft_log_entry_t *entry);
void uart_send(const char *message);
void uart_receive(char *buffer, size_t len);
void handle_vote_request(raft_node_t *node, uint32_t candidate_term);
void handle_log_replication(raft_node_t *node, raft_log_entry_t *entry);
void handle_heartbeat(raft_node_t *node);

raft_node_t raft_node;

// UART setup for communication
void setup_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, false);
}

// Raft state initialization
void raft_init(raft_node_t *node) {
    node->state = FOLLOWER;
    node->current_term = 0;
    node->voted_for = -1;
    node->commit_index = 0;
    node->last_applied = 0;
    node->last_log_index = 0;
    node->votes_received = 0;
}

// Start a new election (candidate role)
void start_election(raft_node_t *node) {
    node->state = CANDIDATE;
    node->current_term += 1;
    node->voted_for = NODE_ID; // Vote for self
    node->votes_received = 1;  // Self-vote

    printf("Node %d starting election for term %d\n", NODE_ID, node->current_term);
    
    // Request votes from other nodes via UART (placeholder)
    char vote_request[32];
    sprintf(vote_request, "REQUEST_VOTE %d %d", NODE_ID, node->current_term);
    uart_send(vote_request);
}

// Send heartbeat to followers (leader role)
void send_heartbeat(raft_node_t *node) {
    if (node->state == LEADER) {
        uart_send("HEARTBEAT");
        printf("Node %d (leader) sending heartbeat\n", NODE_ID);
    }
}

// Apply a log entry (commit to blockchain)
void apply_log(raft_node_t *node, raft_log_entry_t *entry) {
    if (node->last_log_index < MAX_LOG_ENTRIES) {
        node->log[node->last_log_index] = *entry;
        node->last_log_index++;
        node->commit_index = node->last_log_index;
        printf("Log entry applied: term %d, data %s\n", entry->term, entry->data);
    } else {
        printf("Log is full!\n");
    }
}

// Handle vote request (follower role)
void handle_vote_request(raft_node_t *node, uint32_t candidate_term) {
    if (node->state == FOLLOWER && candidate_term > node->current_term) {
        node->current_term = candidate_term;
        node->voted_for = 2; // For example, voting for candidate with ID 2
        uart_send("VOTE_GRANTED");
        printf("Node %d voted for candidate %d in term %d\n", NODE_ID, 2, candidate_term);
    }
}

// Handle log replication (follower role)
void handle_log_replication(raft_node_t *node, raft_log_entry_t *entry) {
    apply_log(node, entry);
    uart_send("LOG_ACK");
}

// Handle heartbeat from the leader (follower role)
void handle_heartbeat(raft_node_t *node) {
    if (node->state == FOLLOWER) {
        printf("Node %d (follower) received heartbeat\n", NODE_ID);
    }
}

// Placeholder UART communication (sending and receiving messages)
void uart_send(const char *message) {
    uart_puts(UART_ID, message);
}

void uart_receive(char *buffer, size_t len) {
    uart_read_blocking(UART_ID, (uint8_t *)buffer, len);
}

int main() {
    // Initialize UART and Raft node
    stdio_init_all();
    setup_uart();
    raft_init(&raft_node);

    // Main event loop
    uint64_t last_heartbeat_time = 0;
    uint64_t last_election_time = 0;
    
    while (true) {
        uint64_t current_time = time_us_64() / 1000; // Get current time in ms

        if (raft_node.state == LEADER) {
            if (current_time - last_heartbeat_time > HEARTBEAT_INTERVAL) {
                send_heartbeat(&raft_node);
                last_heartbeat_time = current_time;
            }
        } else if (raft_node.state == FOLLOWER) {
            if (current_time - last_election_time > ELECTION_TIMEOUT) {
                start_election(&raft_node);
                last_election_time = current_time;
            }
        }

        // Receive and handle messages
        char buffer[32];
        uart_receive(buffer, sizeof(buffer));
        
        if (strncmp(buffer, "REQUEST_VOTE", 12) == 0) {
            uint32_t candidate_term;
            sscanf(buffer, "REQUEST_VOTE %*d %d", &candidate_term);
            handle_vote_request(&raft_node, candidate_term);
        } else if (strncmp(buffer, "LOG_ENTRY", 9) == 0) {
            raft_log_entry_t entry;
            sscanf(buffer, "LOG_ENTRY %d %s", &entry.term, entry.data);
            handle_log_replication(&raft_node, &entry);
        } else if (strncmp(buffer, "HEARTBEAT", 9) == 0) {
            handle_heartbeat(&raft_node);
        }

        sleep_ms(100); // Simulate small delay for responsiveness
    }

    return 0;
}
