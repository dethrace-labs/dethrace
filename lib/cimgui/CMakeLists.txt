add_library(cimgui STATIC
    cimgui.h
    cimgui.cpp
)
target_include_directories(cimgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(cimgui PRIVATE imgui)
