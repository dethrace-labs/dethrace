cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

project(smacker C)

add_library(smacker STATIC)

target_include_directories(smacker PUBLIC
    .
)

if(NOT MSVC)
    target_compile_options(smacker PRIVATE -Wall)
else()
    target_compile_definitions(smacker PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

target_sources(smacker PRIVATE
    smacker.c
    smk_bitstream.c
    smk_hufftree.c
    smacker.h
    smk_bitstream.h
    smk_hufftree.h
    smk_malloc.h
)
