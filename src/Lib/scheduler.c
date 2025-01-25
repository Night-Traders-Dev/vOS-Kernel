#include "scheduler.h"
#include "vstring.h"

static void idle_task(void) {
    while (1) {
        __asm__ volatile("wfi"); // Wait for interrupt (low power mode)
    }
}

int task_create(void (*task_entry)(void), uint8_t priority) {
    if (task_count >= MAX_TASKS) {
        print_string("[kernel] Error: Maximum task limit reached.\n");
        return -1; // Indicate failure
    }

    int task_id = -1;

    // Search for a TERMINATED task to reuse its slot
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].state == TERMINATED) {
            task_id = i;
            break;
        }
    }

    if (task_id == -1) {
        // No reusable task found, use the next available slot
        task_id = task_count++;
    }

    // Initialize the task
    task_t *task = &tasks[task_id];
    task->task_entry = task_entry;
    task->state = READY;
    task->priority = priority;
    task->stack_base = (uint32_t *)task_stacks[task_id];

    // Set the stack pointer to the top of the stack
    task->stack_pointer = task->stack_base + (STACK_SIZE / sizeof(uint32_t));

    // Push the task's entry point (PC) onto the stack
    *(--task->stack_pointer) = (uintptr_t)task_entry;

    // Push the Link Register (LR) with a default value (NULL or a handler)
    *(--task->stack_pointer) = 0;

    // Push default values for R0-R12 (general-purpose registers)
    for (int i = 0; i < CONTEXT_SAVE_SIZE; i++) {
        *(--task->stack_pointer) = 0;
    }

    // Align the stack pointer to 8 bytes
    task->stack_pointer = (uint32_t *)((uintptr_t)task->stack_pointer & ~0x7);

    // Debug print
    print_string("[kernel] Task created with ID: ");
    print_int(task_id);
    print_string(", Priority: ");
    print_int(priority);
    print_string(".\n");

    return 0; // Indicate success
}



void scheduler(void) {
    uint32_t prev_task_idx = current_task;
    int next_task_idx = -1;
    int highest_priority = -1;

    // Find the task with the highest priority that is READY
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].state == READY && tasks[i].priority > highest_priority) {
            highest_priority = tasks[i].priority;
            next_task_idx = i;
        }
    }

    // Fall back to an idle task if no READY tasks are found
    if (next_task_idx == -1) {
        task_create(idle_task, 99);
//        idle_task();
    }

    current_task = next_task_idx;

    // Switch context if necessary
    if (prev_task_idx != current_task) {
        task_t *prev_task = &tasks[prev_task_idx];
        task_t *next_task = &tasks[current_task];

        print_string("[kernel] Switching from task ");
        print_int(prev_task_idx);
        print_string(" to task ");
        print_int(current_task);
        print_string(".\n");

        context_switch(prev_task, next_task);
    }
}

void context_switch(task_t *prev_task, task_t *next_task) {
    __asm__ volatile("mov %0, sp" : "=r"(prev_task->stack_pointer) : : "memory");
    __asm__ volatile("mov sp, %0" :: "r"(next_task->stack_pointer) : "memory");
    ((void (*)(void))((uintptr_t)next_task->stack_pointer[-1]))();
}


void task_yield(void) {
    tasks[current_task].state = READY;
    scheduler();
}
