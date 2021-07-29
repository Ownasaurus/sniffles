cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(sniffles C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(sniffles)

pico_generate_pio_header(sniffles ${CMAKE_CURRENT_LIST_DIR}/sniffles.pio)

target_sources(sniffles PRIVATE sniffles.c)

target_link_libraries(sniffles PRIVATE
        pico_stdlib
        hardware_pio
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(sniffles 1)
pico_enable_stdio_uart(sniffles 0)

pico_add_extra_outputs(sniffles)

