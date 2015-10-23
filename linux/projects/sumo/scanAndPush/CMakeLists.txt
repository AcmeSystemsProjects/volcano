cmake_minimum_required(VERSION 3.3)
project(scanAndPush)


set(SOURCE_FILES testScanPush.c
    sumoMotor.c
    sensor.c)


add_executable(scanAndPush ${SOURCE_FILES})