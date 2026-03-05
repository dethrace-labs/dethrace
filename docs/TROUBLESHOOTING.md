# Troubleshooting

## "Could not find a supported platform"

Example output from MacOS:
```
[DEBUG] Harness_InitPlatform Attempting video driver "sdl3"
[DEBUG] Harness_InitPlatform Try platform "sdl3"...
Failed to load SDL3 function: SDL_free (dlsym(RTLD_DEFAULT, SDL_free): symbol not found)
[DEBUG] Harness_InitPlatform Attempting video driver "sdl2"
[DEBUG] Harness_InitPlatform Try platform "sdl2"...
Failed to load SDL2 function: SDL_Init (dlsym(RTLD_DEFAULT, SDL_Init): symbol not found)
[DEBUG] Harness_InitPlatform Attempting video driver "sdl1"
[DEBUG] Harness_InitPlatform Try platform "sdl1"...
Could not find SDL1 library
Could not find a supported platform
```

### Solution
Install SDL3

- MacOS `brew install sdl3`

## "Couldn't open General Settings file"

This means dethrace could not find the Carmageddon data files. 

### Solution

Move the dethrace binary (.app bundle on MacOS) into your Carmageddon directory. You have the right directory if it contains `carma.exe` and a `DATA` directory. 

Alternatively, you can configure which directory to load Carmageddon data from.

Configuration is supported from the command line - eg `./dethrace --dir /path/to/carma` or specified in an [ini file](https://github.com/dethrace-labs/dethrace/blob/main/docs/CONFIGURATION.md)
