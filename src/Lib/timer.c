#include "timer.h"

uint32_t system_ticks = 0;
uint32_t SystemCoreClock = 16000000;

// Check if the scheduler is initialized
bool scheduler_initialized(void) {
    return (task_count > 0);
}

// Initialize the timer
bool timer_init(void) {
    // Register the timer interrupt with the GIC
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

    // Enable the custom timer and configure it for periodic mode with interrupt
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

    if ((TIMER_CTRL & TIMER_CTRL_ENABLE_Msk) != 0) {
//        scheduler_initialized();
        print_string("Custom Timer Configured Successfully\n");
    }
    return true;
}

// Increment the system tick counter
void increment_system_ticks(void) {
    system_ticks++;
}

void gic_register_timer_interrupt(void) {
    uint64_t timer_interrupt_id = 30;  // Timer interrupt ID (verify with GIC configuration)
    gic_enable_interrupt(timer_interrupt_id);
    gic_set_priority(timer_interrupt_id, 0x20);  // Example priority
    gic_set_target(timer_interrupt_id, 0x01);  // Target CPU 0 (for single-core)
    gic_set_config(timer_interrupt_id, 0x2);  // 0x2 means edge-triggered
    gic_init();
    print_string("Timer interrupt registered and configured.\n");
}

// Timer interrupt handler
void Timer_Handler(void) {
    increment_system_ticks();  // Increment global tick counter
}

// Check if a timeout has occurred
bool timeout_occurred(uint32_t start_tick, uint32_t timeout_ticks) {
    uint32_t current_tick = system_ticks;
    // Account for tick counter overflow
    return (current_tick - start_tick) >= timeout_ticks;
}
