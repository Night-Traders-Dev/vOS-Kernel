#include "scheduler.h"

uint32_t task_count = 0;
uint32_t current_task = 0;
task_t tasks[MAX_TASKS]; 

static void idle_task(void) {
    while (1) {
        print_string("idle\n");
        __asm__ volatile("wfi"); // Wait for interrupt (low power mode)
    }
}

void sort_tasks_by_priority(void) {
    for (uint8_t i = 0; i < task_count - 1; i++) {
        for (uint8_t j = i + 1; j < task_count; j++) {
            if (tasks[i].priority > tasks[j].priority) {
                task_t temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }
}

int task_create(void (*task_entry)(void), uint8_t priority) {
    if (task_count >= MAX_TASKS) {
        print_string("[kernel] Error: Maximum task limit reached.\n");
        return -1; // Indicate failure
    }

    int task_id = -1;
    // Find an available task slot
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].state == TERMINATED) {
            task_id = i;
            break;
        }
    }

    // If no terminated task slot, create a new task at the next available index
    if (task_id == -1) {
        task_id = task_count++;
    }

    // Initialize the task
    task_t *task = &tasks[task_id];
    task->task_entry = task_entry;
    task->state = READY;
    task->priority = priority;
    task->stack_base = (uint32_t *)task_stacks[task_id];

    task->stack_pointer = task->stack_base + (STACK_SIZE / sizeof(uint32_t));
    *(--task->stack_pointer) = (uintptr_t)task_entry; // Task entry point
    *(--task->stack_pointer) = 0; // Return address (e.g., to context_switch or idle task)

    // Initialize all registers (use CONTEXT_SAVE_SIZE for the number of registers to be saved)
    for (int i = 0; i < CONTEXT_SAVE_SIZE; i++) {
        *(--task->stack_pointer) = 0; // Initialize registers to zero
    }

    // Ensure the stack pointer is properly aligned
    task->stack_pointer = (uint32_t *)((uintptr_t)task->stack_pointer & ~0x7);

    print_string("[kernel] Task created with ID: ");
    print_int(task_id);
    print_string(", Priority: ");
    print_int(priority);
    print_string(".\n");

    return 0; // Indicate success
}

void scheduler(void) {
    print_string("Scheduler...\n");

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

    // Fall back to the idle task if no READY tasks are found
    if (next_task_idx == -1) {
        static int idle_task_created = 0;

        if (!idle_task_created) {
            idle_task_created = 1;
            print_string("[kernel] No READY tasks. Creating idle task.\n");
            task_create(idle_task, 0); // Create idle task with the lowest priority
        }

        next_task_idx = task_count - 1; // Assume idle task is the last created
    }

    // Update the current task
    current_task = next_task_idx;

    // Check if a context switch is required
    if (prev_task_idx != current_task) {
        task_t *prev_task = &tasks[prev_task_idx];
        task_t *next_task = &tasks[current_task];

        // Debug log for task switching
        print_string("[kernel] Switching context from task ");
        print_int(prev_task_idx);
        print_string(" (Priority: ");
        print_int(prev_task->priority);
        print_string(") to task ");
        print_int(current_task);
        print_string(" (Priority: ");
        print_int(next_task->priority);
        print_string(").\n");

        // Perform context switch
        context_switch(prev_task, next_task);
    } else {
        // Log if no task switch is necessary
        print_string("[kernel] No task switch needed. Continuing task ");
        print_int(current_task);
        print_string(".\n");
    }
}

// Perform a context switch between two tasks
void context_switch(task_t *prev_task, task_t *next_task) {
    __asm__ volatile("mov %0, sp" : "=r"(prev_task->stack_pointer) : : "memory");
    __asm__ volatile("mov sp, %0" :: "r"(next_task->stack_pointer) : "memory");

    // Jump to the task entry point
    ((void (*)(void))((uintptr_t)next_task->stack_pointer[-1]))();
}

void task_yield(void) {
    print_string("[scheduler] Task yielding execution...\n");
    tasks[current_task].state = READY;
    scheduler();
    print_string("[scheduler] Task rescheduled.\n");
}
