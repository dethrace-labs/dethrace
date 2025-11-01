configure_file(version.h.in version.h @ONLY)

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

if(MSVC_42_FOR_RECCMP)
  target_link_libraries(harness PRIVATE brender inih compile_with_werror)
else()
  target_link_libraries(harness PRIVATE brender miniaudio stb inih compile_with_werror)
endif()

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
    include/harness/audio.h

    ascii_tables.h
    harness_trace.c
    harness.c
    harness.h

    platforms/null.c
    platforms/null.h

    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

if(DETHRACE_SOUND_ENABLED)
    target_sources(harness PRIVATE audio/miniaudio.c)
    target_link_libraries(harness PRIVATE miniaudio stb)
else()
    target_sources(harness PRIVATE audio/null.c)
endif()

if(MSVC_42_FOR_RECCMP)
    # we don't currently care about running it, just get it compiled!
    target_sources(harness PRIVATE os/null.c)
elseif(WIN32)
    target_sources(harness PRIVATE os/windows.c)
    target_link_libraries(harness PRIVATE dbghelp ws2_32 iphlpapi)
elseif(APPLE)
    target_sources(harness PRIVATE os/macos.c)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(harness PRIVATE os/linux.c)
else()
    message(FATAL_ERROR "Unsupported or unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

if(DETHRACE_PLATFORM_SDL1)
    target_sources(harness PRIVATE
        platforms/sdl1.c
        platforms/sdl1_scancode_to_dinput.h
        platforms/sdl1_syms.h
    )
    target_compile_definitions(harness PRIVATE DETHRACE_PLATFORM_SDL1)

    if(DETHRACE_PLATFORM_SDL_DYNAMIC)
        set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/sdl1.c" APPEND PROPERTY INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:SDL::SDL,INTERFACE_INCLUDE_DIRECTORIES>")
        get_filename_component(sdl_library_directory "${SDL_LIBRARY}" DIRECTORY)
        set_property(GLOBAL APPEND PROPERTY DETHRACE_BUILD_RPATHS "${sdl_library_directory}")
    else()
        target_include_directories(harness PRIVATE ${SDL_INCLUDE_DIR})
        target_link_libraries(harness PRIVATE ${SDL_LIBRARIES})
    endif()
endif()

if(DETHRACE_PLATFORM_SDL2)
    target_sources(harness PRIVATE
        platforms/sdl2.c
        platforms/sdl2_scancode_map.h
        platforms/sdl2_syms.h
    )
    target_compile_definitions(harness PRIVATE DETHRACE_PLATFORM_SDL2)

    if(DETHRACE_PLATFORM_SDL_DYNAMIC)
        set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/sdl2.c" APPEND PROPERTY INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:SDL2::SDL2,INTERFACE_INCLUDE_DIRECTORIES>")
        set_property(GLOBAL APPEND PROPERTY DETHRACE_BUILD_RPATHS "$<TARGET_FILE_DIR:SDL2::SDL2>")
    else()
        target_link_libraries(harness PRIVATE SDL2::SDL2)
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "TinyCC")
        set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/sdl2.c" APPEND PROPERTY COMPILE_DEFINITIONS "SDL_DISABLE_IMMINTRIN_H")
    endif()
endif()

if(DETHRACE_PLATFORM_SDL3)
    target_sources(harness PRIVATE
        platforms/sdl3.c
        platforms/sdl2_scancode_map.h
        platforms/sdl3_syms.h
    )
    target_compile_definitions(harness PRIVATE DETHRACE_PLATFORM_SDL3)
    if(DETHRACE_PLATFORM_SDL_DYNAMIC)
        set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/sdl3.c" APPEND PROPERTY INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:SDL3::SDL3,INTERFACE_INCLUDE_DIRECTORIES>")
        set_property(GLOBAL APPEND PROPERTY DETHRACE_BUILD_RPATHS "$<TARGET_FILE_DIR:SDL3::SDL3>")
    else()
        target_link_libraries(harness PRIVATE SDL3::SDL3)
    endif()
endif()

if(DETHRACE_PLATFORM_SDL_DYNAMIC)
    target_compile_definitions(harness PRIVATE DETHRACE_SDL_DYNAMIC)
    target_link_libraries(harness PRIVATE ${CMAKE_DL_LIBS})
endif()


