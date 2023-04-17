# Create object files so we can link them into the main binary and into tests without building twice.
add_library(dethrace_obj OBJECT)

option(DETHRACE_REPLAY_DEBUG "Debug replay" OFF)

target_include_directories(dethrace_obj
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        common
        pd
)

target_link_libraries(dethrace_obj PUBLIC SDL2::SDL2 smackw32 harness brender s3)



if (CMAKE_C_COMPILER_ID MATCHES "MSVC")
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
        -Wno-error=sign-compare
        -Wno-error=format-overflow
        -Wno-error=unused-but-set-variable  # (SelectRaceDraw::test2), etc
        -Wno-format-overflow
    )
endif()
if(DETHRACE_FIX_BUGS)
    target_compile_definitions(dethrace_obj PRIVATE DETHRACE_FIX_BUGS)
endif()

if(IS_BIGENDIAN)
    target_compile_definitions(dethrace_obj PRIVATE BR_ENDIAN_BIG=1)
else()
    target_compile_definitions(dethrace_obj PRIVATE BR_ENDIAN_LITTLE=1)
endif()

if(DETHRACE_REPLAY_DEBUG)
    target_compile_definitions(dethrace_obj PRIVATE DETHRACE_REPLAY_DEBUG)
endif()

target_sources(dethrace_obj PRIVATE
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
    common/loading.c
    common/loading.h
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
    common/oil.c
    common/oil.h
    common/oppocar.c
    common/oppocar.h
    common/opponent.c
    common/opponent.h
    common/oppoproc.c
    common/oppoproc.h
    common/options.c
    common/options.h
    common/pedestrn.c
    common/pedestrn.h
    common/piping.c
    common/piping.h
    common/powerup.c
    common/powerup.h
    common/pratcam.c
    common/pratcam.h
    common/racestrt.c
    common/racestrt.h
    common/racesumm.c
    common/racesumm.h
    common/raycast.c
    common/raycast.h
    common/replay.c
    common/replay.h
    common/skidmark.c
    common/skidmark.h
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
    pc-dos/dosnet.c
    pd/net.h
    pc-dos/dossys.c
    pd/sys.h
)

# Create our main game binary.
add_executable(dethrace)
target_link_libraries(dethrace PRIVATE glad dethrace_obj compile_with_werror)
target_sources(dethrace PRIVATE main.c)

if(NOT MSVC)
    if(APPLE)
        target_link_libraries(dethrace PRIVATE "-framework OpenGL")
    else()
        set(OpenGL_GL_PREFERENCE GLVND)
        find_package(OpenGL REQUIRED)
        if(TARGET OpenGL::OpenGL)
            target_link_libraries(dethrace PRIVATE OpenGL::OpenGL)
        else()
            target_link_libraries(dethrace PRIVATE OpenGL::GL)
        endif()
    endif()
else()
    target_link_libraries(dethrace PRIVATE dbghelp)
    target_link_options(dethrace PRIVATE /subsystem:windows /ENTRY:mainCRTStartup)
    target_compile_definitions(dethrace PRIVATE -D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED)
endif()

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

if (DETHRACE_INSTALL)
    install(TARGETS dethrace
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
    if(MSVC)
        INSTALL(FILES $<TARGET_PDB_FILE:dethrace>
            DESTINATION "${CMAKE_INSTALL_BINDIR}"
            OPTIONAL
        )
    endif()
endif()
