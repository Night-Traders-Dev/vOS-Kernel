# vOS Kernel

Welcome to the vOS Kernel! This project is a simple ARM64 microkernel designed for educational purposes and experimentation with operating system concepts. The kernel is capable of booting in a QEMU environment, on a Raspberry Pi 4, and handling basic input and output through UART.

> [!CAUTION]
> * WIP  
> * Beware there be dragons!!!

---

# Table of Contents

- Features  
- Getting Started  
- Building the Project  
- Running the Kernel  
- Kernel Architecture  
- Raspberry Pi 4 Support  
- Pico 2 Support  
- Key Components  
- Contributing  
- License  

---

# Features

- **UART Output**: Sends boot messages and kernel status to the serial console.  
- **Basic Command Shell**: A simple shell that accepts commands, currently supports an exit command.  
- **Interrupt Handling**: Supports basic synchronous exceptions and IRQ handling.  
- **Minimal Design**: Focused on being a simple and educational microkernel.  
- **Formatted Output**: Custom implementation of `vprint` for formatted string output via UART.  
- **Scheduler**: A simple task scheduler with task management and memory tracking functionality.  

---

# Getting Started

To get started with vOS Kernel, you will need the following tools installed:

- **QEMU**: For running the virtual machine.  
- **Clang**: The C language family frontend for LLVM.  

You can install these on Ubuntu/Debian with the following command:

```bash
sudo apt update && sudo apt install qemu clang
```

---

# Building the Project

To build the kernel and classic bootloader using the Makefile, simply run the following command in the project directory:

```bash
make qemu
```
or
```bash
make rpi
```

This will assemble, compile, and link the bootloader and kernel, producing the necessary ELF file for QEMU or Raspberry Pi 4.

To clean the build files:

```bash
make clean
```

---

# Running the Kernel

### In QEMU
After building the project, you can run the kernel in QEMU using:

```bash
make run-qemu
```

### On Raspberry Pi 4
To get run instructions the kernel on a Raspberry Pi 4:
   ```bash
   make run-rpi
   ```

---

# Kernel Architecture

- **Bootloader**: Written in assembly, initializes the stack and prints a boot message to UART.  
- **Kernel**: Written in C, it initializes the UART, sets up the interrupt vector table, and enters a simple command shell.  

> [!IMPORTANT]  
> - Bootloader and Kernel are written specifically with **ARM64 (aarch64)** in mind.  
> - **RISC-V** support is planned.  
> - **x86_64** support is not planned.  

---

# Raspberry Pi 4 Support

The vOS Kernel is now capable of running on a **Raspberry Pi 4**.  
Key considerations for Raspberry Pi 4 support include:  

- **UART**: The kernel communicates over UART0 for serial output. Ensure the Raspberry Pi is connected to a serial console to view kernel logs.  
- **Bootloader**: The Raspberry Pi 4 uses `kernel8.img` as the binary name. Ensure you copy the compiled image to the SD card's boot partition.  

---

# Pico 2 Support

In addition to QEMU and Raspberry Pi 4, the vOS Kernel is planned to support the **RP2350** (Pico 2) microcontroller, which features a **hybrid architecture** with ARM Cortex-M33 and RISC-V cores. The kernel will be capable of running in **ARM** or **RISC-V** modes.  

---

# Key Components

- **Boot/boot.S**: Bootloader assembly code that sets up the stack and prints boot messages.  
- **Lib/kernel.c**: Main kernel code that handles UART output and commands.  
- **Include/*.h**: Header files for the kernel functions.  
- **rpi.ld**: Linker script for Raspberry Pi 4 memory layout.  
- **qemu.ld**: Linker script for QEMU memory layout.  

---

# Contributing

Contributions are welcome! If you'd like to contribute to vOS Kernel, please follow these steps:

1. Fork the repository.  
2. Create your feature branch:  
   ```bash
   git checkout -b feature/YourFeature
   ```  
3. Commit your changes:  
   ```bash
   git commit -m 'Add some feature'
   ```  
4. Push to the branch:  
   ```bash
   git push origin feature/YourFeature
   ```  
5. Open a Pull Request.  

---

# License

This project is licensed under the **MIT License**. See the LICENSE file for details.  


