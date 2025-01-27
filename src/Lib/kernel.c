#include "kernel.h"

// Kernel tasks
void shell_task(void) {
    char buffer[128];
    const uint32_t timeout_ticks = 50000;
    init_heartbeat();
//    setup_timer_1ms(timer_ISR);
    while (1) {
//        if (check_heartbeat()) {
//            print_string("1-second heartbeat triggered!\n");/
//        }
        print_string("$ ");
        uart_read_string(buffer, sizeof(buffer), timeout_ticks);

        if (buffer[0] != '\0') {
            handle_command(buffer);
        } else {
            print_string("[kernel] No input received.\n");
        }
    }
}

void hello(void) {
    print_string("hello\n");
}
// Kernel entry point
void kernel_entry(void) {
    static bool kernel_initialized = false;
    const char *kernel_fs = "Kernel Dummy File";
    const char *data_fs = "Data Dummy File";

    // Initialize filesystem
    fs_init();
    if (fs_mkdir("/") < 0) {
        print_string("[kernel] Failed to create root directory.\n");
        system_off();
    }
    if (fs_create("kernel.fs") < 0) {
        print_string("[kernel] Failed to create kernel.fs.\n");
        system_off();
    }
    if (fs_create("data.fs") < 0) {
        print_string("[kernel] Failed to create data.fs.\n");
        system_off();
    }

    if (fs_write("data.fs", data_fs, strlength(data_fs)) < 0) {
        print_string("[kernel] Failed to write to data.fs.\n");
        system_off();
    }
    if (fs_write("kernel.fs", kernel_fs, strlength(kernel_fs)) < 0) {
        print_string("[kernel] Failed to write to kernel.fs.\n");
        system_off();
    }
    if (fs_dir_size("/", strlength(kernel_fs) + strlength(data_fs)) < 0) {
        print_string("[kernel] Failed to calculate directory size.\n");
        system_off();
    }

    clear();
    print_string("Welcome to vOS\n\n");

    kernel_initialized = true;
    init_scheduler();
    task_create(uart_task, 2);
    int shell_task_idx = task_create(shell_task, 1);
    void (*shell_task_entry)(void) = shell_task;
    create_timer(5, hello);
    execute_task_immediately(shell_task_entry);
}


// System shutdown function
void system_off(void) {
    print_string("[kernel] vOS Kernel Shutdown...\n");

    #if defined(TARGET_PICO) || defined(TARGET_PICO_2)
        gpio_init(15);
        gpio_set_dir(15, GPIO_OUT);
        gpio_put(15, 0);

        sleep_ms(100);

        while (1) {
        }
    #else
        register const uint64_t function_id asm("x0") = QEMU_SHUTDOWN_PORT;
        asm volatile("hvc #0" :: "r"(function_id));
    #endif
}
