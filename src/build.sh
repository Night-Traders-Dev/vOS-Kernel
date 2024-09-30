#!/bin/bash

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
