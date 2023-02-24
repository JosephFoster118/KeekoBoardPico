cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(${CMAKE_SOURCE_DIR}/pico_sdk_import.cmake)

#set(APP_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/App")
# Build FreeRTOS
include(${CMAKE_SOURCE_DIR}/FreeRTOS_Kernel_import.cmake)

#set(FREERTOS_SRC_DIRECTORY $ENV{FREERTOS_SRC_DIRECTORY})
set(FREERTOS_CFG_DIRECTORY "${CMAKE_SOURCE_DIR}/Config")
project(KeekoPicoLib C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(include ${FREERTOS_CFG_DIRECTORY})
set(SOURCES
    src/Uart.cpp
    src/BoardUart.cpp
    src/USBUart.cpp
    src/DigitalIO.cpp
    src/BoardDigitalIO.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_uart
    tinyusb_host
    tinyusb_device
    tinyusb_board
    KeekoGeneralLib
)
