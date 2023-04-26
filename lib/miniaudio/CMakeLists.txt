include(CheckSymbolExists)
include(CMakePushCheckState)

add_library(miniaudio STATIC
    miniaudio.c
)

target_include_directories(miniaudio 
    PUBLIC
        include
        include/miniaudio
)

cmake_push_check_state(RESET)
check_symbol_exists("exp" "math.h" HAVE_EXP)
if(NOT HAVE_EXP)
    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_symbol_exists("exp" "math.h" HAVE_EXP_WITH_M)
    if(HAVE_EXP_WITH_M)
        target_link_libraries(miniaudio PRIVATE m)
    endif()
endif()
cmake_pop_check_state()
