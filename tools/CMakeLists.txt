find_package(SDL2 REQUIRED)

add_executable(minismackerplay minismackerplay.c)
target_link_libraries(minismackerplay PRIVATE miniaudio libsmacker SDL2::SDL2 compile_with_werror)
if(MSVC)
    target_compile_options(minismackerplay PRIVATE -W3)
else()
    target_compile_options(minismackerplay PRIVATE -Wall)
endif()
