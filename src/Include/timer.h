#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "scheduler.h"
#include "kernel.h"
#include "gic.h"

// Timer Base Address and Registers
#define TIMER_BASE 0x40000000  // Example base address for the custom timer
#define TIMER_LOAD  (*(volatile uint32_t *)(TIMER_BASE + 0x00))  // Load value register
#define TIMER_VAL   (*(volatile uint32_t *)(TIMER_BASE + 0x04))  // Current value register
#define TIMER_CTRL  (*(volatile uint32_t *)(TIMER_BASE + 0x08))  // Control register

// Timer Control Register Bit Fields
#define TIMER_CTRL_ENABLE_Msk      (1U << 0)  // Enable the timer
#define TIMER_CTRL_MODE_Msk        (1U << 1)  // Timer mode (periodic or one-shot)
#define TIMER_CTRL_INTEN_Msk       (1U << 2)  // Enable timer interrupt

// Timer State Macros
#define TIMER_IS_RUNNING()         (TIMER_CTRL & TIMER_CTRL_ENABLE_Msk)  // Check if the timer is running
#define TIMER_SET_MODE_PERIODIC()  (TIMER_CTRL |= TIMER_CTRL_MODE_Msk)   // Set timer to periodic mode
#define TIMER_SET_MODE_ONESHOT()   (TIMER_CTRL &= ~TIMER_CTRL_MODE_Msk)  // Set timer to one-shot mode

// Default Timeout Threshold
#define TIMEOUT_THRESHOLD_MS 5000  // Default timeout in milliseconds

// System Clock Macro
#define IS_CLOCK_INITIALIZED() (SystemCoreClock != 0)  // Check if SystemCoreClock is initialized

// Function Prototypes
extern bool timer_init(void);                            // Initialize the custom timer
extern void increment_system_ticks(void);                // Increment the global system tick counter
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks);  // Check for timeout
__attribute__((weak)) void Timer_Handler(void);   // Timer interrupt handler (weak for override)

// GIC-Specific Function
extern void gic_register_timer_interrupt(void);          // Register timer interrupt in the GIC

// External Variables
extern uint32_t system_ticks;     // Global system tick counter
extern uint32_t SystemCoreClock;  // System clock frequency (in Hz). Must be initialized before using the timer.

#endif // TIMER_H
