cmake_minimum_required(VERSION 3.12)

# Set the path to the Pico SDK
set(PICO_SDK_PATH "/opt/pico-sdk/")

# Include Pico SDK files
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Project configuration
project(qraft)

# Initialize the Pico SDK
pico_sdk_init()

# Set default build type to ARM if not specified
if(NOT DEFINED BUILD_TYPE)
    set(BUILD_TYPE "ARM")
endif()

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
