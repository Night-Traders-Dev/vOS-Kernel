# Set the build directory
BUILD_DIR = ../build

# Compiler and linker flags
CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
CFLAGS = -ffreestanding -nostdlib
UEFI_FLAGS = -fno-stack-protector -fpic -fshort-wchar -Wall

# Target rules
all: build

clean:
	@echo "Cleaning build files..."
	rm -f boot.o kernel.o command_handler.o
	@echo "Clean complete."

build: kernel.o command_handler.o boot.o
	@echo "Linking bootloader and kernel..."
	$(LD) -T kernel.ld -o $(BUILD_DIR)/boot.elf boot.o kernel.o command_handler.o
	@echo "Build complete."

build_uefi: kernel.o boot_uefi.o
	@echo "Linking UEFI bootloader with required libraries..."
	$(CC) -o $(BUILD_DIR)/boot.elf boot.o kernel.o -nostdlib -T kernel-uefi.ld \
	    -L/usr/local/lib -l:libefi.a -l:libgnuefi.a

kernel.o: kernel.c
	$(CC) -c -o kernel.o kernel.c $(CFLAGS)

command_handler.o: command_handler.c
	$(CC) -c -o command_handler.o command_handler.c $(CFLAGS)

boot.o: boot.S
	$(AS) -o boot.o boot.S

boot_uefi.o: uefi-boot.c
	$(CC) -I/usr/local/include/efi -I/usr/include/efi -c -o boot.o uefi-boot.c $(CFLAGS) $(UEFI_FLAGS)

run:
	@echo "Running QEMU..."
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

.PHONY: clean build build_uefi run
