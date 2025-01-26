#include "scheduler.h"

uint32_t task_count = 0;
uint32_t current_task = 0;
task_t tasks[MAX_TASKS]; 

void idle_task(void) {
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
    task->stack_base = (uint64_t *)task_stacks[task_id];

    // Initialize the stack pointer to the top of the stack
    task->stack_pointer = task->stack_base + (STACK_SIZE / sizeof(uint64_t));

    // Simulate an interrupted CPU state (stack frame setup for ARM64)
    *(--task->stack_pointer) = (uintptr_t)task_entry; // PC: Entry point of the task
    *(--task->stack_pointer) = 0;                    // x30 (LR): Link Register
    *(--task->stack_pointer) = 0;                    // x29 (FP): Frame Pointer

    // Push callee-saved registers (x19–x28, initialized to 0)
    for (int i = 19; i <= 28; i++) {
        *(--task->stack_pointer) = 0; // Callee-saved register
    }

    // Push general-purpose registers (x0–x18, initialized to 0)
    for (int i = 0; i <= 18; i++) {
        *(--task->stack_pointer) = 0; // General-purpose register
    }

    // Ensure the stack pointer is 16-byte aligned as per the AArch64 ABI
    task->stack_pointer = (uint64_t *)((uintptr_t)task->stack_pointer & ~0xF);

    print_string("[kernel] Task created with ID: ");
    print_int(task_id);
    print_string(", Priority: ");
    print_int(priority);
    print_string(".\n");

    return task_id; // Return the task ID as a success indicator
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

void context_switch(task_t *prev_task, task_t *next_task) {
    __asm__ volatile("mov %0, sp" : "=r"(prev_task->stack_pointer) : : "memory");
    __asm__ volatile("mov sp, %0" :: "r"(next_task->stack_pointer) : "memory");
    __asm__ volatile(
        "ldp x19, x20, [sp], #16\n" // Restore x19, x20
        "ldp x21, x22, [sp], #16\n" // Restore x21, x22
        "ldp x23, x24, [sp], #16\n" // Restore x23, x24
        "ldp x25, x26, [sp], #16\n" // Restore x25, x26
        "ldp x27, x28, [sp], #16\n" // Restore x27, x28
        "ldp x29, x30, [sp], #16\n" // Restore x29 (FP) and x30 (LR)
        "ldr x0, [sp], #8\n"        // Restore x0 (argument register)
        "br x0\n"                   // Branch to the task entry point
    );
}


void task_yield(void) {
    print_string("[scheduler] Task yielding execution...\n");
    tasks[current_task].state = READY;
    scheduler();
    print_string("[scheduler] Task rescheduled.\n");
}
