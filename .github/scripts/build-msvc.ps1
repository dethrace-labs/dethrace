if ($($Env:PLATFORM_ARCH) -eq "x86") {
    $sdl_path = "x86"
} else {
    $sdl_path = "x64"
}

if ($($Env:GITHUB_REF_TYPE) -eq "tag") {
    $BUILD_TAG=$Env:GITHUB_REF_NAME
} else {
    $BUILD_TAG=$(git rev-parse --short HEAD)
}

$sdl2_version = "2.24.0"

# install deps
Invoke-WebRequest -Uri https://www.libsdl.org/release/SDL2-devel-$sdl2_version-VC.zip -OutFile $Env:TEMP\SDL2-devel.zip
Expand-Archive $Env:TEMP\SDL2-devel.zip -DestinationPath $Env:TEMP

# build
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON "-DSDL2_ROOT_DIR=$($Env:TEMP)\SDL2-$sdl2_version" -B build
cmake --build build --config RelWithDebInfo

# copy SDL2.dll to build folder, so tests can run
cp $Env:TEMP\SDL2-$sdl2_version\lib\$sdl_path\SDL2.dll build

# package artifact
7z a dethrace-$BUILD_TAG-windows-$Env:MATRIX_PLATFORM.zip .\build\dethrace.exe .\build\dethrace.pdb $Env:TEMP\SDL2-$sdl2_version\lib\$sdl_path\SDL2.dll

echo "::set-output name=filename::dethrace-$BUILD_TAG-windows-$Env:MATRIX_PLATFORM.zip"
