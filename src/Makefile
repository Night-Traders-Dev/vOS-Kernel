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

all: clean build run

ifeq ($(BOOTLOADER), "asm")
	@echo "Asm bootloader selected\n" | tee -a dmesg
	BOOT_SRC := $(BOOT_DIR)/boot.S
endif
ifeq ($(BOOTLOADER), "cxx")
	@echo "C bootloader selected\n" | tee -a dmesg
	BOOT_SRC := $(BOOT_DIR)/boot.c
endif

clean:
	@echo "Cleaning build files...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Cleaning build files..." >> dmesg
	rm -f $(BOOT_DIR)/boot.o $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(BUILD_DIR)/boot.elf $(LIB_DIR)/fs.o $(LIB_DIR)/vstring.o $(LIB_DIR)/scheduler.o $(LIB_DIR)/timer.o
	@echo "Clean complete.\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Clean complete." >> dmesg

build: $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/fs.o $(LIB_DIR)/vstring.o $(BOOT_DIR)/boot.o $(LIB_DIR)/scheduler.o $(LIB_DIR)/timer.o
	@echo "Compiling and linking bootloader and kernel...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling and linking bootloader and kernel..." >> dmesg
	$(LD) -T kernel.ld -o $(BUILD_DIR)/boot.elf $(BOOT_DIR)/boot.o $(LIB_DIR)/kernel.o $(LIB_DIR)/command_handler.o $(LIB_DIR)/fs.o $(LIB_DIR)/vstring.o $(LIB_DIR)/scheduler.o $(LIB_DIR)/timer.o
	@echo "Build complete.\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Build complete." >> dmesg

$(LIB_DIR)/kernel.o: $(LIB_DIR)/kernel.c
	@echo "Compiling kernel...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling kernel..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/kernel.o $(LIB_DIR)/kernel.c $(CFLAGS)

$(LIB_DIR)/command_handler.o: $(LIB_DIR)/command_handler.c
	@echo "Compiling command handler...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling command handler..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/command_handler.o $(LIB_DIR)/command_handler.c $(CFLAGS)

$(LIB_DIR)/fs.o: $(LIB_DIR)/fs.c
	@echo "Compiling filesystem...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling filesystem..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/fs.o $(LIB_DIR)/fs.c $(CFLAGS)

$(LIB_DIR)/vstring.o: $(LIB_DIR)/vstring.c
	@echo "Compiling vstring...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling vstring..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/vstring.o $(LIB_DIR)/vstring.c $(CFLAGS)

$(LIB_DIR)/scheduler.o: $(LIB_DIR)/scheduler.c
	@echo "Compiling scheduler...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling scheduler..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/scheduler.o $(LIB_DIR)/scheduler.c $(CFLAGS)

$(LIB_DIR)/timer.o: $(LIB_DIR)/timer.c
	@echo "Compiling timer...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Compiling timer..." >> dmesg
	$(CC) -c -o $(LIB_DIR)/timer.o $(LIB_DIR)/timer.c $(CFLAGS)

ifeq ($(BOOT_SRC), $(BOOT_DIR)/boot.c)
$(BOOT_DIR)/boot.o: $(BOOT_DIR)/boot.c
	$(CC) -c -o $(BOOT_DIR)/boot.o $(BOOT_DIR)/boot.c $(CFLAGS)
else
$(BOOT_DIR)/boot.o: $(BOOT_DIR)/boot.S
	$(AS) -o $(BOOT_DIR)/boot.o $(BOOT_DIR)/boot.S
endif

run:
	@echo "vOS built using $(BOOT_SRC)...\n"
	@echo "Running QEMU...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Running QEMU..." >> dmesg
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

debug:
	@echo "Running QEMU in debug mode\nConnect gdb...\n" | tee -a dmesg
	@date "+[%Y-%m-%d %H:%M:%S] Running QEMU in debug mode..." >> dmesg
	qemu-system-aarch64 -s -S -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(BUILD_DIR)/boot.elf

.PHONY: clean build run
