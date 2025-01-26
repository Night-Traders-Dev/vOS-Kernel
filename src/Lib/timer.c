#include "timer.h"

uint32_t system_ticks = 0;
uint32_t SystemCoreClock = 0;

bool scheduler_initialized(void) {
    return (task_count > 0);  // Check if tasks are initialized
}

bool timer_init(void) {
    // Register the timer interrupt with the GIC once, before timer configuration
    gic_register_timer_interrupt();
    // Ensure the system clock is initialized
    if (SystemCoreClock == 0) {
        print_string("Error: SystemCoreClock not initialized. Setting default value.\n");
        SystemCoreClock = 16000000;  // Default to 16 MHz
    }

    char buffer[32];
    int_to_string(SystemCoreClock, buffer);
    print_string("SystemCoreClock: ");
    print_string(buffer);
    print_string("\n");

    // Configure the custom timer for a 1ms interval
    TIMER_LOAD = (SystemCoreClock / 1000) - 1;  // Set reload value
    print_string("Configuring TIMER_LOAD\n");

    TIMER_VAL = 0;  // Clear the current value register
    print_string("Clearing TIMER_VAL\n");

    // Enable the custom timer and configure for periodic mode with interrupt
    TIMER_CTRL = TIMER_CTRL_ENABLE_Msk |
                 TIMER_CTRL_MODE_Msk |
                 TIMER_CTRL_INTEN_Msk;

    print_string("Enabling TIMER_CTRL\n");

    // Verify if the timer is enabled
    if ((TIMER_CTRL & TIMER_CTRL_ENABLE_Msk) == 0) {
        print_string("Error: Failed to enable timer.\n");
        return false;
    }

    if (TIMER_LOAD == 0) {
        print_string("Error: TIMER_LOAD value is 0. Timer cannot function.\n");
        return false;
    }

    print_string("Custom Timer Configured Successfully\n");

    // Check if the scheduler is initialized
    if (!scheduler_initialized()) {
        print_string("Error: Scheduler initialization failed. Halting.\n");
        while (1) {
            system_off();
        }
    }

    if (TIMER_CTRL & TIMER_CTRL_ENABLE_Msk) {
        return true;  // Timer initialized successfully
    } else {
        return false;  // Timer initialization failed
    }
}


// Increment system ticks
void increment_system_ticks(void) {
    system_ticks++;
}

// GIC-specific registration for the timer interrupt
void gic_register_timer_interrupt(void) {
    // Timer interrupt is assumed to be interrupt ID 30
    gic_enable_interrupt(30);            // Enable the interrupt in the GIC
    gic_set_priority(30, 0x20);          // Set priority for the timer interrupt (optional)
    gic_set_target(30, 0x01);            // Set target CPU (assuming single-core)
    gic_set_config(30, 2);               // Set the interrupt to edge-triggered (optional)
}

// Timer interrupt handler
void Timer_Handler(void) {
    increment_system_ticks();  // Increment the global tick counter

    // Call the scheduler to manage tasks
    scheduler();
}

// Timeout check based on system ticks
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks) {
    uint32_t current_tick = system_ticks;
    // Account for tick counter overflow
    return (current_tick - start_tick) >= timeout_ticks;
}
