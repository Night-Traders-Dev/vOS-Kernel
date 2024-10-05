# vOS Kernel

Welcome to the vOS Kernel! This project is a simple ARM64 microkernel designed for educational purposes and experimentation with operating system concepts. The kernel is capable of booting in a QEMU environment and handling basic input and output through UART.

# Table of Contents

* Features

* Getting Started

* Building the Project

* Running the Kernel

* Kernel Architecture

* Contributing

* License


# Features
[!IMPORTANT]
UART Output: Sends boot messages and kernel status to the serial console.

Basic Command Shell: A simple shell that accepts commands, currently supports an exit command.

Interrupt Handling: Supports basic synchronous exceptions and IRQ handling.

Minimal Design: Focused on being a simple and educational microkernel.

Formatted Output: Custom implementation of vprint for formatted string output via UART.


# Getting Started

To get started with vOS Kernel, you will need the following tools installed:

* QEMU: For running the virtual machine.

* Clang: The C language family frontend for LLVM.


You can install these on Ubuntu/Debian with the following command:
```
sudo apt update
sudo apt install qemu clang
```
# Building the Project

To build the kernel and classic bootloader using the Makefile, simply run the following command in the project directory:
```
make build
```
This will assemble, compile, and link the bootloader and kernel, producing the necessary ELF file for QEMU.

To build with the UEFI bootloader, use:
[!CAUTION]
currently uefi is not supported, using build works as default and compatibility
```
make build_uefi
```
To clean the build files:
```
make clean
```
# Running the Kernel

After building the project, you can run the kernel in QEMU using:
```
make run
```
# Kernel Architecture

* Bootloader: Written in assembly, initializes the stack and prints a boot message to UART.

* Kernel: Written in C, it initializes the UART, sets up the interrupt vector table, and enters a simple command shell.


# Key Components

* Boot/boot.S: Bootloader assembly code that sets up the stack and prints boot messages.

* Lib/kernel.c: Main kernel code that handles UART output and commands.

* Include/*.h: Header files for the kernel functions.


# Contributing

Contributions are welcome! If you'd like to contribute to vOS Kernel, please follow these steps:

1. Fork the repository.


2. Create your feature branch (git checkout -b feature/YourFeature).


3. Commit your changes (git commit -m 'Add some feature').


4. Push to the branch (git push origin feature/YourFeature).


5. Open a Pull Request.



# License

This project is licensed under the MIT License. See the LICENSE file for details.

