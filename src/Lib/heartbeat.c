#include <stdint.h>
#include "heartbeat.h"

#define CPU_CLOCK_HZ 48000000
#define CYCLES_PER_MS (CPU_CLOCK_HZ / 1000)
volatile uint32_t tick_count = 0;
static void delay_cycles(uint32_t cycles);

void setup_timer_1ms(void (*timer_ISR)(void)) {
    while (1) {
        delay_cycles(CYCLES_PER_MS); // Wait for 1ms
        timer_ISR();                // Call the user-defined ISR
    }
}

static void delay_cycles(uint32_t cycles) {
    uint32_t start, current;

    // Read the cycle counter register at the start
    __asm__ volatile ("mrs %0, cntvct_el0" : "=r" (start));

    do {
        // Read the current cycle counter value
        __asm__ volatile ("mrs %0, cntvct_el0" : "=r" (current));
    } while ((current - start) < cycles);
}

void timer_interrupt_handler(void) {
    tick_count++;
}

void init_heartbeat(void) {
    tick_count = 0;
}

bool check_heartbeat(void) {
    static uint32_t last_tick = 0;

    if ((tick_count - last_tick) >= 1000) {
        last_tick += 1000;
        return true;
    }

    return false;
}
