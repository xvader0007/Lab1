cmake_minimum_required(VERSION 3.28)
project(Lab1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Lab1
        complex.c
        complex.h
        field_info.h
        main.c
        polynomial.h
        polynomial.c
        field_info.c
        tests.c
        tests.h
)
