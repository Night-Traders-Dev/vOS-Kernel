#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Handles timer interrupts, updating the tick count.
 */
void timer_interrupt_handler(void);

/**
 * @brief Initializes the heartbeat system.
 * Resets the tick count and prepares the system timer.
 */
void init_heartbeat(void);

/**
 * @brief Sets up a 1ms timer using the ARM64 system timer.
 * Uses the ARM generic timer (`cntvct_el0`) for accurate timing.
 *
 * @param timer_ISR The interrupt service routine to call every 1ms.
 */
void setup_timer_1ms(void (*timer_ISR)(void));

/**
 * @brief Checks if 1 second has elapsed since the last heartbeat.
 *
 * @return true if 1 second has passed, false otherwise.
 */
bool check_heartbeat(void);

/**
 * @brief Reads the ARM system counter value (cntvct_el0).
 *
 * @return The current counter value.
 */
static inline uint64_t read_cntvct(void);

/**
 * @brief Retrieves the CPU frequency from cntfrq_el0.
 *
 * @return The CPU frequency in Hz.
 */
static inline uint64_t get_cpu_freq(void);

/**
 * @brief Delays execution for a given number of CPU cycles.
 *
 * @param cycles The number of cycles to wait.
 */
static void delay_cycles(uint64_t cycles);

#endif // HEARTBEAT_H
