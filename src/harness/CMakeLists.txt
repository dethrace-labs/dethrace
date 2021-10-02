add_library(harness STATIC)

target_include_directories(harness 
    PRIVATE
        .
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/DETHRACE
    PUBLIC
        include
)

target_link_libraries(harness PRIVATE miniposix brender SDL2::SDL2 glad s3 cglm_headers)

if(NOT MSVC)
    target_compile_options(harness PRIVATE
        -g
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
    sdl/gl_renderer.c
    sdl/gl_renderer.h
    sdl/gl_renderer_shaders.c
    sdl/gl_renderer_shaders.h
    rendering/renderer_state.c
    rendering/renderer_state.h
    renderers/null_renderer.c
    renderers/null_renderer.h
    sound/sound.c
    sound/sound.h
    sdl/common.c
    sdl/common.h
    platforms/sdl_gl.h
    stack_trace_handler.h
)