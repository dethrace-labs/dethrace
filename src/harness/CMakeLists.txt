configure_file(version.h.in version.h @ONLY)

option(DETHRACE_PLATFORM_SDL2 "Support SDL2 platform driver" ON)
set(count_sdl_platforms 0)

set(DETHRACE_PLATFORMS )
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

add_library(harness STATIC)

target_include_directories(harness
    PRIVATE
        .
        ${CMAKE_SOURCE_DIR}
        "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC
        include
)

if(DETHRACE_FIX_BUGS)
    target_compile_definitions(harness PRIVATE DETHRACE_FIX_BUGS)
endif()

target_link_libraries(harness PRIVATE brender miniaudio stb compile_with_werror)

if(NOT MSVC)
    target_compile_options(harness PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
    )
    add_compile_flag_if_supported(harness -Wstrict-prototypes LANGUAGES C)
else()
    target_compile_definitions(harness PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

if(IS_BIGENDIAN)
    target_compile_definitions(harness PRIVATE BR_ENDIAN_BIG=1)
else()
    target_compile_definitions(harness PRIVATE BR_ENDIAN_LITTLE=1)
endif()

target_sources(harness PRIVATE
    include/harness/compiler.h
    include/harness/hooks.h
    include/harness/trace.h
    include/harness/config.h
    include/harness/os.h
    include/harness/win95_polyfill.h
    include/harness/win95_polyfill_defs.h
    include/harness/audio.h
    # cameras/debug_camera.c
    # cameras/debug_camera.h
    ascii_tables.h
    harness_trace.c
    harness.c
    harness.h
    audio/miniaudio.c
    win95/polyfill.c
    win95/winsock.c
    platforms/null.c
    platforms/null.h

    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

if(DETHRACE_PLATFORM_SDL2)
    target_sources(harness PRIVATE
        platforms/sdl2.c
        platforms/sdl2_scancode_to_dinput.h
    )
    target_compile_definitions(harness PRIVATE DETHRACE_PLATFORM_SDL2)
    if(DETHRACE_PLATFORM_SDL_DYNAMIC)
        set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/sdl2.c" APPEND PROPERTY INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:SDL2::SDL2,INTERFACE_INCLUDE_DIRECTORIES>")
        set_property(GLOBAL APPEND PROPERTY DETHRACE_BUILD_RPATHS "$<TARGET_FILE_DIR:SDL2::SDL2>")
    else()
        target_link_libraries(harness PRIVATE SDL2::SDL2)
    endif()
endif()

if(DETHRACE_PLATFORM_SDL_DYNAMIC)
    target_compile_definitions(harness PRIVATE DETHRACE_SDL_DYNAMIC)
    target_link_libraries(harness PRIVATE ${CMAKE_DL_LIBS})
endif()

if(WIN32)
    target_sources(harness PRIVATE
        os/windows.c
    )
    target_link_libraries(harness PRIVATE dbghelp ws2_32)
elseif(APPLE)
    target_sources(harness PRIVATE
        os/macos.c
    )
else()
    target_sources(harness PRIVATE
        os/linux.c
    )
endif()
