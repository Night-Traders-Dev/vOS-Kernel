BUILD_DIR = ../build
BOOT_DIR = Boot
LIB_DIR = Lib
INCLUDE_DIR = Include

CC = clang
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
CFLAGS = -ffreestanding -nostdlib -fpic -fno-stack-protector -I$(INCLUDE_DIR)

BOOTLOADER = asm
BOOT_SRC = $(BOOT_DIR)/boot.S
OBJS = $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/fs.o \
       $(LIB_DIR)/vstring.o $(LIB_DIR)/scheduler.o $(LIB_DIR)/timer.o \
       $(BOOT_DIR)/boot.o $(LIB_DIR)/uart.o $(LIB_DIR)/gic.o

all: clean build run

ifeq ($(BOOTLOADER), cxx)
BOOT_SRC = $(BOOT_DIR)/boot.c
endif

clean:
	@echo "Cleaning build files..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Cleaning build files..." >> dmesg
	rm -f $(OBJS) $(BUILD_DIR)/boot.elf
	@echo "Clean complete." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Clean complete." >> dmesg

build: $(OBJS)
	@echo "Compiling and linking bootloader and kernel..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling and linking bootloader and kernel..." >> dmesg
	$(LD) -T kernel.ld -o $(BUILD_DIR)/boot.elf $(OBJS)
	@echo "Build complete." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Build complete." >> dmesg

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

run:
	@echo "vOS built using $(BOOT_SRC)..."
	@echo "Running QEMU..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Running QEMU..." >> dmesg
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

debug:
	@echo "Running QEMU in debug mode. Connect gdb..." | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Running QEMU in debug mode..." >> dmesg
	qemu-system-aarch64 -s -S -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

.PHONY: clean build run debug
