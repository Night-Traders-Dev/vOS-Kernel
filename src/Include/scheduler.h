#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

// Maximum number of tasks
#define MAX_TASKS 5
#define STACK_SIZE 0x200 // Size of each task stack
#define CONTEXT_SAVE_SIZE 18

// Task state
typedef enum {
    RUNNING,
    READY,
    BLOCKED,
    TERMINATED
} task_state_t;

// Task structure
typedef struct {
    uint32_t *stack_pointer;
    uint32_t *stack_base;
    task_state_t state;
    void (*task_entry)(void);
    int priority;
} task_t;

int task_create(void (*task_entry)(void), uint8_t priority);
void scheduler(void);
void context_switch(task_t *prev_task, task_t *next_task);
void task_yield(void);

static uint8_t task_stacks[MAX_TASKS][STACK_SIZE];
static task_t tasks[MAX_TASKS];
static uint32_t task_count = 0;
static uint32_t current_task = 0;

static void idle_task(void);

#endif // SCHEDULER_H
