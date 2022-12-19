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

set(IO_PLATFORMS "SDL_OpenGL;GLFW_OpenGL")
set(IO_PLATFORM "SDL_OpenGL" CACHE STRING "Dethrace platform")
set_property(CACHE IO_PLATFORM PROPERTY STRINGS "${IO_PLATFORMS}")
if(NOT IO_PLATFORM IN_LIST IO_PLATFORMS)
    message(FATAL_ERROR "Invalid IO_PLATFORM=${IO_PLATFORM} (choices=${IO_PLATFORMS})")
endif()

set(RENDERER_OPENGL FALSE)
if(IO_PLATFORM MATCHES ".*OpenGL$")
    set(RENDERER_OPENGL TRUE)
else()
    message(FATAL_ERROR "Unknown Renderer")
endif()

set(IOPLATFORM_GLFW FALSE)
set(IOPLATFORM_SDL FALSE)
if(IO_PLATFORM MATCHES "^GLFW")
    set(IOPLATFORM_GLFW TRUE)
elseif(IO_PLATFORM MATCHES "^SDL")
    set(IOPLATFORM_SDL TRUE)
else()
    message(FATAL_ERROR "Unknown io platform")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(DETHRACE_IDE_ARGUMENTS "" CACHE STRING "DethRace arguments (only used by MSVC when debugging")
set(DETHRACE_IDE_ROOT_DIR "" CACHE PATH "DethRace rootdir (only used by MSVC when debugging)")
option(BUILD_TESTS "Build unit tests." OFF)
option(DETHRACE_INSTALL "Add install target" OFF)
option(DETHRACE_WERROR "Treat warnings as errors")

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

if(IO_PLATFORM STREQUAL "SDL_OpenGL")
    find_package(SDL2 REQUIRED)
elseif(IO_PLATFORM STREQUAL "GLFW_OpenGL")
    find_package(glfw3 REQUIRED CONFIG)
endif()

option(DETHRACE_DEBUGUI "Build Dethrace with debug ui" ON)

if(DETHRACE_DEBUGUI)
    enable_language(CXX)
    add_subdirectory(lib/imgui EXCLUDE_FROM_ALL)
    add_subdirectory(lib/cimgui EXCLUDE_FROM_ALL)
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

option(DETHRACE_FIX_BUGS "Fix Dethrace bugs" ON)

add_subdirectory(src/harness)
add_subdirectory(src/S3)
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
