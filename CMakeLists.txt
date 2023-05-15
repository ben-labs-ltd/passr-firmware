cmake_minimum_required(VERSION 3.20.0)

set(BOARD rpi_pico)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(passr)

target_sources(app PRIVATE src/main.c)
