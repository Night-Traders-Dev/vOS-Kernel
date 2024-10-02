#!/bin/bash

# Function to clean the build artifacts
clean() {
    echo "Cleaning build files..."
    rm -f boot.o kernel.o boot.elf
    echo "Clean complete."
}

# Function to build the bootloader and kernel
build() {
    if [ "$1" == "uefi" ]; then
        echo "Building UEFI bootloader..."

        # Compile UEFI bootloader in C
        aarch64-linux-gnu-gcc -I/usr/include/efi -c -o boot.o uefi-boot.c -ffreestanding -nostdlib  -fno-stack-protector -fpic  -fshort-wchar -Wall

    else
        echo "Building standard bootloader..."

        # Assemble standard bootloader
        aarch64-linux-gnu-as -o boot.o boot.S
    fi

    echo "Building kernel..."

    # Compile kernel in C
    aarch64-linux-gnu-gcc -c -o kernel.o kernel.c -ffreestanding -nostdlib

    echo "Linking bootloader and kernel..."

    # Link bootloader and kernel into a single ELF
    aarch64-linux-gnu-ld -T kernel.ld -o boot.elf boot.o kernel.o
    run_qemu


}

run_qemu() {
    echo "Running QEMU..."
    qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -semihosting -serial mon:stdio -kernel boot.elf
}

# Check for arguments
if [ "$1" == "clean" ]; then
    clean
    exit 0
elif [ "$1" == "build" ]; then
    build "$2"
elif [ "$1" == "cleanbuild" ]; then
    clean
    build "$2"
else
    printf "build.sh usage...\n\n"
    echo "'./build.sh build' to build with boot.S"
    echo "'./build.sh build uefi' to build with uefi-boot.c"
    echo "'./build.sh clean' to remove old executables"
    echo "'./build.sh cleanbuild' to remove old executables and build"
fi
