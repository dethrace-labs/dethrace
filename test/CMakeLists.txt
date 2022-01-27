add_executable(dethrace_test)
add_test(NAME test_dethrace COMMAND dethrace_test)

target_link_libraries(dethrace_test PRIVATE glad dethrace_obj)

target_include_directories(dethrace_test PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src/harness
    ${CMAKE_SOURCE_DIR}/src/BRSRC13
    ${CMAKE_SOURCE_DIR}/src/DETHRACE
    ${CMAKE_SOURCE_DIR}/src/DETHRACE/common
    ${CMAKE_SOURCE_DIR}/src/DETHRACE/pd
)

if(NOT MSVC)
    if(APPLE)
        target_link_libraries(dethrace_test PRIVATE "-framework OpenGL")
    else()
        set(OpenGL_GL_PREFERENCE GLVND)
        find_package(OpenGL REQUIRED)
        if(TARGET OpenGL::OpenGL)
            target_link_libraries(dethrace_test PRIVATE OpenGL::OpenGL)
        else()
            target_link_libraries(dethrace_test PRIVATE OpenGL::GL)
        endif()
    endif()
else()
    target_compile_definitions(dethrace_test PRIVATE -D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED)
    target_link_libraries(dethrace_test PRIVATE dbghelp)
endif()

target_sources(dethrace_test PRIVATE
    BRSRC13/test_assocarr.c
    BRSRC13/test_actsupt.c
    BRSRC13/test_brlists.c
    BRSRC13/test_datafile.c
    BRSRC13/test_fixed.c
    BRSRC13/test_fwsetup.c
    BRSRC13/test_genclip.c
    BRSRC13/test_pattern.c
    BRSRC13/test_pmfile.c
    BRSRC13/test_quat.c
    BRSRC13/test_lexer.c
    BRSRC13/test_logwrite.c
    BRSRC13/test_matrix23.c
    BRSRC13/test_matrix34.c
    BRSRC13/test_matrix4
    BRSRC13/test_register.c
    BRSRC13/test_regsupt.c
    BRSRC13/test_resource.c
    BRSRC13/test_resreg.c
    BRSRC13/test_scratch.c
    BRSRC13/test_token.c
    BRSRC13/test_vector.c
    BRSRC13/test_v1dbfile.c
    DETHRACE/test_controls.c
    DETHRACE/test_dossys.c
    DETHRACE/test_errors.c
    DETHRACE/test_flicplay.c
    DETHRACE/test_graphics.c
    DETHRACE/test_init.c
    DETHRACE/test_input.c
    DETHRACE/test_loading.c
    DETHRACE/test_powerup.c
    DETHRACE/test_utility.c
    framework/unity.c
    framework/unity.h
    framework/unity_internals.h
    framework
    main.c
    tests.h
)
