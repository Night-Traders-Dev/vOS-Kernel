set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

# Define the cross-compiler toolchain
set(CMAKE_C_COMPILER "/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "/bin/arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "/bin/arm-none-eabi-as")

set(CMAKE_C_FLAGS "-mcpu=cortex-m33 -mthumb")
