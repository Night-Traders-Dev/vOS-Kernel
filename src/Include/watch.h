#ifndef WATCH_H
#define WATCH_H

#include <stdint.h>
#include <stdbool.h>
#include "vstring.h"

// Define the maximum number of timers supported
#define MAX_TIMERS 10

// Timer structure definition
typedef struct {
    uint32_t expiration_time;  // Time at which the timer expires (in system ticks)
    void (*callback)(void);    // Function to call when the timer expires
    bool active;               // Whether the timer is currently active
} timer_t;

// Function prototypes

/**
 * @brief Initializes the timer system, preparing it to manage timers.
 */
extern void init_timers(void);

/**
 * @brief Creates a new timer with a specified delay and callback.
 * 
 * @param delay The number of system ticks before the timer expires.
 * @param callback The function to call when the timer expires.
 * @return The timer ID (index), or -1 if no timers are available.
 */
extern int create_timer(uint32_t delay, void (*callback)(void));

/**
 * @brief Deletes a timer by its ID, deactivating it.
 * 
 * @param timer_id The ID of the timer to delete.
 */
extern void delete_timer(int timer_id);

/**
 * @brief Updates all active timers. This function should be called periodically.
 */
extern void update_timers(void);

/**
 * @brief Increments the system tick counter. This function should be called periodically.
 */
extern void increment_ticks(void);

#endif // WATCH_H
