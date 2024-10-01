#!/bin/bash

# Function to clean the build artifacts
clean() {
    echo "Cleaning build files..."
    rm -f boot.o kernel.o boot.elf
    echo "Clean complete."
}

# Function to build the bootloader and kernel
build() {
    echo "Building bootloader..."

    # Assemble bootloader
    aarch64-linux-gnu-as -o boot.o boot.S

    echo "Building kernel..."

    # Compile kernel in C
    aarch64-linux-gnu-gcc -c -o kernel.o kernel.c -ffreestanding -nostdlib

    echo "Linking bootloader and kernel..."

    # Link bootloader and kernel into a single ELF
    aarch64-linux-gnu-ld -T kernel.ld -o boot.elf boot.o kernel.o

    echo "Running QEMU..."

    # Run the combined ELF in QEMU
    qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel boot.elf
}

# Check for arguments
if [ "$1" == "clean" ]; then
    clean
    exit 0
elif [ "$1" == "cleanbuild" ]; then
    clean
    build
else
    build
fi
