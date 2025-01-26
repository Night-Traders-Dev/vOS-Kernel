#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "scheduler.h"
#include "kernel.h"
#include "gic.h"

// Define timer-specific constants
#define TIMER_BASE 0x40000000  // Example base address for the custom timer
#define TIMER_LOAD  (*(volatile uint32_t *)(TIMER_BASE + 0x00))  // Load value register
#define TIMER_VAL   (*(volatile uint32_t *)(TIMER_BASE + 0x04))  // Current value register
#define TIMER_CTRL  (*(volatile uint32_t *)(TIMER_BASE + 0x08))  // Control register

// Bit fields for the custom timer control register
#define TIMER_CTRL_ENABLE_Msk      (1U << 0)  // Enable the timer
#define TIMER_CTRL_MODE_Msk        (1U << 1)  // Mode selection (e.g., periodic or one-shot)
#define TIMER_CTRL_INTEN_Msk       (1U << 2)  // Enable timer interrupt

// Timeout threshold for generic usage (adjust as needed)
#define TIMEOUT_THRESHOLD_MS 5000  // Timeout in milliseconds

// Timer state macros
#define TIMER_IS_RUNNING()         (TIMER_CTRL & TIMER_CTRL_ENABLE_Msk)  // Check if timer is running
#define TIMER_SET_MODE_PERIODIC()  (TIMER_CTRL |= TIMER_CTRL_MODE_Msk)   // Set timer to periodic mode
#define TIMER_SET_MODE_ONESHOT()   (TIMER_CTRL &= ~TIMER_CTRL_MODE_Msk)  // Set timer to one-shot mode

// Function prototypes
bool timer_init(void);                          // Initialize the custom timer
void increment_system_ticks(void);              // Increment the global system tick counter
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks);  // Check for timeout
__attribute__((weak)) void Timer_Handler(void); // Custom timer interrupt handler (weak for override)

// GIC-specific setup for the timer interrupt
void gic_register_timer_interrupt(void);       // Register timer interrupt in GIC

// Macro for error checking
#define IS_CLOCK_INITIALIZED() (SystemCoreClock != 0)  // Check if SystemCoreClock is initialized

// External variables
extern uint32_t system_ticks;     // Global system tick counter
extern uint32_t SystemCoreClock;  // System clock frequency (in Hz). Must be initialized before using the timer.

#endif // TIMER_H
