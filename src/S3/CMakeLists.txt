add_library(s3 STATIC)

target_include_directories(s3 PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/BRSRC13
    ${CMAKE_SOURCE_DIR}/src/harness
)

target_link_libraries(s3 PRIVATE SDL2::SDL2 glad)

if(NOT MSVC)
    target_compile_options(s3 PRIVATE
        -g
        -Wno-return-type
        -Wno-missing-declarations
        -Werror=implicit-function-declaration
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
