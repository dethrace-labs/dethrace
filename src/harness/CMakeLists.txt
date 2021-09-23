add_library(harness STATIC)

target_include_directories(harness PUBLIC .)
target_include_directories(harness PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/src/DETHRACE
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
    cameras/debug_camera.c
    cameras/debug_camera.h
    harness_hooks.h
    harness_trace.c
    harness_trace.h
    harness.c
    harness.h
    input/keyboard.c
    input/keyboard.h
    renderers/gl_renderer.c
    renderers/gl_renderer.h
    renderers/gl_renderer_shaders.c
    renderers/gl_renderer_shaders.h
    rendering/renderer_state.c
    rendering/renderer_state.h
    renderers/null_renderer.c
    renderers/null_renderer.h
    sound/sound.c
    sound/sound.h
    stack_trace_handler.h
)