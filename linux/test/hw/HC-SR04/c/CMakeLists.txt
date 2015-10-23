cmake_minimum_required(VERSION 3.3)
project(hc_sr04)


set(SOURCE_FILES test_hc-sr04.c
                ncursesProg.c)

# gcc -lncurses
link_libraries(ncurses)

add_executable(hc_sr04 ${SOURCE_FILES})