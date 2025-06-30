cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

project(inih C)

add_library(inih STATIC)

target_include_directories(inih
    SYSTEM PUBLIC
    .
)

target_sources(inih PRIVATE
    ini.c
)
