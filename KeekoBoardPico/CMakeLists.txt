
cmake_minimum_required(VERSION 3.13)
enable_language( C CXX ASM )
# Pull in SDK (must be before project)
#include(${CMAKE_SOURCE_DIR}/pico_sdk_import.cmake)

#set(APP_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/App")
# Build FreeRTOS
#include(${CMAKE_SOURCE_DIR}/FreeRTOS_Kernel_import.cmake)

# Set project data
#set(FREERTOS_SRC_DIRECTORY $ENV{FREERTOS_SRC_DIRECTORY})
project(KeekoBoardPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)



set(SOURCES
    src/main.cpp
    src/KeekoBoard.cpp
    src/usb_descriptors.c
    src/KeekoBoardContext.cpp
)

include_directories(include)
#include_directories(include $ENV{PICO_SDK_PATH}/**
#    ${FREERTOS_CFG_DIRECTORY}/
#    ${FREERTOS_SRC_DIRECTORY}/include
#    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
#)
#
add_executable(${PROJECT_NAME} ${SOURCES})


#add_subdirectory(KeekoPicoLib)

# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)
target_link_libraries(${PROJECT_NAME}
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
    KeekoPicoLib    
)


#add_dependencies(KeekoBoardPico KeekoPicoLib)
pico_add_extra_outputs(${PROJECT_NAME})
