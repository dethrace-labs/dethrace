add_library(imgui STATIC
    imgui.cpp
    imgui.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp

    # FIXME: only when using SDL
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_sdl.cpp
    backends/imgui_impl_sdl.h
)
target_include_directories(imgui
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_link_libraries(imgui
    PRIVATE
        SDL2::SDL2
)
target_compile_features(imgui PUBLIC cxx_std_11)
