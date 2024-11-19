# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD waveshare_rp2040_zero CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(main C CXX ASM)

pico_sdk_init()
include_directories(includes)
add_executable(main
    libraries/link_protocol.c
    libraries/queue_data.c
    libraries/shared.c
    libraries/trade.c
    libraries/utilities.c
    libraries/worker.c
    libraries/flash_storage.c
    )

pico_generate_pio_header(main ${CMAKE_CURRENT_LIST_DIR}/pio/gbacom.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/includes)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_sources(main PRIVATE main.c)

target_link_libraries(main
    PRIVATE
    pico_stdlib
    hardware_pio
    hardware_dma
    pico_multicore
    pico_platform
)

pico_add_extra_outputs(main)