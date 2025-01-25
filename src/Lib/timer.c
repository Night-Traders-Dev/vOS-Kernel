#include "timer.h"

uint32_t system_ticks = 0;
uint32_t SystemCoreClock = 0;


// Initialize the SysTick timer
void timer_init(void) {
    // Set the SysTick timer to interrupt every 1ms
    SYSTICK_LOAD = (SystemCoreClock / 1000) - 1;  // Set reload value for 1ms interval
    SYSTICK_VAL = 0;  // Clear the current value register
    SYSTICK_CTRL = SYSTICK_CTRL_CLKSOURCE_Msk |    // Use system clock
                   SYSTICK_CTRL_TICKINT_Msk |     // Enable interrupt
                   SYSTICK_CTRL_ENABLE_Msk;      // Enable the timer
}

// Increment system ticks
void increment_system_ticks(void) {
    system_ticks++;
}

// SysTick interrupt handler
void SysTick_Handler(void) {
    increment_system_ticks(); // Increment the global tick counter
//    scheduler();  // Call scheduler to check for task switching
}

// Timeout check based on system ticks
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks) {
    uint32_t current_tick = system_ticks;
    return (current_tick - start_tick >= timeout_ticks);
}
