configure_file(version.h.in version.h @ONLY)

add_library(harness STATIC)

if (NOT DEFINED IO_PLATFORM)
    set(IO_PLATFORM "SDL2")
endif()

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

if (IO_PLATFORM STREQUAL "SDL2")
    target_sources(harness PRIVATE
        platforms/sdl2.c
        platforms/sdl2_scancode_to_dinput.h
    )
    target_link_libraries(harness PRIVATE SDL2::SDL2)
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
