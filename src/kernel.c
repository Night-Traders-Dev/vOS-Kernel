#include <stdint.h>

#define UART_BASE 0x09000000    // UART base address for QEMU's virt machine

// Function prototypes
void print_string(const char *str);
void kernel_entry(void);
void sync_handler(void);
void irq_handler(void);
void fiq_handler(void);
void error_handler(void);

// Declare vectors as an external symbol
extern void *vectors[];

// Kernel entry point
void kernel_entry(void) {
    // Set up stack pointer (done by bootloader)

    // Print a message indicating the kernel is running
    print_string("[kernel]Kernel init completed\n");

    // Set up the interrupt vector table
    asm volatile("msr vbar_el1, %0" : : "r" (vectors));

    // Unmask IRQ interrupts (needed for Ctrl+C)
    asm volatile("msr daifclr, #0b10");  // Unmask IRQ only

    // Main kernel loop
    while (1) {
        asm volatile("svc #0");  // Trigger a software interrupt (system call)
        asm volatile("wfi");     // Wait for interrupt
    }
}

// Print a string to UART
void print_string(const char *str) {
    while (*str) {
        *((volatile uint32_t *) UART_BASE) = *str++;  // Output each character to UART
    }
}

// Exception handlers
void sync_handler(void) {
    print_string("Synchronous exception occurred (System Call).\n");
}

void irq_handler(void) {
    print_string("IRQ received (Ctrl+C pressed, shutting down).\n");

    // Perform QEMU shutdown using Hypervisor Call (HVC)
    asm volatile("mov x0, #0x18");  // 0x18 is QEMU's "power down" signal
    asm volatile("hvc #0");         // Hypervisor call to trigger shutdown
}

void fiq_handler(void) {
    print_string("FIQ received (not used).\n");
}

void error_handler(void) {
    print_string("Error occurred.\n");
}

// Interrupt Vector Table for ARM64
__attribute__((aligned(2048))) void *vectors[] = {
    sync_handler,   // Synchronous exceptions (e.g., system calls)
    irq_handler,    // IRQ (e.g., for Ctrl+C handling)
    fiq_handler,    // FIQ (not used yet)
    error_handler   // Error
};

// Stack
__attribute__((section(".bss"))) char kernel_stack[8192];  // 8KB kernel stack
extern char kernel_stack_top;
