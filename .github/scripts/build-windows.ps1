
if ($($Env:MATRIX_PLATFORM) -eq "x86") {
    $sdl_path = "x86"
} else {
    $sdl_path = "x64"
}

# install deps
Invoke-WebRequest -Uri https://www.libsdl.org/release/SDL2-devel-2.0.12-VC.zip -OutFile $Env:TEMP\SDL2-devel.zip
Invoke-WebRequest -Uri https://github.com/ninja-build/ninja/releases/download/v1.10.1/ninja-win.zip -OutFile $Env:TEMP\ninja-win.zip
Expand-Archive $Env:TEMP\SDL2-devel.zip -DestinationPath $Env:TEMP
Expand-Archive $Env:TEMP\ninja-win.zip -DestinationPath $Env:TEMP\ninja
echo "$Env:TEMP\ninja" | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append

echo "Temp directory is " $Env:TEMP
dir $($Env:TEMP)/
dir $($Env:TEMP)/SDL2-2.0.12/
dir $($Env:TEMP)/SDL2-2.0.12/lib

# build
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSDL2=ON "-DSDL2_ROOT_DIR=$($Env:TEMP)\SDL2-2.0.12" -B build
cmake --build build --config RelWithDebInfo

7z a windows-$($Env:MATRIX_PLATFORM).zip build/dethrace.exe build/dethrace.pdb $($Env:TEMP)/SDL2-2.0.12/lib/$sdl_path/SDL2.dll