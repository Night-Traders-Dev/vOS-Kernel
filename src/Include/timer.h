#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

// Define the base address for SysTick registers
#define SYSTICK_BASE 0xE000E010
#define SYSTICK_LOAD  (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))  // Load value register
#define SYSTICK_VAL   (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))  // Current value register
#define SYSTICK_CTRL  (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))  // Control and status register

// Define bit fields for the SysTick control register
#define SYSTICK_CTRL_CLKSOURCE_Msk   (1 << 2)  // Select the clock source (external or system)
#define SYSTICK_CTRL_TICKINT_Msk     (1 << 1)  // Enable interrupt
#define SYSTICK_CTRL_ENABLE_Msk      (1 << 0)  // Enable the timer

#define TIMEOUT_THRESHOLD 5000  // Example timeout threshold in milliseconds

// Function prototypes
extern void timer_init(void);                 // Initialize the timer
extern void increment_system_ticks(void);     // Increment the global system tick counter
extern bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks); // Timeout check
__attribute__((weak)) void SysTick_Handler(void);  // SysTick interrupt handler (weak to allow override)

// Macros for error checking
#define IS_CLOCK_INITIALIZED() (SystemCoreClock != 0)  // Check if SystemCoreClock is initialized

// External variables
extern uint32_t system_ticks;     // System ticks counter
extern uint32_t SystemCoreClock;  // System clock frequency (in Hz). Must be initialized before using the timer.

#endif // TIMER_H
