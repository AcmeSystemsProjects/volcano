cmake_minimum_required(VERSION 3.3)
project(turnsInTheRing)

set(SOURCE_FILES
        ../../../lib/gpio/gpioFastLib.c
        sumoMotor.c
        sensor.c
        main.c
        init.c)

add_executable(turnsInTheRing ${SOURCE_FILES})