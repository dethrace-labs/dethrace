cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

project(dethrace C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

if(MSVC)
    set(CMAKE_INSTALL_BINDIR "." CACHE PATH "User executables")
    set(CMAKE_INSTALL_DOCDIR "." CACHE PATH "Documentation root")
endif()

include(CheckCCompilerFlag)
include(CMakeDependentOption)
include(TestBigEndian)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" DETHRACE_VERSION)
    string(STRIP "${DETHRACE_VERSION}" DETHRACE_VERSION)
else()
    include(GetGitRevisionDescription)
    git_describe(DETHRACE_VERSION)

    if(NOT DETHRACE_VERSION)
        set(DETHRACE_VERSION "unknown")
    endif()
endif()

message(STATUS "dethrace version ${DETHRACE_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(DETHRACE_IDE_ARGUMENTS "" CACHE STRING "DethRace arguments (only used by MSVC when debugging")
set(DETHRACE_IDE_ROOT_DIR "" CACHE PATH "DethRace rootdir (only used by MSVC when debugging)")
option(BUILD_TESTS "Build unit tests." OFF)
option(DETHRACE_INSTALL "Add install target" OFF)
option(DETHRACE_WERROR "Treat warnings as errors")
option(DETHRACE_FIX_BUGS "Fix Dethrace bugs" ON)
option(DETHRACE_3DFX_PATCH "Include changes from VOODOO2C.EXE" ON)
option(DETHRACE_SOUND_ENABLED "Include audio support" ON)

function(add_compile_flag_if_supported TARGET FLAG)
    cmake_parse_arguments(ARGS "" "" "LANGUAGES" ${ARGN})

    if(NOT ARGS_LANGUAGES)
        set(ARGS_LANGUAGES C CXX)
    endif()

    string(MAKE_C_IDENTIFIER "${FLAG}" FLAG_TO_IDENTIFIER)
    set(HAVE_FLAG_VARIABLE_NAME "HAVE_${FLAG_TO_IDENTIFIER}")
    check_c_compiler_flag("${FLAG}" "${HAVE_FLAG_VARIABLE_NAME}")

    if(${HAVE_FLAG_VARIABLE_NAME})
        string(REPLACE ";" "," ARGS_LANGUAGES "${ARGS_LANGUAGES}")
        target_compile_options("${TARGET}" PRIVATE "$<$<COMPILE_LANGUAGE:${ARGS_LANGUAGES}>:${FLAG}>")
    endif()
endfunction()

function(add_compile_flags_if_supported TARGET)
    foreach(FLAG ${ARGN})
        add_compile_flag_if_supported("${TARGET}" "${FLAG}")
    endforeach()
endfunction()

test_big_endian(IS_BIGENDIAN)

option(DETHRACE_PLATFORM_SDL1 "Support SDL 1.2 platform driver" OFF)
option(DETHRACE_PLATFORM_SDL2 "Support SDL 2 platform driver" ON)
set(count_sdl_platforms 0)

set(DETHRACE_PLATFORMS)

if(DETHRACE_PLATFORM_SDL1)
    find_package(SDL REQUIRED)
    list(APPEND DETHRACE_PLATFORMS SDL1)
    math(EXPR count_sdl_platforms "${count_sdl_platforms} + 1")
endif()

if(DETHRACE_PLATFORM_SDL2)
    find_package(SDL2 CONFIG)

    if(NOT SDL2_FOUND)
        find_package(SDL2 MODULE REQUIRED)
    endif()

    list(APPEND DETHRACE_PLATFORMS SDL2)
    math(EXPR count_sdl_platforms "${count_sdl_platforms} + 1")
endif()

if(count_sdl_platforms GREATER 1)
    # Force dynamic SDL when enabling 2 (or more) SDL platform backends
    set(dynamic_sdl_force TRUE)
else()
    # When no SDL backend is selected, disable dynamic SDL backend
    # Enabling one SDL backend is a don-t-care
    set(dynamic_sdl_force FALSE)
endif()

cmake_dependent_option(DETHRACE_PLATFORM_SDL_DYNAMIC "Load SDL dynamically" FALSE "count_sdl_platforms EQUAL 1" "${dynamic_sdl_force}")

if(NOT DETHRACE_PLATFORMS)
    message(FATAL_ERROR "No platform drivers enabled")
endif()

add_subdirectory(lib/BRender-v1.3.2)
add_subdirectory(lib/libsmacker)

if(DETHRACE_SOUND_ENABLED)
    add_subdirectory(lib/miniaudio2)
    add_subdirectory(lib/stb2)
endif()

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
add_subdirectory(src/DETHRACE)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(DETHRACE_INSTALL)
    install(FILES LICENSE
        DESTINATION "."
    )

    set(DETHRACE_PACKAGE_PLATFORM "" CACHE STRING "Dethrace binary package platform")
    set(DETHRACE_PACKAGE_ARCH "" CACHE STRING "Dethrace binary package architecture")

    if(NOT DETHRACE_PACKAGE_PLATFORM)
        set(DETHRACE_PACKAGE_PLATFORM "${CMAKE_SYSTEM_NAME}")
    endif()

    string(TOLOWER "${DETHRACE_PACKAGE_PLATFORM}" DETHRACE_PACKAGE_PLATFORM)
    string(TOLOWER "${DETHRACE_PACKAGE_ARCH}" DETHRACE_PACKAGE_ARCH)

    set(CPACK_PACKAGE_NAME "dethrace")
    set(CPACK_SYSTEM_NAME "${DETHRACE_PACKAGE_PLATFORM}")

    set(CPACK_PACKAGE_FILE_NAME "dethrace-${DETHRACE_VERSION}-${DETHRACE_PACKAGE_PLATFORM}")

    if(DETHRACE_PACKAGE_ARCH)
        string(APPEND CPACK_PACKAGE_FILE_NAME "-${DETHRACE_PACKAGE_ARCH}")
    endif()

    set(CPACK_PACKAGE_DIRECTORY dist)

    if(APPLE)
        set(CPACK_DMG_DISABLE_APPLICATIONS_SYMLINK "ON")
        set(CPACK_GENERATOR "DragNDrop")
        set(ext ".dmg")
    elseif(MSVC)
        set(CPACK_GENERATOR ZIP)
        set(ext ".zip")
    else()
        set(CPACK_GENERATOR TGZ)
        set(ext ".tar.gz")
    endif()

    if(EXISTS "$ENV{GITHUB_OUTPUT}")
        file(APPEND "$ENV{GITHUB_OUTPUT}" "filename=${CPACK_PACKAGE_FILE_NAME}${ext}")
    endif()

    include(CPack)
endif()
