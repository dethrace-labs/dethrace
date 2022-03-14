add_library(harness STATIC)

if (NOT DEFINED RENDERER_PLATFORM)
    set(RENDERER_PLATFORM "SDL_OpenGL")
endif()

target_include_directories(harness 
    PRIVATE
        .
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/DETHRACE
    PUBLIC
        include
)

target_link_libraries(harness PRIVATE miniposix brender SDL2::SDL2 glad s3 cglm_headers)

if(WIN32)
    target_link_libraries(harness PRIVATE dbghelp)
endif()

if(NOT MSVC)
    target_compile_options(harness PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unused-parameter
    )
else()
    target_compile_definitions(harness PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

target_sources(harness PRIVATE
    include/harness/hooks.h
    include/harness/trace.h
    include/harness/config.h

    cameras/debug_camera.c
    cameras/debug_camera.h
    harness_trace.c
    harness.c
    harness.h
    renderers/null.h
    renderers/renderer.h
    brender_emu/renderer_impl.c
    brender_emu/renderer_impl.h
    sound/sound.c
    sound/sound.h
)

if (RENDERER_PLATFORM STREQUAL "SDL_OpenGL")
    target_sources(harness PRIVATE
        platforms/sdl_gl.c
        renderers/gl/gl_renderer.c
        renderers/gl/gl_renderer.h
        renderers/gl/shaders.c
        renderers/gl/shaders.h
        renderers/gl/stored_context.c
        renderers/gl/stored_context.h
    )
endif()

if(WIN32)
    target_sources(harness PRIVATE
        os/windows.c
    )
elseif(APPLE)
target_sources(harness PRIVATE
    os/macosx.c
)
else()
    target_sources(harness PRIVATE
        os/linux.c
    )
endif()
