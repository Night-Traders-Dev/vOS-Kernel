#include "watch.h"


// Global array of active timers
timer_t timers[MAX_TIMERS];

// Global system tick counter
volatile uint32_t os_ticks = 0;

// Function to initialize the timer system (no hardware required)
void init_timers(void) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        timers[i].active = false;
    }
}

// Function to create a timer
int create_timer(uint32_t delay, void (*callback)(void)) {
    // Find an inactive timer slot
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!timers[i].active) {
            timers[i].expiration_time = os_ticks + delay;
            timers[i].callback = callback;
            timers[i].active = true;
            return i;  // Return the index of the created timer
        }
    }
    return -1;  // No available timer slots
}

// Function to delete a timer
void delete_timer(int timer_id) {
    if (timer_id >= 0 && timer_id < MAX_TIMERS) {
        timers[timer_id].active = false;  // Mark the timer as inactive
    }
}

// Function to update timers (to be called periodically)
void update_timers(void) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].active && os_ticks >= timers[i].expiration_time) {
            // Timer has expired, call the callback function
            timers[i].callback();

            // Deactivate the timer after it triggers
            timers[i].active = false;
        }
    }
}

// Function to increment the system tick (to be called on a periodic basis, e.g., every 1ms)
void increment_ticks(void) {
    os_ticks++;
}

// Example callback function for a timer
void timer_expired_callback(void) {
    // Do something when the timer expires
    print_string("Timer expired!\n");
}

