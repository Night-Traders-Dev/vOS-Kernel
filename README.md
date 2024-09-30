# vOS Kernel

Welcome to the **vOS Kernel**! This project is a simple ARM64 microkernel designed for educational purposes and experimentation with operating system concepts. The kernel is capable of booting in a QEMU environment and handling basic input and output through UART.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
- [Building the Project](#building-the-project)
- [Running the Kernel](#running-the-kernel)
- [Kernel Architecture](#kernel-architecture)
- [Contributing](#contributing)
- [License](#license)

## Features

- **UART Output**: Sends boot messages and kernel status to the serial console.
- **Basic Command Shell**: A simple shell that accepts commands, currently supports an `exit` command.
- **Interrupt Handling**: Supports basic synchronous exceptions and IRQ handling.
- **Minimal Design**: Focused on being a simple and educational microkernel.

## Getting Started

To get started with vOS Kernel, you will need the following tools installed:

- **QEMU**: For running the virtual machine.
- **GNU Toolchain for ARM64**: Specifically, `aarch64-linux-gnu-gcc` and `aarch64-linux-gnu-ld`.

You can install these on Ubuntu/Debian with the following command:

```bash
sudo apt update
sudo apt install qemu aarch64-linux-gnu-gcc aarch64-linux-gnu-binutils
```
Building the Project

To build the kernel and bootloader, run the following command in the project directory:

sh build.sh

This script will assemble and link the bootloader and kernel code, producing the necessary ELF file for QEMU.

Running the Kernel

Once you have built the project, you can run it in QEMU with the following command:

qemu-system-aarch64 -machine virt -cpu cortex-a53 -nographic -drive file=boot.elf,if=virtio

This command runs QEMU with a virtual ARM64 machine, displaying the output directly in the terminal.

Kernel Architecture

Bootloader: Written in assembly language, initializes the stack and prints a boot message to UART.

Kernel: Written in C, it initializes the UART, sets up the interrupt vector table, and enters a simple command shell.


Key Components

boot.S: Bootloader assembly code that sets up the stack and prints boot messages.

kernel.c: Main kernel code that handles UART output and commands.

linker script: Defines memory layout and sections for the kernel.


Contributing

Contributions are welcome! If you'd like to contribute to vOS Kernel, please follow these steps:

1. Fork the repository.


2. Create your feature branch (git checkout -b feature/YourFeature).


3. Commit your changes (git commit -m 'Add some feature').


4. Push to the branch (git push origin feature/YourFeature).


5. Open a Pull Request.



License

This project is licensed under the MIT License. See the LICENSE file for details.


