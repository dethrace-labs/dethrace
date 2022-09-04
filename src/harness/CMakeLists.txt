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

target_link_libraries(harness PRIVATE brender dethrace_werror)

if(WIN32)
    target_link_libraries(harness PRIVATE dbghelp)
endif()

if(NOT MSVC)
    target_compile_options(harness PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
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
    include/harness/hooks.h
    include/harness/trace.h
    include/harness/config.h
    include/harness/os.h

    # cameras/debug_camera.c
    # cameras/debug_camera.h
    harness_trace.c
    harness.c
    harness.h
    io_platforms/io_platform.h
    renderers/null.h
    renderers/renderer.h
    brender_emu/renderer_impl.c
    brender_emu/renderer_impl.h
    sound/sound.c
    sound/sound.h
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

if (IO_PLATFORM STREQUAL "SDL_OpenGL")
    target_sources(harness PRIVATE
        io_platforms/sdl_gl.c
        renderers/gl/gl_renderer.c
        renderers/gl/gl_renderer.h
        renderers/gl/shaders.c
        renderers/gl/shaders.h
        renderers/gl/stored_context.c
        renderers/gl/stored_context.h
    )
    target_link_libraries(harness PRIVATE SDL2::SDL2 glad)
endif()

if(WIN32)
    target_sources(harness PRIVATE
        os/windows.c
    )
elseif(APPLE)
target_sources(harness PRIVATE
    os/macos.c
)
else()
    target_sources(harness PRIVATE
        os/linux.c
    )
endif()
