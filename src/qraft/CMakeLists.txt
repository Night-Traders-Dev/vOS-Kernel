cmake_minimum_required(VERSION 3.12)

# Set the path to the Pico SDK
set(PICO_SDK_PATH "/opt/pico-sdk/")
set(PICO_BOARD pico2)
# Set default build type to ARM if not specified
if(${BUILD_TYPE} STREQUAL "ARM")
    set(PICO_PLATFORM rp2350)
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

    set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/arm-none-eabi-g++")
    set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-as")

    set(CMAKE_C_FLAGS "-mcpu=cortex-m33 -mthumb")
elseif(${BUILD_TYPE} STREQUAL "RISC-V")
    set(PICO_PLATFORM rp2350-riscv)
    set(PICO_COMPILER pico_riscv_gcc)
    set(PICO_GCC_TRIPLE to riscv32-unknown-elf)

    set(CMAKE_C_COMPILER "/opt/pico-riscv/bin/riscv32-unknown-elf-gcc")
    set(CMAKE_CXX_COMPILER "/opt/pico-riscv/bin/riscv32-unknown-elf-g++")
    set(CMAKE_ASM_COMPILER "/opt/pico-riscv/bin/riscv32-unknown-elf-as")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=rv32imac_zicsr_zifencei_zbs -mabi=ilp32")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -march=rv32imac_zicsr_zifencei_zbs -mabi=ilp32")
else()
    message(FATAL_ERROR "Unknown BUILD_TYPE: ${BUILD_TYPE}")
endif()


# Include Pico SDK files
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Project configuration
project(qraft)


# Initialize the Pico SDK
pico_sdk_init()

# Add source files for both ARM and RISC-V
set(SRC_ARM src/qraft-arm.c)
set(SRC_RISC src/qraft-risc.c)

if(${BUILD_TYPE} STREQUAL "ARM")
    message(STATUS "Building for ARM architecture")
    add_executable(qraft-arm ${SRC_ARM})
    target_link_libraries(qraft-arm pico_stdlib hardware_uart)
    pico_add_extra_outputs(qraft-arm)

elseif(${BUILD_TYPE} STREQUAL "RISC-V")
    message(STATUS "Building for RISC-V architecture")
    add_executable(qraft-risc ${SRC_RISC})
    target_link_libraries(qraft-risc pico_stdlib hardware_uart)
    pico_add_extra_outputs(qraft-risc)

else()
    message(FATAL_ERROR "Unknown BUILD_TYPE: ${BUILD_TYPE}")
endif()
