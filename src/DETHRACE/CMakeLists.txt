# Create object files so we can link them into the main binary and into tests without building twice.
add_library(dethrace_obj OBJECT)

option(DETHRACE_REPLAY_DEBUG "Debug replay" OFF)

target_include_directories(dethrace_obj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    common
    pd
)

if(DETHRACE_ASAN)
    target_compile_options(dethrace_obj PUBLIC -fsanitize=address)
    target_link_options(dethrace_obj PUBLIC -fsanitize=address)
endif()

target_link_libraries(dethrace_obj PUBLIC smackw32 harness BRender::Full BRender::DDI s3)

if(MSVC_42_FOR_RECCMP)
# do nothing
elseif(MSVC)
    target_compile_definitions(dethrace_obj PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(dethrace_obj PRIVATE
        /wd4101
        /wd4996
    )
else()
    target_compile_options(dethrace_obj PRIVATE
        -Wall
    )
    add_compile_flags_if_supported(dethrace_obj
        -Wformat
        -Wno-unused-but-set-variable
        -Wno-unused-variable
        -Wno-unused-result
        -Wno-absolute-value
        -Wstrict-prototypes
    )
endif()

target_compile_definitions(dethrace_obj PRIVATE INSIDE_DETHRACE)

if(DETHRACE_FIX_BUGS)
    target_compile_definitions(dethrace_obj PRIVATE DETHRACE_FIX_BUGS)
endif()

if(DETHRACE_3DFX_PATCH)
    target_compile_definitions(dethrace_obj PRIVATE DETHRACE_3DFX_PATCH)
endif()

if(IS_BIGENDIAN)
    target_compile_definitions(dethrace_obj PRIVATE BR_ENDIAN_BIG=1)
else()
    target_compile_definitions(dethrace_obj PRIVATE BR_ENDIAN_LITTLE=1)
endif()

if(DETHRACE_REPLAY_DEBUG)
    target_compile_definitions(dethrace_obj PRIVATE DETHRACE_REPLAY_DEBUG)
endif()

if(MINGW)
    target_compile_options(dethrace_obj PRIVATE -Wno-error=incompatible-pointer-types)
endif()

target_sources(dethrace_obj PRIVATE
    common/skidmark.c
    common/opponent.c
    common/oil.c
    common/racesumm.c
    common/replay.c
    common/loading.c
    common/piping.c

    common/brucetrk.c
    common/brucetrk.h
    common/car.c
    common/car.h
    common/controls.c
    common/controls.h
    common/crush.c
    common/crush.h
    common/cutscene.c
    common/cutscene.h
    common/demo.c
    common/demo.h
    common/depth.c
    common/depth.h
    common/displays.c
    common/displays.h
    common/drfile.c
    common/drfile.h
    common/drmem.c
    common/drmem.h
    common/errors.c
    common/errors.h
    common/finteray.c
    common/finteray.h
    common/flicplay.c
    common/flicplay.h
    common/globvars.c
    common/globvars.h
    common/globvrbm.c
    common/globvrbm.h
    common/globvrkm.c
    common/globvrkm.h
    common/globvrme.c
    common/globvrme.h
    common/globvrpb.c
    common/globvrpb.h
    common/grafdata.c
    common/grafdata.h
    common/graphics.c
    common/graphics.h
    common/init.c
    common/init.h
    common/input.c
    common/input.h
    common/intrface.c
    common/intrface.h
    common/loadsave.c
    common/loadsave.h
    common/main.c
    common/main.h
    common/mainloop.c
    common/mainloop.h
    common/mainmenu.c
    common/mainmenu.h
    common/netgame.c
    common/netgame.h
    common/network.c
    common/network.h
    common/newgame.c
    common/newgame.h
    common/oppocar.c
    common/oppocar.h
    common/oppoproc.c
    common/oppoproc.h
    common/options.c
    common/options.h
    common/pedestrn.c
    common/pedestrn.h
    common/powerup.c
    common/powerup.h
    common/pratcam.c
    common/pratcam.h
    common/racestrt.c
    common/racestrt.h
    common/raycast.c
    common/raycast.h

    common/sound.c
    common/sound.h
    common/spark.c
    common/spark.h
    common/structur.c
    common/structur.h
    common/trig.c
    common/trig.h
    common/utility.c
    common/utility.h
    common/world.c
    common/world.h
    constants.h
    dr_types.h
    pd/net.h
    pd/sys.h

    pc-all/allsys.c

    # original win95 sys and network
    # pc-win95/win95sys.c
    # pc-win95/ssdx.c
    # pc-win95/ssdx.h

    # original dos sys and network
    # pc-dos/dosnet.c
    # pc-dos/dossys.c
)

if(DETHRACE_NET_ENABLED)
    # dethrace-added cross platform sys and network implementation
    target_sources(dethrace_obj PRIVATE pc-all/allnet.c)
else()
    target_sources(dethrace_obj PRIVATE pc-none/nonet.c)
endif()

# Create our main game binary.
if(MSVC_42_FOR_RECCMP)
    add_executable(dethrace WIN32)
    set(CMAKE_C_FLAGS_DEBUG "/Oi /Zi /MLd /G5")

    add_compile_definitions(_DEBUG)
    set_target_properties(dethrace PROPERTIES OUTPUT_NAME "CARM95")
    target_link_options(dethrace PRIVATE /INCREMENTAL:NO /subsystem:windows /ENTRY:mainCRTStartup)

    set_source_files_properties(common/structur.c PROPERTIES COMPILE_FLAGS "/FAs")
    set_source_files_properties(common/racesumm.c PROPERTIES COMPILE_FLAGS "/FAs")
    set_source_files_properties(common/replay.c PROPERTIES COMPILE_FLAGS "/FAs")
    set_source_files_properties(common/loading.c PROPERTIES COMPILE_FLAGS "/FAs")
    set_source_files_properties(common/piping.c PROPERTIES COMPILE_FLAGS "/FAs")

    reccmp_add_target(dethrace ID CARM95)
    reccmp_configure()
else()
    add_executable(dethrace
        WIN32
        ${CMAKE_SOURCE_DIR}/packaging/macos/dethrace.icns
        ${CMAKE_SOURCE_DIR}/packaging/windows/dethrace.rc
    )
    get_property(build_rpaths GLOBAL PROPERTY DETHRACE_BUILD_RPATHS)
    set_property(TARGET dethrace APPEND PROPERTY BUILD_RPATH "${build_rpaths}")

    if(MSVC)
        target_link_libraries(dethrace PRIVATE dbghelp)
        target_link_options(dethrace PRIVATE /subsystem:windows /ENTRY:mainCRTStartup)
        target_compile_definitions(dethrace PRIVATE -D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED -DWIN32_LEAN_AND_MEAN)
    endif()
endif()

target_link_libraries(dethrace PRIVATE dethrace_obj compile_with_werror)
target_sources(dethrace PRIVATE main.c)

if(DETHRACE_IDE_ROOT_DIR)
    set_target_properties(dethrace PROPERTIES
        VS_DEBUGGER_ENVIRONMENT "DETHRACE_ROOT_DIR=${DETHRACE_IDE_ROOT_DIR}"
    )
endif()

if(DETHRACE_IDE_ARGUMENTS)
    message(STATUS "before: ${DETHRACE_IDE_ARGUMENTS}")
    separate_arguments(DETHRACE_IDE_ARGUMENTS NATIVE_COMMAND "${DETHRACE_IDE_ARGUMENTS}")
    message(STATUS "after: ${DETHRACE_IDE_ARGUMENTS}")
    set_target_properties(dethrace PROPERTIES
        VS_DEBUGGER_COMMAND_ARGUMENTS "${DETHRACE_IDE_ARGUMENTS}"
    )
endif()

if(DETHRACE_INSTALL)
    install(TARGETS dethrace
        BUNDLE DESTINATION . COMPONENT Runtime
        RUNTIME DESTINATION . COMPONENT Runtime
    )

    if(APPLE)
        set_target_properties(dethrace PROPERTIES
            BUNDLE True
            MACOSX_BUNDLE True
            MACOSX_BUNDLE_DISPLAY_NAME "Deth Race"
            OUTPUT_NAME "Deth Race"
            MACOSX_BUNDLE_BUNDLE_VERSION ${DETHRACE_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${DETHRACE_VERSION}
            MACOSX_BUNDLE_ICON_FILE dethrace.icns
        )
        set_source_files_properties(${CMAKE_SOURCE_DIR}/packaging/macos/dethrace.icns PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
        )
        target_sources(dethrace_obj PRIVATE
            ${CMAKE_SOURCE_DIR}/packaging/macos/dethrace.icns
        )

        install(CODE "
            include(BundleUtilities)
            fixup_bundle(\"${CMAKE_BINARY_DIR}/Deth Race.app\" \"\" \"/Library/Frameworks\")
            execute_process(COMMAND
                codesign -s - -f \"${CMAKE_BINARY_DIR}/Deth Race.app/Contents/Frameworks/SDL2.framework\"
            )
            "
            COMPONENT RUNTIME
        )
    endif()

    if(MSVC)
        install(FILES $<TARGET_PDB_FILE:dethrace>
            DESTINATION "."
            OPTIONAL
        )

        if(DETHRACE_PLATFORM_SDL_DYNAMIC)
            if(DETHRACE_PLATFORM_SDL1)
                get_filename_component(sdl1_lib_dir "${SDL_LIBRARY}" DIRECTORY)
                get_filename_component(sdl1_root_dir "${SDL_LIBRARY}" PATH)
                set(sdl1_bin_dir "${sdl1_root_dir}/bin" PATH)
                find_file(SDL1DLL_PATH NAMES "SDL.dll" PATHS ${sdl1_lib_dir} ${sdl1_root_dir} PATH_SUFFIXES "bin" REQUIRED)

                if(SDL1DLL_PATH)
                    install(FILES "${SDL1DLL_PATH}"
                        DESTINATION "."
                        OPTIONAL
                    )
                endif()
            endif()

            if(DETHRACE_PLATFORM_SDL2)
                install(FILES "$<TARGET_FILE:SDL2::SDL2>"
                    DESTINATION "."
                    OPTIONAL
                )
            endif()

            if(DETHRACE_PLATFORM_SDL3)
                install(FILES "$<TARGET_FILE:SDL3::SDL3>"
                    DESTINATION "."
                    OPTIONAL
                )
            endif()
        endif()
    endif()
endif()
