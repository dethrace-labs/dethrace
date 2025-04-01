add_executable(dethrace_test)
add_test(NAME test_dethrace COMMAND dethrace_test)

target_link_libraries(dethrace_test PRIVATE dethrace_obj)

target_include_directories(dethrace_test PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src/harness
    ${CMAKE_SOURCE_DIR}/lib/BRender-v1.3.2
    ${CMAKE_SOURCE_DIR}/src/DETHRACE
    ${CMAKE_SOURCE_DIR}/src/DETHRACE/common
    ${CMAKE_SOURCE_DIR}/src/DETHRACE/pd
)

if(NOT MSVC)
else()
    target_compile_definitions(dethrace_test PRIVATE -D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED -DWIN32_LEAN_AND_MEAN)
    target_link_libraries(dethrace_test PRIVATE dbghelp)
endif()

target_sources(dethrace_test PRIVATE
    DETHRACE/test_controls.c
    DETHRACE/test_dossys.c
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
