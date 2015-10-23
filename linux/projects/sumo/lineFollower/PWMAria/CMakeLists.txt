cmake_minimum_required(VERSION 3.3)
project(PWMAria)
SET( lineFollower_VERSION_MAJOR 1)
SET( lineFollower_VERSION_MINOR 2)
configure_file ("${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h")
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(SOURCE_FILES main.c
init.c
line.c
sumoMotor.c
../../../../lib/gpio/gpioFastLib.c)



add_executable(PWMAria ${SOURCE_FILES})
