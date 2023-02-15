find_package(SDL2 REQUIRED)

add_executable(minismackerplay minismackerplay.c)
target_link_libraries(minismackerplay PRIVATE miniaudio smacker SDL2::SDL2)
