configure_file(version.h.in version.h @ONLY)

add_library(harness STATIC)

if (NOT DEFINED IO_PLATFORM)
    set(IO_PLATFORM "SDL_OpenGL")
endif()

target_include_directories(harness
    PRIVATE
        .
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/DETHRACE
        "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC
        include
)

if(DETHRACE_FIX_BUGS)
    target_compile_definitions(harness PRIVATE DETHRACE_FIX_BUGS)
endif()

target_link_libraries(harness PRIVATE brender s3 compile_with_werror)

if(WIN32)
    target_link_libraries(harness PRIVATE dbghelp)
endif()

if(NOT MSVC)
    target_compile_options(harness PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
    )
    add_compile_flags_if_supported(harness
        -Wstrict-prototypes
    )
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
    # cameras/debug_camera.c
    # cameras/debug_camera.h
    ascii_tables.h
    harness_trace.c
    harness.c
    harness.h
    sound/sound.c
    sound/sound.h
    win95/polyfill.c
    win95/winsock.c
    platforms/null.c
    platforms/null.h

    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

if (IO_PLATFORM STREQUAL "SDL_OpenGL")
    # generate embedded glsl resources

    target_sources(harness PRIVATE
        platforms/sdl_opengl.c
        platforms/sdl2_scancode_to_dinput.h

    )
    target_include_directories(harness PRIVATE "${dethrace_SOURCE_DIR}/src/DETHRACE/common")
    target_link_libraries(harness PRIVATE SDL2::SDL2 glad)
endif()

if(WIN32)
    target_sources(harness PRIVATE
        os/windows.c
    )
    target_link_libraries(harness PRIVATE ws2_32)
elseif(APPLE)
    target_sources(harness PRIVATE
        os/macos.c
    )
else()
    target_sources(harness PRIVATE
        os/linux.c
    )
endif()
