# vOS Project Suite

Welcome to the **vOS Project Suite**! This suite includes a microkernel, a desktop environment, and a minimal user interface, designed for educational purposes and experimentation in operating system development.

## Table of Contents

- [Overview](#overview)
- [Repositories](#repositories)
  - [vOS Kernel](#vos-kernel)
  - [vOS](#vos)
  - [vOS Desktop](#vos-desktop)
- [Getting Started](#getting-started)
- [Building the Projects](#building-the-projects)
- [Running the Projects](#running-the-projects)
- [Contributing](#contributing)
- [License](#license)

## Overview

The vOS Project Suite consists of three main components:

1. **vOS Kernel**: A simple ARM64 microkernel that provides essential operating system features.
2. **vOS**: A minimal user space environment that interfaces with the kernel.
3. **vOS Desktop**: A graphical desktop environment built on top of the vOS user space.

## Repositories

### vOS Kernel

- **Repository**: [vOS-Kernel](https://github.com/Night-Traders-Dev/vOS-Kernel)
- **Description**: The microkernel of the vOS Project Suite. It handles basic I/O through UART and manages interrupts.
- **Features**:
  - UART output for boot messages and kernel status.
  - Basic command shell.
  - Interrupt handling.

### vOS

- **Repository**: [vOS](https://github.com/Night-Traders-Dev/vOS)
- **Description**: The user space environment that communicates with the vOS kernel.
- **Features**:
  - User commands to interact with the kernel.
  - Integration with the vOS Kernel for executing system calls.

### vOS Desktop

- **Repository**: [vOS-Desktop](https://github.com/Night-Traders-Dev/vOS-Desktop)
- **Description**: A graphical desktop environment designed for vOS.
- **Features**:
  - Lightweight and minimalistic user interface.
  - Desktop management and application launching.

## Getting Started

To get started with the vOS Project Suite, you will need to install the following tools:

- **QEMU**: For running the virtual machine.
- **GNU Toolchain for ARM64**: Specifically, `aarch64-linux-gnu-gcc` and `aarch64-linux-gnu-ld`.
- **Python3**: For running virtual operating system.
- **PIP**: For installing required libraries
- **Textual**: For running the vOS-Desktop Environment

Building the Projects

To build each component of the vOS Project Suite, navigate to each project repository and follow the respective build commands


Contributing

Contributions are welcome! If you'd like to contribute to the vOS Project Suite, please follow these steps:

1. Fork the repository.


2. Create your feature branch (git checkout -b feature/YourFeature).


3. Commit your changes (git commit -m 'Add some feature').


4. Push to the branch (git push origin feature/YourFeature).


5. Open a Pull Request.



License

This project is licensed under the MIT License. See the LICENSE file for details.



