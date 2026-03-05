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


If you do not have Carmageddon data files, you can use this script to download the demo data to your [pref path](https://wiki.libsdl.org/SDL3/SDL_GetPrefPath) which dethrace will load automatically.

#### Bash
```sh
URL="https://rr2000.cwaboard.co.uk/R4/PC/carmdemo.zip"
APP_NAME="dethrace"

case "$(uname -s)" in
    Linux*) PREF_PATH="${XDG_DATA_HOME:-$HOME/.local/share}/$APP_NAME/" ;;
    Darwin*) PREF_PATH="$HOME/Library/Application Support/$APP_NAME/" ;;
    *) echo "Unsupported OS"; exit 1 ;;
esac

mkdir -p "$PREF_PATH"

TMP_ZIP="$(mktemp)"

curl -L -A "Mozilla/5.0" -o "$TMP_ZIP" "$URL"
unzip -o "$TMP_ZIP" -d "$PREF_PATH"
rm -f "$TMP_ZIP"

```

#### Powershell
```
$URL = "https://rr2000.cwaboard.co.uk/R4/PC/carmdemo.zip"
$AppName = "dethrace"

$PrefPath = Join-Path $env:APPDATA $AppName
New-Item -ItemType Directory -Force -Path $PrefPath | Out-Null

$TmpZip = Join-Path $env:TEMP "carmdemo.zip"

Write-Host "Downloading..."
Invoke-WebRequest -Uri $URL -Headers @{ "User-Agent"="Mozilla/5.0" } -OutFile $TmpZip

Write-Host "Extracting..."
Expand-Archive -Force -Path $TmpZip -DestinationPath $PrefPath

Remove-Item $TmpZip
```
