add_library(s3 STATIC)

target_include_directories(s3 PUBLIC
    include
)

target_link_libraries(s3 PRIVATE)

if(NOT MSVC)
    target_compile_options(s3 PRIVATE
        -g
        -Wall
        -Wextra
        -Werror
        -Wno-unused-parameter
    )
else()
    target_compile_definitions(s3 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(s3 PRIVATE
        /wd4101
        /wd4996
    )
endif()

target_sources(s3 PRIVATE
    s3sound.c
    include/s3/s3sound.h
)
