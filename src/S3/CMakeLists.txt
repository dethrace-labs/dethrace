add_library(s3 STATIC)

target_include_directories(s3 
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        ${CMAKE_SOURCE_DIR}      
)

target_link_libraries(s3 PRIVATE brender SDL2::SDL2 harness miniaudio compile_with_werror)

if(NOT MSVC)
    target_link_libraries(s3 PUBLIC pthread m)
    add_compile_flags_if_supported(s3
        -Wstrict-prototypes
    )
else()
    target_compile_definitions(s3 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(s3 PRIVATE
        /wd4101
        /wd4996
    )
endif()
if(DETHRACE_FIX_BUGS)
    target_compile_definitions(s3 PRIVATE DETHRACE_FIX_BUGS)
endif()

if(IS_BIGENDIAN)
    target_compile_definitions(s3 PRIVATE BR_ENDIAN_BIG=1)
else()
    target_compile_definitions(s3 PRIVATE BR_ENDIAN_LITTLE=1)
endif()

target_sources(s3 PRIVATE
    3d.c
    3d.h
    audio.c
    audio.h
    ioserv.c
    ioserv.h
    profile.c
    profile.h
    resource.c
    resource.h
    s3cda.c
    s3cda.h
    s3music.c
    s3music.h
    s3sound.c
    s3sound.h
)
