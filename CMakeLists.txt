cmake_minimum_required(VERSION 3.20.0)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${BOARD}.overlay")
  set(mcuboot_DTC_OVERLAY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${BOARD}.overlay")
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

project(passr)

target_sources(app PRIVATE src/main.c)
