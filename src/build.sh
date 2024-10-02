#!/bin/bash

# Function to clean the build artifacts
clean() {
    echo "Cleaning build files..."
    rm -f boot.o kernel.o boot.elf
    echo "Clean complete."
}

# Function to build the bootloader and kernel
build() {
    echo "Building kernel..."

    # Compile kernel in C
    aarch64-linux-gnu-gcc -c -o kernel.o kernel.c -ffreestanding -nostdlib

    if [ "$1" == "uefi" ]; then
        echo "Building UEFI bootloader..."

        # Compile UEFI bootloader in C
        aarch64-linux-gnu-gcc -I/usr/local/include/efi -I/usr/include/efi -c -o boot.o uefi-boot.c \
            -ffreestanding -nostdlib -fno-stack-protector -fpic -fshort-wchar -Wall

        echo "Linking UEFI bootloader with required libraries..."

        # Link UEFI bootloader with libraries
        aarch64-linux-gnu-gcc -o boot.elf boot.o kernel.o -nostdlib -T kernel-uefi.ld \
            -L/usr/local/lib -l:libefi.a -l:libgnuefi.a
    else
        echo "Building standard BIOS bootloader..."

        # Assemble standard bootloader
        aarch64-linux-gnu-as -o boot.o boot.S
        echo "Linking bootloader and kernel..."

        # Link bootloader and kernel into a single ELF
        aarch64-linux-gnu-ld -T kernel.ld -o boot.elf boot.o kernel.o
    fi

    run_qemu
}

run_qemu() {
    echo "Running QEMU..."
    qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -semihosting -serial mon:stdio -kernel boot.elf
}

# Check for arguments
case "$1" in
    clean)
        clean
        ;;
    build)
        build "$2"
        ;;
    cleanbuild)
        clean
        build "$2"
        ;;
    *)
        echo "Usage: $0 {clean|build|cleanbuild} [uefi]"
        echo "'$0 build' to build with boot.S"
        echo "'$0 build uefi' to build with uefi-boot.c"
        echo "'$0 clean' to remove old executables"
        echo "'$0 cleanbuild' to remove old executables and build"
        ;;
esac
