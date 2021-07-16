# Create object files so we can link them into the main binary and into tests without building twice.
add_library(dethrace_obj OBJECT)

target_include_directories(dethrace_obj PUBLIC
    .
    common
    pd
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src/harness
    ${CMAKE_SOURCE_DIR}/src/BRSRC13
    ${CMAKE_SOURCE_DIR}/src/S3/include
)

target_link_libraries(dethrace_obj PUBLIC miniposix SDL2::SDL2 smacker harness brender s3 cglm)

if(NOT MSVC)
    target_compile_options(dethrace_obj PRIVATE
        -g
        -Wall
        -Wextra
        -Werror
        -Wno-return-type
        -Wno-unused-variable
        -Wno-unused-parameter
        -Wno-sign-compare
        -Wno-unused-result
    )
else()
    target_compile_definitions(dethrace_obj PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(dethrace_obj PRIVATE
        /wd4101
        /wd4996
    )
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
    watcom_functions.c
    watcom_functions.h
)

# Create our main game binary.
add_executable(dethrace)
target_link_libraries(dethrace PRIVATE glad dethrace_obj)
target_sources(dethrace PRIVATE main.c)

if(NOT MSVC)
    if(APPLE)
        target_link_libraries(dethrace PRIVATE "-framework OpenGL")
        target_link_options(dethrace PRIVATE -fno-pie)
    else()
        target_link_libraries(dethrace PRIVATE GL)
    endif()
else()
    target_link_libraries(dethrace PRIVATE dbghelp)
    target_link_options(dethrace PRIVATE /subsystem:windows /ENTRY:mainCRTStartup)
    target_compile_definitions(dethrace PRIVATE -D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED)
endif()
