#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Simulates a timer interrupt handler.
 */
void timer_interrupt_handler(void);

/**
 * @brief Initialize the heartbeat system.
 */
void init_heartbeat(void);

/**
 * @brief Simulates a timer setup that generates 1ms intervals.
 * 
 * @param timer_ISR The interrupt service routine to call every 1ms.
 */
void setup_timer_1ms(void (*timer_ISR)(void));

/**
 * @brief Check if 1 second has elapsed and trigger the heartbeat.
 * 
 * @return true if 1 second has elapsed, false otherwise.
 */
bool check_heartbeat(void);

#endif // HEARTBEAT_H
