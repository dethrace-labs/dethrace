add_library(harness OBJECT)

target_include_directories(harness PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/src/BRSRC13
    ${CMAKE_SOURCE_DIR}/src/DETHRACE
    ${CMAKE_SOURCE_DIR}/src/S3/include
)

target_link_libraries(harness PRIVATE miniposix SDL2::SDL2 glad)

if(NOT MSVC)
    target_compile_options(harness PRIVATE
        -g
        -Wno-return-type
        -Wno-missing-declarations
        -Werror=implicit-function-declaration
    )
else()
    target_compile_definitions(harness PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

target_sources(harness PRIVATE
    debug.c
    debug.h
    harness.c
    harness.h
    input/keyboard.c
    input/keyboard.h
    renderers/gl_renderer.c
    renderers/gl_renderer.h
    renderers/null_renderer.c
    renderers/null_renderer.h
    sound/sound.c
    sound/sound.h
    stack_trace_handler.h
)