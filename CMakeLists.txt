
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

#set(APP_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/App")
# Build FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

set(LIB "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB})


# Set project data
set(PROJECT_NAME "FREERTOS_PROJECT")
set(VERSION_NUMBER "1.0.0")
set(BUILD_NUMBER "1")
#set(FREERTOS_SRC_DIRECTORY $ENV{FREERTOS_SRC_DIRECTORY})
set(FREERTOS_CFG_DIRECTORY "${CMAKE_SOURCE_DIR}/Config")
project(PROJECT_NAME C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fexceptions)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
pico_sdk_init()


set(SOURCES
    src/main.cpp
    src/BoardMessage.cpp
    src/KeekoBoard.cpp
    src/usb_descriptors.c
    src/KeekoMessage.cpp
)

include_directories(include Config)
#include_directories(include $ENV{PICO_SDK_PATH}/**
#    ${FREERTOS_CFG_DIRECTORY}/
#    ${FREERTOS_SRC_DIRECTORY}/include
#    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
#)

add_executable(KeekoBoardPico ${SOURCES})
#pico_add_extra_outputs(FRTOS_Test)

# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)
target_link_libraries(KeekoBoardPico
            pico_stdlib
	    FreeRTOS-Kernel
	    FreeRTOS-Kernel-Heap1
            hardware_i2c
            hardware_uart
            hardware_flash
            tinyusb_host
            tinyusb_device
            tinyusb_board
            hardware_timer          
)
pico_add_extra_outputs(KeekoBoardPico)
