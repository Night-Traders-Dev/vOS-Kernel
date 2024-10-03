# Set the build directory
BUILD_DIR = ../build

# Paths for source and include files
BOOT_DIR = Boot
LIB_DIR = Lib
INCLUDE_DIR = Include

# Compiler and linker flags
CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
CFLAGS = -ffreestanding -nostdlib -I$(INCLUDE_DIR)
UEFI_FLAGS = -fno-stack-protector -fpic -fshort-wchar -Wall

# Target rules
all: clean build run

all_uefi: clean build_uefi run

clean:
	@echo "Cleaning build files...\n"
	rm -f $(BOOT_DIR)/boot.o $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/syscalls.o boot.elf $(BUILD_DIR)/boot.elf
	@echo "Clean complete.\n"

build: $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/syscalls.o $(BOOT_DIR)/boot.o
	@echo "Linking bootloader and kernel...\n"
	$(LD) -T kernel.ld -o $(BUILD_DIR)/boot.elf $(BOOT_DIR)/boot.o $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/syscalls.o
	@echo "Build complete.\n"

build_uefi: $(LIB_DIR)/kernel.o $(BOOT_DIR)/uefi-boot.o
	@echo "Linking UEFI bootloader with required libraries...\n"
	$(CC) -o $(BUILD_DIR)/boot.elf $(BOOT_DIR)/uefi-boot.o $(LIB_DIR)/kernel.o -nostdlib -T kernel-uefi.ld \
		-L/usr/local/lib -l:libefi.a -l:libgnuefi.a

$(LIB_DIR)/kernel.o: $(LIB_DIR)/kernel.c
	$(CC) -c -o $(LIB_DIR)/kernel.o $(LIB_DIR)/kernel.c $(CFLAGS)

$(LIB_DIR)/command_handler.o: $(LIB_DIR)/command_handler.c
	$(CC) -c -o $(LIB_DIR)/command_handler.o $(LIB_DIR)/command_handler.c $(CFLAGS)

$(LIB_DIR)/syscalls.o: $(LIB_DIR)/syscalls.c
	$(CC) -c -o $(LIB_DIR)/syscalls.o $(LIB_DIR)/syscalls.c $(CFLAGS)

$(BOOT_DIR)/boot.o: $(BOOT_DIR)/boot.S
	$(AS) -o $(BOOT_DIR)/boot.o $(BOOT_DIR)/boot.S

$(BOOT_DIR)/uefi-boot.o: $(BOOT_DIR)/uefi-boot.c
	$(CC) -I/usr/local/include/efi -I/usr/include/efi -c -o $(BOOT_DIR)/uefi-boot.o $(BOOT_DIR)/uefi-boot.c $(CFLAGS) $(UEFI_FLAGS)

run:
	@echo "Running QEMU...\n\n"
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

.PHONY: clean build build_uefi run
