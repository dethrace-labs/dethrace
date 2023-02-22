# Porting Dethrace to other systems

## Operating Systems

Assuming an operating system called _foo_, follow the steps to add support for it.

1. Add a new file `os/foo.h` and implement the required functions defined in [os.h](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/include/harness/os.h):
- `OS_GetTime`
- `OS_Sleep`
- `OS_Basename`
- `OS_GetFirstFileInDirectory`
- `OS_GetNextFileInDirectory`
- `OS_IsDebuggerPresent`
- `OS_InstallSignalHandler`

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

An `IOPlatform` in _dethrace_ implements windowing and input handling, and points to a _renderer_.

The default IO platform is `SDL_OpenGL`, which uses SDL for windowing and input, and OpenGL for rendering. See [io_platforms/sdl_gl.c](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/io_platforms/sdl_gl.c).

To add a new `IOPlatform`:

1. Create `io_platforms/my_platform.c` file and implement the required functions defined in [io_platforms/io_platform.h](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/io_platforms/io_platform.h):
- `Window_Create`
- `Window_PollEvents`
- `Window_Swap`
- `Window_Close`
- `Input_GetKeyMap`
- `Input_IsKeyDown`

`Window_Create` returns a `tRenderer*`, which must implement the interface defined in [renderers/renderer.h](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/renderers/renderer.h). See [renderers/gl](https://github.com/dethrace-labs/dethrace/tree/main/src/harness/renderers/gl) for an example.

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
