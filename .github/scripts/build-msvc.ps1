if ($($Env:MATRIX_PLATFORM) -eq "x86") {
    $sdl_path = "x86"
} else {
    $sdl_path = "x64"
}

$sdl2_version = "2.0.18"
$ninja_version = "1.10.2"

# install deps
Invoke-WebRequest -Uri https://www.libsdl.org/release/SDL2-devel-$sdl2_version-VC.zip -OutFile $Env:TEMP\SDL2-devel.zip
Invoke-WebRequest -Uri https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-win.zip -OutFile $Env:TEMP\ninja-win.zip
Expand-Archive $Env:TEMP\SDL2-devel.zip -DestinationPath $Env:TEMP
Expand-Archive $Env:TEMP\ninja-win.zip -DestinationPath $Env:TEMP\ninja
echo "$Env:TEMP\ninja" | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append

# build
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON "-DSDL2_ROOT_DIR=$($Env:TEMP)\SDL2-$sdl2_version" -B build
cmake --build build --config RelWithDebInfo

# copy SDL2.dll to build folder, so tests can run
cp $Env:TEMP\SDL2-$sdl2_version\lib\$sdl_path\SDL2.dll build

# package artifact
7z a windows-$Env:MATRIX_PLATFORM.zip .\build\dethrace.exe .\build\dethrace.pdb $Env:TEMP\SDL2-$sdl2_version\lib\$sdl_path\SDL2.dll
