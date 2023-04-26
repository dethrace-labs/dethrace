# Porting Dethrace to other systems

## Operating Systems

Assuming an operating system called _foo_, follow the steps to add support for it.

1. Add a new file `os/foo.h` and implement the required functions defined in [os.h](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/include/harness/os.h):
- `OS_InstallSignalHandler`
- `OS_fopen`
- `OS_ConsoleReadPassword`

2. Update `src/harness/CMakeLists.h` and add a new conditional section for "os/foo.h", based on existing conditions for Windows, MacOS etc. 

For example:

```
...
elseif( _FOO_ )
target_sources(harness PRIVATE
    os/foo.c
)
...
```

## IO Platform (windowing / input / rendering)

A `Platform` in _dethrace_ implements windowing, rendering and input handling.

The default platform is `SDL_OpenGL`, which uses SDL for windowing and input, and OpenGL for rendering. See [platforms/sdl_opengl.c](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/platforms/sdl_opengl.c).

To add a new `Platform`:

1. Create `platforms/my_platform.c` file and add a `Harness_Platform_Init` function. Hook up all the function pointers using the `sdl_opengl` [implementation](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/platforms/sdl_opengl.h) as a guide.

2. Add a new conditional section in `src/harness/CMakeLists.txt` for your new platform

For example:
```
if (IO_PLATFORM STREQUAL "My_Platform")
    target_sources(harness PRIVATE
        io_platforms/my_platform.c
    )
endif()
```

3. Run cmake to update your build with the new platform
```sh
cd build
cmake -DIO_PLATFORM=My_Platform ..
```

4. Build
```
cmake --build .
```
