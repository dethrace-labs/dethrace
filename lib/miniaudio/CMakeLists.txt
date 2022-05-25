add_library(miniaudio STATIC)

target_include_directories(miniaudio 
    PUBLIC
        include
    PRIVATE
        include/miniaudio
        
)

# target_link_libraries(s3 PRIVATE brender SDL2::SDL2 ${SDL2_MIXER_LIBRARIES} harness)

target_sources(miniaudio PRIVATE
    miniaudio.c
    
)
