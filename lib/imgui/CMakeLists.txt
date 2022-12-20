add_library(imgui STATIC
    imgui.cpp
    imgui.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
)
target_include_directories(imgui
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_compile_features(imgui PUBLIC cxx_std_11)

add_library(imgui_opengl3 STATIC
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h
)
target_link_libraries(imgui_opengl3 PRIVATE imgui)

if(TARGET SDL2::SDL2)
    add_library(imgui_sdl2 STATIC
        backends/imgui_impl_sdl.cpp
        backends/imgui_impl_sdl.h
    )
    target_link_libraries(imgui_sdl2 PRIVATE imgui)
    target_link_libraries(imgui_sdl2
        PRIVATE
            SDL2::SDL2
    )
endif()

if(TARGET glfw)
    add_library(imgui_glfw STATIC
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_glfw.h
    )
    target_link_libraries(imgui_glfw PRIVATE imgui)
    target_link_libraries(imgui_glfw
        PRIVATE
            glfw
    )
endif()
