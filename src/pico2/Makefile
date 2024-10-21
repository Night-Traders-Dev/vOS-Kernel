ARM_GCC = arm-none-eabi-gcc
RISCV_GCC = riscv64-unknown-elf-gcc
CFLAGS = -O2 -Wall

BOOT_DIR = Boot
LIB_DIR = Lib

BOOT_OBJ = $(BOOT_DIR)/stage1.o $(BOOT_DIR)/stage2-arm.o $(BOOT_DIR)/stage2-risc.o $(BOOT_DIR)/stage2-hybrid.o
KERNEL_OBJ = $(LIB_DIR)/kernel.o

all: mode check

mode:
ifeq ($(MODE),arm)
	$(ARM_GCC) $(CFLAGS) -c $(BOOT_DIR)/stage1.c -o $(BOOT_DIR)/stage1.o
	$(ARM_GCC) $(CFLAGS) -c $(BOOT_DIR)/stage2-arm.S -o $(BOOT_DIR)/stage2-arm.o
	$(ARM_GCC) $(CFLAGS) -c $(LIB_DIR)/kernel.c -o $(LIB_DIR)/kernel.o
	$(ARM_GCC) -T linker.ld $(BOOT_OBJ) $(KERNEL_OBJ) -o kernel-arm.elf
	$(ARM_GCC) -O binary kernel-arm.elf kernel-arm.bin

else ifeq ($(MODE),riscv)
	$(RISCV_GCC) $(CFLAGS) -c $(BOOT_DIR)/stage1.c -o $(BOOT_DIR)/stage1.o
	$(RISCV_GCC) $(CFLAGS) -c $(BOOT_DIR)/stage2-risc.S -o $(BOOT_DIR)/stage2-risc.o
	$(RISCV_GCC) $(CFLAGS) -c $(LIB_DIR)/kernel.c -o $(LIB_DIR)/kernel.o
	$(RISCV_GCC) -T linker.ld $(BOOT_OBJ) $(KERNEL_OBJ) -o kernel-riscv.elf
	$(RISCV_GCC) -O binary kernel-riscv.elf kernel-riscv.bin

else ifeq ($(MODE),hybrid)
	$(ARM_GCC) $(CFLAGS) -c $(BOOT_DIR)/stage1.c -o $(BOOT_DIR)/stage1.o
	$(ARM_GCC) $(CFLAGS) -c $(BOOT_DIR)/stage2-hybrid.c -o $(BOOT_DIR)/stage2-hybrid.o
	$(ARM_GCC) $(CFLAGS) -c $(LIB_DIR)/kernel.c -o $(LIB_DIR)/kernel.o
	$(ARM_GCC) -T linker.ld $(BOOT_OBJ) $(KERNEL_OBJ) -o kernel-hybrid.elf
	$(ARM_GCC) -O binary kernel-hybrid.elf kernel-hybrid.bin

else
	$(error "Please specify MODE=arm, MODE=riscv, or MODE=hybrid")
endif

check:
	@echo "Building in $(MODE) mode."

clean:
	rm -f $(BOOT_DIR)/*.o $(LIB_DIR)/*.o *.elf *.bin
