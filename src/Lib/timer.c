#include "timer.h"
#include "scheduler.h"

uint32_t system_ticks = 0;
uint32_t SystemCoreClock = 0;  // Must be initialized externally

bool scheduler_initialized(void) {
    return (task_count > 0);
}

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

    // Fault detection for SysTick configuration
    if ((SYSTICK_CTRL & SYSTICK_CTRL_ENABLE_Msk) == 0) {
        print_string("Error: Failed to enable SysTick timer.\n");
        return;  // Exit to prevent further issues
    }

    if (SYSTICK_LOAD == 0) {
        print_string("Error: SysTick reload value is 0. Timer cannot function.\n");
        return;  // Exit as the timer will not generate interrupts
    }

    print_string("SysTick Timer Configured Successfully\n");

    if (!scheduler_initialized()) {
        print_string("Error: Scheduler initialization failed. Halting.\n");
        while (1) {
            print_string("System Hanging\n");
        }
    }

    print_string("Returning\n");

    // Start the scheduler
    scheduler();
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
