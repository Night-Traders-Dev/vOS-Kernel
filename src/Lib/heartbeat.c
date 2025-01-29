#include <stdbool.h>
#include <stdint.h>
#include "heartbeat.h"

#define CYCLES_PER_MS (get_cpu_freq() / 1000)

volatile uint32_t tick_count = 0;

static inline uint64_t read_cntvct(void) {
    uint64_t val;
    __asm__ volatile ("mrs %0, cntvct_el0" : "=r" (val));
    return val;
}

static inline uint64_t get_cpu_freq(void) {
    uint64_t freq;
    __asm__ volatile ("mrs %0, cntfrq_el0" : "=r" (freq));
    return freq;
}

static void delay_cycles(uint64_t cycles) {
    uint64_t start = read_cntvct();
    while ((read_cntvct() - start) < cycles);
}

void setup_timer_1ms(void (*timer_ISR)(void)) {
    while (1) {
        delay_cycles(CYCLES_PER_MS);
        tick_count++;
        timer_ISR();
    }
}

void timer_ISR(void) {
    __asm__ volatile ("nop");
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
