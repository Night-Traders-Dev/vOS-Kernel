#include <stdint.h>

#define MODE_ADDRESS 0x20000000
#define ARM_KERNEL_ENTRY  0x10010000
#define RISCV_KERNEL_ENTRY 0x20010000

void start_arm_kernel(void);
void start_riscv_kernel(void);

void hybrid_boot(void) {
    volatile uint32_t* mode = (uint32_t*)MODE_ADDRESS;
    *mode = 0x03;  // Set mode to hybrid

    start_arm_kernel();
    start_riscv_kernel();
}

void start_arm_kernel(void) {
    void (*kernel_entry)(void) = (void (*)(void)) ARM_KERNEL_ENTRY;
    kernel_entry();
}

void start_riscv_kernel(void) {
    void (*kernel_entry)(void) = (void (*)(void)) RISCV_KERNEL_ENTRY;
    kernel_entry();
}
