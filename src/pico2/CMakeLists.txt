cmake_minimum_required(VERSION 3.13)

# Project information
project(vOS-Kernel-Pico2 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-O2 -Wall")

set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)

set(PICO_SDK_PATH "/opt/pico-sdk/")

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

# Set directories (Relative to the root project directory, not the build directory)
set(BOOT_DIR "${CMAKE_SOURCE_DIR}/Boot")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/Lib")

# Define source files (relative to the root project directory)
set(BOOT_FILES "${BOOT_DIR}/stage1.c")
set(KERNEL_FILES "${LIB_DIR}/kernel.c")

# Ensure MODE is defined
if (NOT DEFINED MODE)
    set(MODE "arm")
endif()

# Mode-specific build options
if(MODE STREQUAL "arm" OR MODE STREQUAL "hybrid")
    message(STATUS "Building for ARM Cortex-M33 mode or Hybrid mode")

    # ARM-specific compilation flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m33 -mthumb")

    if(MODE STREQUAL "hybrid")
        list(APPEND BOOT_FILES "${BOOT_DIR}/stage2-hybrid.c")
    else()
        list(APPEND BOOT_FILES "${BOOT_DIR}/stage2-arm.S")
    endif()

    # Output file names
    set(OUTPUT_ELF "kernel-arm.elf")
    set(OUTPUT_BIN "kernel-arm.bin")

elseif(MODE STREQUAL "riscv")
    message(STATUS "Building for RISC-V mode")

    # RISC-V specific flags and tools
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR riscv32)
    set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)

    # RISC-V specific compilation flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=rv32im -mabi=ilp32")

    list(APPEND BOOT_FILES "${BOOT_DIR}/stage2-risc.S")

    # Output file names
    set(OUTPUT_ELF "kernel-riscv.elf")
    set(OUTPUT_BIN "kernel-riscv.bin")

else()
    message(FATAL_ERROR "Invalid MODE specified. Use MODE=arm, MODE=riscv, or MODE=hybrid")
endif()

# Add the source files to the executable
add_executable(${OUTPUT_ELF} ${BOOT_FILES} ${KERNEL_FILES})

# Link the Pico SDK standard library
target_link_libraries(${OUTPUT_ELF} pico_stdlib)

# Add extra outputs (UF2, BIN, HEX)
pico_add_extra_outputs(${OUTPUT_ELF})

# Linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker.ld")
set_target_properties(${OUTPUT_ELF} PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")

# Post-build: convert ELF to binary
add_custom_command(TARGET ${OUTPUT_ELF}
    POST_BUILD
    COMMAND ${CMAKE_C_COMPILER} -O binary ${OUTPUT_ELF} ${OUTPUT_BIN}
    COMMENT "Generating binary file: ${OUTPUT_BIN}"
)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove -f ${OUTPUT_ELF} ${OUTPUT_BIN}
    COMMENT "Cleaning build files"
)
