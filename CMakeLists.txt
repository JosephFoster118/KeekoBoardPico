
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

#set(APP_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/App")
# Build FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

set(LIB "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB})


# Set project data
set(PROJECT_NAME "ROOT")
project(${PROJECT_NAME} C CXX ASM)
#set(FREERTOS_SRC_DIRECTORY $ENV{FREERTOS_SRC_DIRECTORY})
set(FREERTOS_CFG_DIRECTORY "${CMAKE_SOURCE_DIR}/KeekoBoardPico/Config")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fexceptions -Wno-psabi)

include_directories(Config)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
pico_sdk_init()

add_subdirectory(KeekoGeneralLib)
add_subdirectory(KeekoPicoLib)
add_subdirectory(KeekoBoardPico)

add_dependencies(KeekoPicoLib KeekoGeneralLib)
add_dependencies(KeekoBoardPico KeekoPicoLib KeekoGeneralLib)


