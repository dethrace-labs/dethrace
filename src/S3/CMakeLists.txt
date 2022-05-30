add_library(s3 STATIC)

target_include_directories(s3 
    PUBLIC
        include
    PRIVATE
        ${CMAKE_SOURCE_DIR}      
)

target_link_libraries(s3 PRIVATE brender SDL2::SDL2 harness miniaudio)

if(NOT MSVC)
    target_compile_options(s3 PRIVATE
        -g
        -Wall
        -Werror
        -Wno-unused-parameter
    )
    target_link_libraries(s3 PUBLIC pthread m)
else()
    target_compile_definitions(s3 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(s3 PRIVATE
        /wd4101
        /wd4996
    )
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
