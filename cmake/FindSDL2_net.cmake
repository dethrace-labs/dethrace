find_package(FindPkgConfig QUIET)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(SDL2_net QUIET SDL2_net)
endif()

find_path(SDL2_net_INCLUDE_DIRS SDL_net.h
    PATH_SUFFIXES SDL2
)

find_path(SDL2_net_LIBRARIES
    NAMES SDL2_net
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_net
    REQUIRED_VARS SDL2_net_LIBRARIES SDL2_net_INCLUDE_DIRS
)

if(SDL2_net_FOUND)
    if(NOT TARGET SDL2_net::SDL2_net)
        add_library(SDL2_net::SDL2_net INTERFACE IMPORTED)
        set_target_properties(SDL2_net::SDL2_net
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${SDL2_net_INCLUDE_DIRS}"
                INTERFACE_LINK_LIBRARIES "${SDL2_net_LIBRARIES}"
        )
    endif()
endif()
