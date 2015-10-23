cmake_minimum_required(VERSION 3.3)
project(es1)


set(SOURCE_FILES
        test.c
        init.c
        robotArm.c
        i2c.c
        demostration.c
)

# gcc -lm
link_libraries(m)

add_executable(es1 ${SOURCE_FILES})