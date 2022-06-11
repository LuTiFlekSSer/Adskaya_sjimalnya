cmake_minimum_required(VERSION 3.16)
project(Adskaya\ sjimalnya\ v14.88 C)

set(CMAKE_C_FLAGS "-O3")
set(CMAKE_C_STANDARD 99)

MATH(EXPR stack_size "100 * 1024 * 1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

add_executable(Adskaya_sjimalnya_v14.88 "Adskaya sjimalnya v14.88.c" derevo.c derevo.h keyio.c keyio.h bit2char.c bit2char.h fileio.c fileio.h)
