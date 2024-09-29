# vOS - A Minimal ARM64 Microkernel

vOS is a minimal microkernel designed to run on ARM64 architecture using QEMU. The kernel is capable of handling basic interrupts and provides a simple UART output for console messages. The ultimate goal is to create an environment that can run Python 3 and PIP.

## Features

- Bootloader that initializes the system and transfers control to the kernel.
- Basic UART output for console messages.
- Kernel that handles synchronous exceptions and interrupts, specifically Ctrl+C for graceful shutdown.
- Simple structure that can be expanded to include more features.

## Requirements

To build and run vOS, you'll need:

- QEMU installed on your machine.
- A cross-compiler for ARM64 (such as `aarch64-linux-gnu-gcc` and `aarch64-linux-gnu-as`).
- A terminal environment that supports running shell scripts (like Bash).
