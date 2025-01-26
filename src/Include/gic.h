#ifndef GIC_H
#define GIC_H

#include <stdint.h>

#define WRITE_REG(addr, value)    (*(volatile uint32_t *)(uintptr_t)(addr) = (value))
#define READ_REG(addr)            (*(volatile uint32_t *)(uintptr_t)(addr))

#define WRITE_SYSREG(reg, value) asm volatile("msr " reg ", %0" : : "r" (value) : "memory")
#define READ_SYSREG(reg, var) asm volatile("mrs %0, " reg : "=r" (var))


// GICv3 Distributor base address
#define GICD_BASE 0x08000000

// GICv3 Redistributor base address
#define GICR_BASE 0x080A0000

// GICD Register Offsets
#define GICD_CTLR         (GICD_BASE + 0x0000)  // GICD Control Register
#define GICD_ISENABLER    (GICD_BASE + 0x0100)  // GICD Interrupt Set Enable
#define GICD_ICFGR        (GICD_BASE + 0x0C00)  // GICD Interrupt Configuration
#define GICD_ITARGETSR    (GICD_BASE + 0x0800)  // GICD Interrupt Target
#define GICD_IPRIORITYR   (GICD_BASE + 0x0400)  // GICD Interrupt Priority

// GICv3 System Registers for CPU Interface
#define ICC_CTLR_EL1      "S3_0_C12_C12_4"  // CPU Interface Control Register
#define ICC_PMR_EL1       "S3_0_C4_C6_0"    // Priority Mask Register
#define ICC_IAR1_EL1      "S3_0_C12_C12_0"  // Interrupt Acknowledge Register
#define ICC_EOIR1_EL1     "S3_0_C12_C12_1"  // End of Interrupt Register
#define ICC_IGRPEN1_EL1   "S3_0_C12_C12_7"  // Group 1 Interrupt Enable Register
#define ICC_SRE_EL1       "S3_0_C12_C12_5"  // System Register Enable

// GICD Control Register Value
#define GICD_CTLR_ENABLE  (1U << 0)  // Enable the distributor

// Priority values for GIC
#define GICC_PMR_PRIO_LOW (0xFF)  // Lowest priority value

// Function prototypes
void gic_init(void);
void gic_enable_interrupt(uint64_t interrupt_id);
void gic_set_priority(uint64_t interrupt_id, uint64_t priority);
void gic_set_target(uint64_t interrupt_id, uint64_t cpu_target);
void gic_set_config(uint64_t interrupt_id, uint64_t config);

#endif // GIC_H
