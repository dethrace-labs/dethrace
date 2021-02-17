add_executable(dethrace)

target_include_directories(dethrace PRIVATE
    .
    common
    pc-dos
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src/harness
    ${CMAKE_SOURCE_DIR}/src/BRSRC13
)

target_link_libraries(dethrace PRIVATE smacker harness brender miniposix SDL2::SDL2 glad)

if(NOT MSVC)
    target_compile_options(dethrace PRIVATE
        -g
        -Wno-return-type
        -Wno-missing-declarations
        -Werror=implicit-function-declaration
    )

    if(APPLE)
        target_link_libraries(dethrace PRIVATE "-framework OpenGL")
    else()
        target_link_libraries(dethrace PRIVATE GL)
    endif()
else()
    target_compile_definitions(dethrace PRIVATE -D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED)
    target_compile_options(dethrace PRIVATE
        /wd4101
        /wd4996
    )
    target_link_libraries(dethrace PRIVATE dbghelp)
    target_link_options(dethrace PRIVATE /subsystem:windows /ENTRY:mainCRTStartup)
endif()

target_sources(dethrace PRIVATE
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
    main.c
    pc-dos/dosnet.c
    pc-dos/dosnet.h
    pc-dos/dossys.c
    pc-dos/dossys.h
    watcom_functions.c
    watcom_functions.h
)