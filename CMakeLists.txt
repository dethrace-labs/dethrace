cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

set(DETHRACE_IDE_ARGUMENTS "" CACHE STRING "DethRace arguments (only used by MSVC when debugging")
set(DETHRACE_IDE_ROOT_DIR "" CACHE PATH "DethRace rootdir (only used by MSVC when debugging)")

project(dethrace C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(BUILD_TESTS "Build unit tests." OFF)

include(TestBigEndian)
test_big_endian(IS_BIGENDIAN)

find_package(SDL2 REQUIRED)

add_subdirectory(lib/libsmacker)
add_subdirectory(lib/glad)
add_subdirectory(lib/miniaudio)

add_subdirectory(src/harness)
add_subdirectory(src/S3)
add_subdirectory(src/BRSRC13)
add_subdirectory(src/DETHRACE)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
