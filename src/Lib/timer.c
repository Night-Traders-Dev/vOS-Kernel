#include "timer.h"
#include "scheduler.h"

uint32_t system_ticks = 0;
uint32_t SystemCoreClock = 0;  // Must be initialized externally

void timer_init(void) {
    if (SystemCoreClock == 0) {
        print_string("Error: SystemCoreClock not initialized. Setting default value.\n");
        SystemCoreClock = 16000000;  // Default to 16 MHz if not initialized
    }

    print_string("Timer Started\n");

    SYSTICK_LOAD = (SystemCoreClock / 1000) - 1;  // Set reload value for 1ms interval
    SYSTICK_VAL = 0;  // Clear the current value register
    SYSTICK_CTRL = SYSTICK_CTRL_CLKSOURCE_Msk |    // Use system clock
                   SYSTICK_CTRL_TICKINT_Msk |     // Enable interrupt
                   SYSTICK_CTRL_ENABLE_Msk;      // Enable the timer

    print_string("Returning\n");
    scheduler();  // Start the scheduler
}


// Increment system ticks
void increment_system_ticks(void) {
    system_ticks++;
}

// SysTick interrupt handler
void SysTick_Handler(void) {
    increment_system_ticks();  // Increment the global tick counter

    // Scheduler call for task management
    scheduler();
}

// Timeout check based on system ticks
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks) {
    uint32_t current_tick = system_ticks;
    // Account for tick counter overflow
    return (current_tick - start_tick) >= timeout_ticks;
}
