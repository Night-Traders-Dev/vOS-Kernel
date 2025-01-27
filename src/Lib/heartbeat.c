#include <stdbool.h>
#include <stdint.h>
#include "heartbeat.h"

#define CPU_CLOCK_HZ 48000000
#define CYCLES_PER_MS (CPU_CLOCK_HZ / 1000)
volatile uint32_t tick_count = 0;
static void delay_cycles(uint32_t cycles);



void setup_timer_1ms(void (*timer_ISR)(void)) {
    // Disable interrupts
//    __asm__ volatile ("cpsid i");

    // Use a simple software loop for a 1ms timer
    while (1) {
        for (uint32_t i = 0; i < 100000; i++) {
            __asm__ volatile ("nop"); // Calibrated for 1ms
        }
        tick_count++;
        timer_ISR();
    }

    // Enable interrupts
//    __asm__ volatile ("cpsie i");
}

void timer_ISR(void) {
    __asm__ volatile ("nop");
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
