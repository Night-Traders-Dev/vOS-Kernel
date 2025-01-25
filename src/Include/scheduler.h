#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "vstring.h"
#include "timer.h"

// Maximum number of tasks
#define MAX_TASKS 5
#define STACK_SIZE 0x200 // Size of each task stack in bytes
#define CONTEXT_SAVE_SIZE 18 // Number of registers saved during a context switch

// Task states
typedef enum {
    RUNNING,
    READY,
    BLOCKED,
    TERMINATED
} task_state_t;

// Task structure
typedef struct {
    uint32_t *stack_pointer;  // Points to the current stack position for the task
    uint32_t *stack_base;     // Points to the base of the task's stack
    task_state_t state;       // Current state of the task
    void (*task_entry)(void); // Entry function for the task
    int priority;             // Priority of the task (higher value = higher priority)
} task_t;

// Function prototypes
int task_create(void (*task_entry)(void), uint8_t priority); // Create a new task
void scheduler(void);                                       // Task scheduler function
void context_switch(task_t *prev_task, task_t *next_task);  // Perform a context switch
void task_yield(void);                                      // Yield control to the scheduler
static void idle_task(void);                                // Idle task function

// Task stacks and task array
static uint8_t task_stacks[MAX_TASKS][STACK_SIZE];          // Memory for task stacks
static task_t tasks[MAX_TASKS];                             // Array of task control blocks

// Global variables
extern uint32_t task_count;                                 // Number of created tasks
extern uint32_t current_task;                               // Index of the currently running task

#endif // SCHEDULER_H
