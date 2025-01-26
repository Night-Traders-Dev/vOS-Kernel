#include "gic.h"

// Function to initialize the GICv3 system
void gic_init(void) {
    // Initialize Distributor
    WRITE_REG(GICD_CTLR, GICD_CTLR_ENABLE);

    // Enable System Register Interface for GICv3
    WRITE_SYSREG(ICC_SRE_EL1, 0x7);  // Enable EL1 system registers for GIC

    // Enable the CPU interface
//    WRITE_SYSREG(ICC_IGRPEN1_EL1, 1);
    WRITE_SYSREG("ICC_PMR_EL1", GICC_PMR_PRIO_LOW);

    // Set the priority mask to the lowest priority (all interrupts are enabled)
//    WRITE_SYSREG(ICC_PMR_EL1, GICC_PMR_PRIO_LOW);
    WRITE_SYSREG("ICC_IGRPEN1_EL1", 1);
    // Additional setup, such as enabling specific interrupts, can go here...
}

// Function to enable a specific interrupt
void gic_enable_interrupt(uint32_t interrupt_id) {
    uint32_t reg_index = interrupt_id / 32;  // Determine which register to write to
    uint32_t bit_shift = interrupt_id % 32;  // Determine which bit to set

    // Enable the interrupt by setting the corresponding bit
    WRITE_REG(GICD_ISENABLER + (reg_index * 4), (1U << bit_shift));
}

// Function to set the interrupt priority
void gic_set_priority(uint32_t interrupt_id, uint32_t priority) {
    uint32_t reg_index = interrupt_id / 4;  // Determine which register to write to
    uint32_t shift = (interrupt_id % 4) * 8;  // Shift for the appropriate bit field

    // Read the current value of the priority register
    uint32_t reg_value = READ_REG(GICD_IPRIORITYR + (reg_index * 4));

    // Mask the priority field and set the new priority value
    reg_value &= ~(0xFF << shift);  // Clear the existing priority
    reg_value |= (priority << shift);  // Set the new priority

    // Write the updated value back to the register
    WRITE_REG(GICD_IPRIORITYR + (reg_index * 4), reg_value);
}

// Function to set the interrupt target (CPU core)
void gic_set_target(uint32_t interrupt_id, uint32_t cpu_target) {
    uint32_t reg_index = interrupt_id / 4;  // Determine which register to write to
    uint32_t shift = (interrupt_id % 4) * 8;  // Shift for the appropriate bit field

    // Read the current value of the target register
    uint32_t reg_value = READ_REG(GICD_ITARGETSR + (reg_index * 4));

    // Mask the target field and set the new CPU target
    reg_value &= ~(0xFF << shift);  // Clear the existing target
    reg_value |= (cpu_target << shift);  // Set the new target

    // Write the updated value back to the register
    WRITE_REG(GICD_ITARGETSR + (reg_index * 4), reg_value);
}

// Function to configure an interrupt (edge or level triggered)
void gic_set_config(uint32_t interrupt_id, uint32_t config) {
    uint32_t reg_index = interrupt_id / 16;  // Determine which register to write to
    uint32_t shift = (interrupt_id % 16) * 2;  // Shift for the appropriate bit field

    // Read the current value of the configuration register
    uint32_t reg_value = READ_REG(GICD_ICFGR + (reg_index * 4));

    // Mask the config field and set the new config value
    reg_value &= ~(0x3 << shift);  // Clear the existing config
    reg_value |= (config << shift);  // Set the new config

    // Write the updated value back to the register
    WRITE_REG(GICD_ICFGR + (reg_index * 4), reg_value);
}
