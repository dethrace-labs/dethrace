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


add_subdirectory(lib/BRender-v1.3.2)
add_subdirectory(lib/libsmacker)
add_subdirectory(lib/miniaudio)
add_subdirectory(lib/stb)

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

if (NINTENDO_3DS)
    set(APP_TITLE "${PROJECT_NAME}")
    set(APP_DESCRIPTION "${PROJECT_NAME} port for 3DS")
    set(APP_AUTHOR "dethrace-labs")
    set(APP_UNIQUE_ID "0xF02F6")
    set(APP_ICON "${PROJECT_SOURCE_DIR}/packaging/ctr/icon.png")
    set(APP_BANNER "${PROJECT_SOURCE_DIR}/packaging/ctr/banner.png")
    set(APP_AUDIO "${PROJECT_SOURCE_DIR}/packaging/ctr/audio_silent.wav")
    set(APP_ROMFS_DIR "${PROJECT_SOURCE_DIR}/packaging/ctr/romfs")
    set(APP_RSF "${PROJECT_SOURCE_DIR}/packaging/ctr/template.rsf")

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.3dsx
        COMMAND 3dsxtool ${PROJECT_NAME}.elf ${PROJECT_NAME}${ROM}.3dsx
                         --romfs=${APP_ROMFS_DIR}
                         --smdh=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.smdh
        DEPENDS ${PROJECT_NAME} ${PROJECT_NAME}.smdh
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.cia
        COMMAND makerom	-f cia
                        -target t
                        -exefslogo
                        -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.cia
                        -elf ${PROJECT_NAME}.elf
                        -rsf ${APP_RSF}
                            -DAPP_TITLE=${APP_TITLE}
                            -DAPP_UNIQUE_ID=${APP_UNIQUE_ID}
                            -DAPP_ROMFS_DIR=${APP_ROMFS_DIR}
                        -banner ${PROJECT_NAME}.bnr
                        -icon ${PROJECT_NAME}.smdh
        DEPENDS ${PROJECT_NAME} ${APP_RSF} ${PROJECT_NAME}.smdh ${PROJECT_NAME}.bnr
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.smdh
        COMMAND bannertool makesmdh	-o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.smdh
                                    -s ${APP_TITLE}
                                    -l ${APP_DESCRIPTION}
                                    -p ${APP_AUTHOR}
                                    -i ${APP_ICON}

        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bnr
        COMMAND bannertool makebanner	-o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bnr
                                        -i ${APP_BANNER}
                                        -a ${APP_AUDIO}
    )

    add_custom_target( N3DS ALL
        DEPENDS ${PROJECT_NAME}.3dsx
                ${PROJECT_NAME}.cia
    )
endif ()
