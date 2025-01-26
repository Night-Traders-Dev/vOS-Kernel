#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "vstring.h"
#include "timer.h"

// Maximum number of tasks
#define MAX_TASKS 5
#define STACK_SIZE 0x200 // Size of each task stack in bytes
#define NUM_GP_REGISTERS 18 // Number of general-purpose registers saved during a context switch

// Task states
typedef enum {
    RUNNING,
    READY,
    BLOCKED,
    TERMINATED
} task_state_t;

// Task function pointer type
typedef void (*task_entry_t)(void);

// Task structure
typedef struct {
    uintptr_t stack_pointer; // Stack pointer
    uintptr_t stack_base;    // Stack base address
    task_entry_t task_entry; // Entry function for the task
    int priority;            // Task priority
    task_state_t state;      // Current state of the task
} task_t;

// Global variables (declared as extern to avoid multiple definitions)
extern int idle_task_idx;                       // Index of the idle task
extern uint32_t task_count;                     // Number of created tasks
extern uint32_t current_task;                   // Index of the currently running task
extern task_t tasks[MAX_TASKS];                 // Array of task control blocks
extern uint8_t task_stacks[MAX_TASKS][STACK_SIZE]; // Memory for task stacks
extern void init_scheduler(void);                                      // Initialize the scheduler
extern int task_create(task_entry_t task_entry, uint8_t priority);     // Create a new task
extern void scheduler(void);                                           // Task scheduler function
extern void context_switch(task_t *prev_task, task_t *next_task);      // Perform a context switch
extern void task_yield(void);                                          // Yield control to the scheduler
extern void idle_task(void);                                           // Idle task function
extern task_t* scheduler_get_current_task(void);                       // Get the current task
extern void scheduler_set_current_task(task_t* task);                  // Set the current task

#endif // SCHEDULER_H
