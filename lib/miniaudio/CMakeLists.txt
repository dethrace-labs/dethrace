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
