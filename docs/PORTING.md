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

## Platform (windowing / input)

A `Platform` in _dethrace_ implements windowing and input handling.

The default platform is `SDL2`, which uses SDL2 for windowing and input. See [platforms/sdl_opengl.c](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/platforms/sdl2.c).

To add a new `Platform`:

1. Create a `src/harness/my_platform.c` file where you'll implement your platform-specific callbacks.
   Define a public fully-initialized `const tPlatform_bootstrap MYPLATFORM_bootstrap ` variable in this file.

2. Add the the following code fragments to appropriate locations in `src/harness/harness.c`:
    ```c
   extern const tPlatform_bootstrap MYPLATFORM_bootstrap;
   ```
   ```c
   #ifdef DETHRACE_PLATFORM_MYPLATFORM
       &MYPLATFORM_bootstrap,
   #endif
   ```

3. Add new conditionals  to `CMakeLists.txt` and `src/harness/CMakeLists.txt` for your new platform

   For example:
   ```cmake
   # CMakeLists.txt
   option(DETHRACE_PLATFORM_MYPLATFORM "Enable my platform" OFF)
   if(DETHRACE_PLATFORM_MYPLATFORM)
       find_package(MyPlatform REQUIRED)
   endif()
   ```
   ```cmake
   # src/harness/CMakeLists.txt
      if(DETHRACE_PLATFORM_MYPLATFORM)
       target_sources(harness PRIVATE
           my_platform.c
       )
       target_compile_definitions(harness PRIVATE DETHRACE_PLATFORM_MYPLATFORM)
       target_link_libraries(harness PRIVATE MyPlatform::MyPlatform)
   endif()
   ```

4. Hook up all the function pointers using the [sdl2 platform](https://github.com/dethrace-labs/dethrace/blob/main/src/harness/platforms/sdl2.c) as a guide.

5. Run cmake to update your build with the new platform
   ```sh
   cd build
   cmake -DDETHRACE_PLATFORM_MYPLATFORM=ON ..
   cmake --build .
   ```

6. Build
   ```
   -cmake --build .
   ```
