add_library(glad OBJECT)
target_include_directories(glad PUBLIC include)
target_sources(glad PRIVATE src/glad.c include/glad/glad.h)

if(NOT WIN32)
    target_link_libraries(glad PUBLIC dl)
endif()
