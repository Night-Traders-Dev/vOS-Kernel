#include "gic.h"

void gic_init(void) {
    WRITE_REG(GICD_CTLR, GICD_CTLR_ENABLE);  // Enable the GIC distributor
    WRITE_SYSREG("ICC_SRE_EL1", 0x7);        // Enable EL1 system registers for GIC
    WRITE_SYSREG("ICC_PMR_EL1", GICC_PMR_PRIO_LOW);  // Set the lowest priority mask
    WRITE_SYSREG("ICC_IGRPEN1_EL1", 1);      // Enable group 1 interrupts
}

// Function to enable a specific interrupt
void gic_enable_interrupt(uint64_t interrupt_id) {
    uint64_t reg_index = interrupt_id / 32;  // Determine the register index
    uint64_t bit_shift = interrupt_id % 32;  // Determine the bit to set

    // Enable the interrupt by setting the corresponding bit
    WRITE_REG((GICD_ISENABLER + (reg_index * 4)), (1ULL << bit_shift));
}

// Function to set the interrupt priority
void gic_set_priority(uint64_t interrupt_id, uint64_t priority) {
    uint64_t reg_index = interrupt_id / 4;  // Determine the register index
    uint64_t shift = (interrupt_id % 4) * 8;  // Shift for the appropriate field

    // Read the current value of the priority register
    uint64_t reg_value = READ_REG(GICD_IPRIORITYR + (reg_index * 4));

    // Update the priority field
    reg_value &= ~(0xFFULL << shift);  // Clear the current priority
    reg_value |= (priority << shift);  // Set the new priority

    // Write the updated value back to the register
    WRITE_REG((GICD_IPRIORITYR + (reg_index * 4)), reg_value);
}

// Function to set the interrupt target (CPU core)
void gic_set_target(uint64_t interrupt_id, uint64_t cpu_target) {
    uint64_t reg_index = interrupt_id / 4;  // Determine the register index
    uint64_t shift = (interrupt_id % 4) * 8;  // Shift for the appropriate field

    // Read the current value of the target register
    uint64_t reg_value = READ_REG(GICD_ITARGETSR + (reg_index * 4));

    // Update the target CPU field
    reg_value &= ~(0xFFULL << shift);  // Clear the current target
    reg_value |= (cpu_target << shift);  // Set the new CPU target

    // Write the updated value back to the register
    WRITE_REG((GICD_ITARGETSR + (reg_index * 4)), reg_value);
}

// Function to configure an interrupt (edge or level triggered)
void gic_set_config(uint64_t interrupt_id, uint64_t config) {
    uint64_t reg_index = interrupt_id / 16;  // Determine the register index
    uint64_t shift = (interrupt_id % 16) * 2;  // Shift for the appropriate field

    // Read the current value of the configuration register
    uint64_t reg_value = READ_REG(GICD_ICFGR + (reg_index * 4));

    // Update the configuration field
    reg_value &= ~(0x3ULL << shift);  // Clear the current configuration
    reg_value |= (config << shift);  // Set the new configuration

    // Write the updated value back to the register
    WRITE_REG((GICD_ICFGR + (reg_index * 4)), reg_value);
}
