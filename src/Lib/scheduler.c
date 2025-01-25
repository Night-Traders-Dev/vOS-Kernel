#include "scheduler.h"
#include "vstring.h"

void task_create(void (*task_entry)(void), uint8_t priority) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].state == TERMINATED) {
            // Reuse this task slot
            tasks[i].task_entry = task_entry;
            tasks[i].state = READY;
            tasks[i].priority = priority;
            tasks[i].stack_base = (uint32_t *)task_stacks[i];
            tasks[i].stack_pointer = tasks[i].stack_base + (STACK_SIZE / sizeof(uint32_t));
            *(--tasks[i].stack_pointer) = (uintptr_t)task_entry;

            for (int j = 0; j < CONTEXT_SAVE_SIZE; j++) {
                *(--tasks[i].stack_pointer) = 0; // Placeholder for saved registers
            }

            print_string("[kernel] Reused task ID: ");
            print_int(i);
            print_string(", Priority: ");
            print_int(priority);
            print_string(".\n");

            return;
        }
    }

    if (task_count >= MAX_TASKS) {
        print_string("[kernel] Error: Maximum task limit reached.\n");
        return;
    }

    // Create a new task
    task_t *task = &tasks[task_count];
    task->task_entry = task_entry;
    task->state = READY;
    task->priority = priority;
    task->stack_base = (uint32_t *)task_stacks[task_count];
    task->stack_pointer = task->stack_base + (STACK_SIZE / sizeof(uint32_t));
    *(--task->stack_pointer) = (uintptr_t)task_entry;

    for (int i = 0; i < CONTEXT_SAVE_SIZE; i++) {
        *(--task->stack_pointer) = 0;
    }

    print_string("[kernel] Task created with ID: ");
    print_int(task_count);
    print_string(", Priority: ");
    print_int(priority);
    print_string(".\n");

    task_count++;
}


void scheduler(void) {
    static int cycles_since_last_check = 0;
    const int check_interval = 5;

    uint32_t prev_task_idx = current_task;
    int next_task_idx = -1;
    int highest_priority = -1;

    // Check for active tasks every few cycles
    if (cycles_since_last_check >= check_interval) {
        cycles_since_last_check = 0;

        int active_task_count = 0;
        for (int i = 0; i < task_count; i++) {
            if (tasks[i].state == READY) {
                active_task_count++;
            }
        }

        // If there is only one active task, no need to switch
        if (active_task_count <= 1) {
            return;
        }
    }

    cycles_since_last_check++;

    // Find the task with the highest priority that is READY
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].state == READY && tasks[i].priority > highest_priority) {
            highest_priority = tasks[i].priority;
            next_task_idx = i;
        }
    }

    // Fall back to round-robin if no task is READY or priorities are equal
    if (next_task_idx == -1) {
        next_task_idx = (current_task + 1) % task_count;
        int scanned_tasks = 0;
        while (tasks[next_task_idx].state != READY) {
            next_task_idx = (next_task_idx + 1) % task_count;
            scanned_tasks++;

            // Prevent infinite loop in case all tasks are BLOCKED or TERMINATED
            if (scanned_tasks >= task_count) {
                return; // No valid task to switch to
            }
        }
    }

    current_task = next_task_idx;

    // Switch context if the current task is different from the previous one
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
