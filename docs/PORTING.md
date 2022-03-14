# Porting Dethrace to other systems

## Operating Systems

See: xxx

Assuming an operating system called _foo_, follow the steps to add support for it.

1. Add a new file `os/foo.h` and implement the following functions defined in `os/os.h`:

- `OS_IsDebuggerPresent`
- `OS_InstallSignalHandler`
- `OS_PrintStacktrace`

2. Update `CMakeLists.h` and add a new conditional section for "os/foo.h", based on existing conditions for Windows, MacOS etc.

```
...
elseif( FOO )
target_sources(harness PRIVATE
    os/foo.c
)
...
```

## Rendering / Windowing / Input
