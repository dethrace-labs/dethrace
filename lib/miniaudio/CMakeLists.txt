add_library(miniaudio STATIC)

target_include_directories(miniaudio 
    PUBLIC
        include
    PRIVATE
        include/miniaudio
        
)

target_sources(miniaudio PRIVATE
    miniaudio.c
    
)

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(miniaudio PRIVATE
        -Wno-deprecated-declarations
    )
endif()
