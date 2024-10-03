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

# Function to log messages with a timestamp
log_with_timestamp = echo "`date +[%Y-%m-%d\ %H:%M:%S]` $1" >> dmesg

# Target rules
all: clean build run

all_uefi: clean build_uefi run

clean:
	@echo "Cleaning build files...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Cleaning build files..." >> dmesg
	rm -f $(BOOT_DIR)/boot.o $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/syscalls.o boot.elf $(BUILD_DIR)/boot.elf
	@echo "Clean complete.\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Clean complete." >> dmesg

build: $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/syscalls.o $(BOOT_DIR)/boot.o
	@echo "Compiling and linking bootloader and kernel...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Compiling and linking bootloader and kernel..." >> dmesg
	$(LD) -T kernel.ld -o $(BUILD_DIR)/boot.elf $(BOOT_DIR)/boot.o $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/syscalls.o
	@echo "Build complete.\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Build complete." >> dmesg

build_uefi: $(LIB_DIR)/kernel.o $(BOOT_DIR)/uefi-boot.o
	@echo "Linking UEFI bootloader with required libraries...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Linking UEFI bootloader with required libraries..." >> dmesg
	$(CC) -o $(BUILD_DIR)/boot.elf $(BOOT_DIR)/uefi-boot.o $(LIB_DIR)/kernel.o -nostdlib -T kernel-uefi.ld \
		-L/usr/local/lib -l:libefi.a -l:libgnuefi.a
	@echo "UEFI Build complete.\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` UEFI Build complete." >> dmesg

$(LIB_DIR)/kernel.o: $(LIB_DIR)/kernel.c
	@echo "Compiling kernel...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Compiling kernel..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/kernel.o $(LIB_DIR)/kernel.c $(CFLAGS)

$(LIB_DIR)/command_handler.o: $(LIB_DIR)/command_handler.c
	@echo "Compiling command handler...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Compiling command handler..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/command_handler.o $(LIB_DIR)/command_handler.c $(CFLAGS)

$(LIB_DIR)/syscalls.o: $(LIB_DIR)/syscalls.c
	@echo "Compiling syscalls...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Compiling syscalls..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/syscalls.o $(LIB_DIR)/syscalls.c $(CFLAGS)

$(BOOT_DIR)/boot.o: $(BOOT_DIR)/boot.S
	$(AS) -o $(BOOT_DIR)/boot.o $(BOOT_DIR)/boot.S

$(BOOT_DIR)/uefi-boot.o: $(BOOT_DIR)/uefi-boot.c
	$(CC) -I/usr/local/include/efi -I/usr/include/efi -c -o $(BOOT_DIR)/uefi-boot.o $(BOOT_DIR)/uefi-boot.c $(CFLAGS) $(UEFI_FLAGS)

run:
	@echo "Running QEMU...\n" | tee -a dmesg
	@echo "`date +[%Y-%m-%d\ %H:%M:%S]` Running QEMU..." >> dmesg
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

.PHONY: clean build build_uefi run
