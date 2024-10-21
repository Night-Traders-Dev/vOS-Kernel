# vOS Kernel

Welcome to the vOS Kernel! This project is a simple ARM64 microkernel designed for educational purposes and experimentation with operating system concepts. The kernel is capable of booting in a QEMU environment and handling basic input and output through UART.

# Table of Contents

* Features

* Getting Started

* Building the Project

* Running the Kernel

* Kernel Architecture

* Pico 2 Support

* Contributing

* License

# Features

* UART Output: Sends boot messages and kernel status to the serial console.
* Basic Command Shell: A simple shell that accepts commands, currently supports an exit command.
* Interrupt Handling: Supports basic synchronous exceptions and IRQ handling.
* Minimal Design: Focused on being a simple and educational microkernel.
* Formatted Output: Custom implementation of vprint for formatted string output via UART.

# Getting Started

To get started with vOS Kernel, you will need the following tools installed:

* QEMU: For running the virtual machine.
* Clang: The C language family frontend for LLVM.

You can install these on Ubuntu/Debian with the following command:

sudo apt update sudo apt install qemu clang

# Building the Project

To build the kernel and classic bootloader using the Makefile, simply run the following command in the project directory:

make build

This will assemble, compile, and link the bootloader and kernel, producing the necessary ELF file for QEMU.

To build with the UEFI bootloader, use:
> [!CAUTION]
> * currently UEFI is not supported
> * using `make build` works as default and compatibility

make build_uefi

To clean the build files:

make clean

# Running the Kernel

After building the project, you can run the kernel in QEMU using:

make run

# Kernel Architecture

* Bootloader: Written in assembly, initializes the stack and prints a boot message to UART.
* Kernel: Written in C, it initializes the UART, sets up the interrupt vector table, and enters a simple command shell.
> [!IMPORTANT]
> * Bootloader and Kernel are written specifically with Arm64 (aarch64) in mind.
> * RISC-V support is planned.
> * x86_64 support is not planned.

# Pico 2 Support

In addition to QEMU, the vOS Kernel now supports running on the **RP2350** (Pico 2) microcontroller, which features a **hybrid architecture** with ARM Cortex-M33 and RISC-V cores. The kernel is capable of running in **ARM-only**, **RISC-V-only**, or **Hybrid** modes where both cores are utilized.

### Key Features for Pico 2:
* **ARM Cortex-M33** and **RISC-V Hazard3** cores can be booted individually or simultaneously in hybrid mode.
* The bootloader dynamically selects the mode (ARM, RISC-V, or Hybrid) based on a command passed via UART.
* The kernel auto-detects the mode and prints the appropriate message via UART.

### Building for Pico 2:
To build the bootloader and kernel for **Pico 2**, you can specify the mode during the build process using:

make MODE=arm    # For ARM-only mode make MODE=riscv  # For RISC-V-only mode make MODE=hybrid # For Hybrid mode

Each mode will compile the appropriate second-stage bootloader and link the kernel for that architecture.

To clean the Pico 2 build files:

make clean

# Key Components

* **Boot/boot.S**: Bootloader assembly code that sets up the stack and prints boot messages.
* **Lib/kernel.c**: Main kernel code that handles UART output and commands.
* **Include/*.h**: Header files for the kernel functions.

# Contributing

Contributions are welcome! If you'd like to contribute to vOS Kernel, please follow these steps:

1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a Pull Request.

# License

This project is licensed under the MIT License. See the LICENSE file for details.

> [!WARNING]
> There be dragons<br>This code is still early and WIP



