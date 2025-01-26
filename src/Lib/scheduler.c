#include "scheduler.h"

// Global variable definitions
int idle_task_idx = -1;                         // Default to -1 (no idle task initially)
uint32_t task_count = 0;                        // Start with no tasks created
uint32_t current_task = 0;                      // Current task index
task_t tasks[MAX_TASKS];                        // Task control blocks
uint8_t task_stacks[MAX_TASKS][STACK_SIZE];     // Task stacks

// Idle task definition
void idle_task(void) {
    while (1) {
        print_string("idle\n");
    }
}

// Initialize the scheduler and create the idle task
void init_scheduler(void) {
    idle_task_idx = task_create(idle_task, 255); // Lowest priority
    if (idle_task_idx < 0) {
        print_string("[kernel] Failed to create idle task.\n");
    }
}

// Sort tasks by priority (lowest number = highest priority)
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


task_t* scheduler_get_current_task(void) {
    return &tasks[current_task];
}

void scheduler_set_current_task(task_t* task) {
    for (int i = 0; i < task_count; i++) {
        if (&tasks[i] == task) {
            current_task = i;
            return;
        }
    }
    print_string("[kernel] Error: Task not found.\n");
}

// Execute a specific task immediately, overriding the scheduler temporarily
void execute_task_immediately(void (*task_function)(void)) {
    if (!task_function) {
        print_string("[scheduler] Invalid task function provided.\n");
        return;
    }

    // Save the current task
    task_t* current_task_ptr = scheduler_get_current_task();

    // Execute the task function immediately
    print_string("[scheduler] Executing high-priority task immediately...\n");
    task_function();

    // Restore the original task
    scheduler_set_current_task(current_task_ptr);

    print_string("[scheduler] High-priority task execution complete.\n");
}

// Create a new task
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
    task_t* task = &tasks[task_id];
    task->task_entry = task_entry;
    task->state = READY;
    task->priority = priority;
    task->stack_base = (uintptr_t)task_stacks[task_id]; // Use uintptr_t for the base address

    // Initialize the stack pointer to the top of the stack
    task->stack_pointer = task->stack_base + STACK_SIZE;

    // Simulate an interrupted CPU state (stack frame setup for ARM64)
    task->stack_pointer -= sizeof(uintptr_t);
    *((uintptr_t*)task->stack_pointer) = (uintptr_t)task_entry; // PC: Entry point of the task
    task->stack_pointer -= sizeof(uintptr_t);
    *((uintptr_t*)task->stack_pointer) = 0; // x30 (LR): Link Register
    task->stack_pointer -= sizeof(uintptr_t);
    *((uintptr_t*)task->stack_pointer) = 0; // x29 (FP): Frame Pointer

    // Push callee-saved registers (x19–x28, initialized to 0)
    for (int i = 19; i <= 28; i++) {
        task->stack_pointer -= sizeof(uintptr_t);
        *((uintptr_t*)task->stack_pointer) = 0; // Callee-saved register
    }

    // Push general-purpose registers (x0–x18, initialized to 0)
    for (int i = 0; i <= 18; i++) {
        task->stack_pointer -= sizeof(uintptr_t);
        *((uintptr_t*)task->stack_pointer) = 0; // General-purpose register
    }

    // Ensure the stack pointer is 16-byte aligned as per the AArch64 ABI
    task->stack_pointer &= ~0xF;

    print_string("[kernel] Task created with ID: ");
    print_int(task_id);
    print_string(", Priority: ");
    print_int(priority);
    print_string(".\n");

    return task_id; // Return the task ID as a success indicator
}

// Scheduler implementation
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

    // Fall back to the idle task if no READY tasks are found
    if (next_task_idx == -1) {
        if (idle_task_idx == -1) {
            print_string("[kernel] Creating idle task.\n");
            idle_task_idx = task_create(idle_task, 0);
        }
        next_task_idx = idle_task_idx;
    }

    // Update the current task
    current_task = next_task_idx;

    // Check if a context switch is required
    if (prev_task_idx != current_task) {
        tasks[prev_task_idx].state = READY; // Set the previous task to READY
        tasks[current_task].state = RUNNING; // Set the next task to RUNNING

        print_string("[kernel] Switching context from task ");
        print_int(prev_task_idx);
        print_string(" to task ");
        print_int(current_task);
        print_string(".\n");

        // Perform context switch
        context_switch(&tasks[prev_task_idx], &tasks[current_task]);
    } else {
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
