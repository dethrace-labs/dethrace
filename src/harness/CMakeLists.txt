configure_file(version.h.in version.h @ONLY)

add_library(harness STATIC
    include/harness/compiler.h
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
    renderers/null/null.c
    renderers/null/null.h
    renderers/renderer.h
    brender_emu/renderer_impl.c
    brender_emu/renderer_impl.h
    sound/sound.c
    sound/sound.h
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)
if(DETHRACE_DEBUGUI)
    target_sources(harness PRIVATE
        debugui.cpp
        debugui.h
    )
    target_compile_definitions(harness PUBLIC DETHRACE_DEBUGUI)
    target_link_libraries(harness PRIVATE cimgui imgui)
endif()

target_include_directories(harness
    PRIVATE
        .
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/DETHRACE
        "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderers"
)

if(DETHRACE_FIX_BUGS)
    target_compile_definitions(harness PRIVATE DETHRACE_FIX_BUGS)
endif()

target_link_libraries(harness PRIVATE brender compile_with_werror)

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


if(RENDERER_OPENGL)
    # generate embedded glsl resources
    foreach(elem resources/framebuffer_vert.glsl resources/framebuffer_frag.glsl resources/3d_vert.glsl resources/3d_frag.glsl)
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${elem}.h"
            COMMAND "${CMAKE_COMMAND}" "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}" "-DFILE=${elem}" -P "${dethrace_SOURCE_DIR}/cmake/EmbedResource.cmake"
            DEPENDS "${elem}"
        )
    endforeach()

    target_sources(harness PRIVATE
        renderers/gl/gl_renderer.c
        renderers/gl/gl_renderer.h
        renderers/gl/stored_context.c
        renderers/gl/stored_context.h

        resources/framebuffer_vert.glsl.h
        resources/framebuffer_frag.glsl.h
        resources/3d_vert.glsl.h
        resources/3d_frag.glsl.h
    )
    target_compile_definitions(harness PUBLIC RENDERER_OPENGL3)
    target_include_directories(harness PRIVATE "${dethrace_SOURCE_DIR}/src/DETHRACE/common")
    target_include_directories(harness PRIVATE "${dethrace_SOURCE_DIR}/src/S3/include")
    if(DETHRACE_DEBUGUI)
        target_link_libraries(harness PRIVATE imgui_opengl3)
    endif()
endif()

if(IOPLATFORM_SDL)
    target_sources(harness PRIVATE
        io_platforms/sdl_gl.c
        include/harness/private/platform_glfw.h
    )
    target_compile_definitions(harness PUBLIC IOPLATFORM_SDL2)
    target_link_libraries(harness PRIVATE SDL2::SDL2 glad)
    if(DETHRACE_DEBUGUI)
        target_link_libraries(harness PRIVATE imgui_sdl2)
    endif()
elseif(IOPLATFORM_GLFW)
    target_sources(harness PRIVATE
        io_platforms/glfw_gl.c
        include/harness/private/platform_glfw.h
    )
    target_compile_definitions(harness PUBLIC IOPLATFORM_GLFW)
    target_link_libraries(harness PRIVATE glad)
    if(DETHRACE_DEBUGUI)
        target_link_libraries(harness PRIVATE imgui_glfw)
    endif()
else()
    message(FATAL_ERROR "Unknown IO_PLATFORM")
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
