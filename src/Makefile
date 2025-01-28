BUILD_DIR = ../build
BOOT_DIR = Boot
LIB_DIR = Lib
INCLUDE_DIR = Include

CC = clang
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
CFLAGS_COMMON = -ffreestanding -nostdlib -fpic -fno-stack-protector -I$(INCLUDE_DIR)

# QEMU-specific flags
CFLAGS_QEMU = -mcpu=cortex-a72 -march=armv8-a
LDFLAGS_QEMU = -T qemu.ld

# Raspberry Pi 4-specific flags
CFLAGS_RPI = -mcpu=cortex-a72 -march=armv8-a
LDFLAGS_RPI = -T rpi.ld

# Bootloader selection
BOOTLOADER = asm
BOOT_SRC = $(BOOT_DIR)/boot.S
OBJS = $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/fs.o \
       $(LIB_DIR)/vstring.o $(LIB_DIR)/scheduler.o \
       $(BOOT_DIR)/boot.o $(LIB_DIR)/uart.o $(LIB_DIR)/watch.o \
       $(LIB_DIR)/heartbeat.o

all: clean qemu

ifeq ($(BOOTLOADER), cxx)
BOOT_SRC = $(BOOT_DIR)/boot.c
endif

clean:
	@echo "Cleaning build files..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Cleaning build files..." >> dmesg
	rm -f $(OBJS) $(BUILD_DIR)/*
	@echo "Clean complete." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Clean complete." >> dmesg

# QEMU build
qemu: CFLAGS=$(CFLAGS_COMMON) $(CFLAGS_QEMU)
qemu: LDFLAGS=$(LDFLAGS_QEMU)
qemu: $(OBJS)
	@echo "Compiling and linking for QEMU..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling and linking for QEMU..." >> dmesg
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/qemu.elf $(OBJS)
	@echo "QEMU build complete." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] QEMU build complete." >> dmesg

# Raspberry Pi 4 build
rpi: CFLAGS=$(CFLAGS_COMMON) $(CFLAGS_RPI)
rpi: LDFLAGS=$(LDFLAGS_RPI)
rpi: $(OBJS)
	@echo "Compiling and linking for Raspberry Pi 4..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling and linking for Raspberry Pi 4..." >> dmesg
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/rpi.elf $(OBJS)
	@echo "Building bootable Raspberry Pi 4 image..." | tee -a dmesg
	mkdir -p $(BUILD_DIR)/boot
	cp $(BUILD_DIR)/rpi.elf $(BUILD_DIR)/boot/kernel8.img
	@echo "Raspberry Pi 4 build complete." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Raspberry Pi 4 build complete." >> dmesg

# Pattern rule for compiling C source files
$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	@echo "Compiling $<..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling $<..." >> dmesg
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule for compiling the bootloader
$(BOOT_DIR)/boot.o: $(BOOT_SRC)
	@echo "Compiling bootloader ($<)..." | tee -a dmesg
	$(if $(findstring .c,$<), \
		$(CC) -c -o $@ $< $(CFLAGS), \
		$(AS) -o $@ $<)

run-qemu:
	@echo "Running QEMU..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Running QEMU..." >> dmesg
	qemu-system-aarch64 -M virt -cpu cortex-a72 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/qemu.elf

run-rpi:
	@echo "Preparing Raspberry Pi 4 SD card..." | tee -a dmesg
	@echo "Copy $(BUILD_DIR)/boot to the boot partition of an SD card."
	@echo "Insert the SD card into the Raspberry Pi 4 and boot."

debug-qemu:
	@echo "Running QEMU in debug mode. Connect gdb..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Running QEMU in debug mode..." >> dmesg
	qemu-system-aarch64 -s -S -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/qemu.elf

.PHONY: clean qemu rpi run-qemu run-rpi debug-qemu
