#ifndef GIC_H
#define GIC_H

#include <stdint.h>

// GICv3 Distributor base address
#define GICD_BASE 0x2C000000  // QEMU's default address for GICv3 Distributor

// GICv3 CPU Interface base address
#define GICC_BASE 0x2C001000  // QEMU's default address for GICv3 CPU Interface

// GICD Register Offsets
#define GICD_CTLR         (GICD_BASE + 0x0000)  // GICD Control Register
#define GICD_ISENABLER    (GICD_BASE + 0x0100)  // GICD Interrupt Set Enable
#define GICD_ICFGR        (GICD_BASE + 0x0C00)  // GICD Interrupt Configuration
#define GICD_ITARGETSR    (GICD_BASE + 0x0800)  // GICD Interrupt Target
#define GICD_IPRIORITYR   (GICD_BASE + 0x0400)  // GICD Interrupt Priority

// GICC Register Offsets
#define GICC_CTLR         (GICC_BASE + 0x0000)  // GICC Control Register
#define GICC_PMR          (GICC_BASE + 0x0004)  // GICC Priority Mask Register
#define GICC_BPR          (GICC_BASE + 0x0008)  // GICC Binary Point Register

// GICD Control Register Value
#define GICD_CTLR_ENABLE  (1U << 0)  // Enable the distributor

// GICC Control Register Value
#define GICC_CTLR_ENABLE  (1U << 0)  // Enable the CPU interface

// Priority values for GIC
#define GICC_PMR_PRIO_LOW (0xFF)  // Lowest priority value

// Function prototypes
void gic_init(void);
void gic_enable_interrupt(uint32_t interrupt_id);
void gic_set_priority(uint32_t interrupt_id, uint32_t priority);
void gic_set_target(uint32_t interrupt_id, uint32_t cpu_target);
void gic_set_config(uint32_t interrupt_id, uint32_t config);

#endif // GIC_H
