cmake_minimum_required(VERSION 3.25)
project("AIO_UveDX_CI1")

option(COMPILE_EXAMPLES "Compile the examples" OFF)

add_subdirectory("UveDX")

if(COMPILE_EXAMPLES)
    add_subdirectory("examples/DVD")
endif()

add_subdirectory("Game")
