cmake_minimum_required(VERSION 3.28)
project(obj)

add_executable(obj_test
        main.c
        obj.h
        obj.c
)

#target_compile_options(obj_test PUBLIC -O3)