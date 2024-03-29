cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

project(dethrace C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include(CheckCCompilerFlag)
include(GNUInstallDirs)
include(TestBigEndian)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" DETHRACE_VERSION)
    string(STRIP "${DETHRACE_VERSION}" DETHRACE_VERSION)
else()
    include(GetGitRevisionDescription)
    git_describe(DETHRACE_VERSION)
endif()

message(STATUS "dethrace version ${DETHRACE_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(DETHRACE_IDE_ARGUMENTS "" CACHE STRING "DethRace arguments (only used by MSVC when debugging")
set(DETHRACE_IDE_ROOT_DIR "" CACHE PATH "DethRace rootdir (only used by MSVC when debugging)")
option(BUILD_TESTS "Build unit tests." OFF)
option(DETHRACE_INSTALL "Add install target" OFF)
option(DETHRACE_WERROR "Treat warnings as errors")
option(DETHRACE_FIX_BUGS "Fix Dethrace bugs" ON)

function(add_compile_flag_if_supported TARGET FLAG)
    string(MAKE_C_IDENTIFIER "${FLAG}" FLAG_TO_IDENTIFIER)
    set(HAVE_FLAG_VARIABLE_NAME "HAVE_${FLAG_TO_IDENTIFIER}")
    check_c_compiler_flag("${FLAG}" "${HAVE_FLAG_VARIABLE_NAME}")
    if(${HAVE_FLAG_VARIABLE_NAME})
        target_compile_options("${TARGET}" PRIVATE "${FLAG}")
    endif()
endfunction()

function(add_compile_flags_if_supported TARGET)
    foreach(FLAG ${ARGN})
        add_compile_flag_if_supported("${TARGET}" "${FLAG}")
    endforeach()
endfunction()

test_big_endian(IS_BIGENDIAN)

find_package(SDL2 CONFIG)
if(NOT SDL2_FOUND)
    find_package(SDL2 MODULE REQUIRED)
endif()

add_subdirectory(lib/libsmacker)
add_subdirectory(lib/glad)
add_subdirectory(lib/miniaudio)

add_library(compile_with_werror INTERFACE)

if(DETHRACE_WERROR)
    if(MSVC)
        target_compile_options(compile_with_werror INTERFACE /WX)
    else()
        target_compile_options(compile_with_werror INTERFACE -Werror)
    endif()
endif()



add_subdirectory(src/harness)
add_subdirectory(src/S3)
add_subdirectory(src/smackw32)
add_subdirectory(src/BRSRC13)
add_subdirectory(src/DETHRACE)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(DETHRACE_INSTALL)
    install(FILES LICENSE
        DESTINATION "${CMAKE_INSTALL_DOCDIR}"
    )

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^([xX]86|[xX]86_64|[iI].86|AMD64)$")
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(DETHRACE_ARCH x86)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(DETHRACE_ARCH amd64)
        else()
            message(SEND_ERROR "Unknown CMAKE_SIZEOF_VOID_P (${CMAKE_SIZEOF_VOID_P})")
        endif()
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(ARM|arm|ARM64|aarch64)$")
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(DETHRACE_ARCH arm)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(DETHRACE_ARCH aarch64)
        else()
            message(SEND_ERROR "Unknown CMAKE_SIZEOF_VOID_P (${CMAKE_SIZEOF_VOID_P})")
        endif()
    else()
        message(SEND_ERROR "Unknown CMAKE_SYSTEM_PROCESSOR (${CMAKE_SYSTEM_PROCESSOR})")
    endif()

    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "${DETHRACE_VERSION}")
    string(TOLOWER "${CMAKE_SYSTEM_NAME}-${DETHRACE_ARCH}" CPACK_SYSTEM_NAME)

    set(CPACK_PACKAGE_DIRECTORY dist)

    if(MSVC)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()

    include(CPack)
endif()
