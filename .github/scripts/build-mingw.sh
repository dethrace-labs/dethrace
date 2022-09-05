#!/bin/bash
set -e

if test -z "$MSYSTEM"; then
  # mingw@Linux
  if test $MATRIX_PLATFORM = "x86"; then
      sdl_path="i686-w64-mingw32"
      apt_packages="gcc-mingw-w64-i686 g++-mingw-w64-i686"
      export CC=i686-w64-mingw32-gcc-win32
      export CXX=i686-w64-mingw32-g++-win32
  else
      sdl_path="x86_64-w64-mingw32"
      apt_packages="gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64"
      export CC=x86_64-w64-mingw32-gcc-win32
      export CXX=x86_64-w64-mingw32-g++-win32
  fi

  # install deps
  sudo apt-get update -qq > /dev/null
  sudo apt-get install -qq -y $apt_packages > /dev/null

  SDL2_VERSION=2.0.18

  wget -nv https://www.libsdl.org/release/SDL2-devel-$SDL2_VERSION-mingw.tar.gz -O /tmp/SDL2-devel-$SDL2_VERSION-mingw.tar.gz
  tar -xf /tmp/SDL2-devel-$SDL2_VERSION-mingw.tar.gz -C /tmp

  $CC --verbose
  $CXX --verbose

  # build
  cmake -DCMAKE_SYSTEM_NAME=Windows -DSDL2_ROOT_DIR=/tmp/SDL2-$SDL2_VERSION/$sdl_path -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON -DDETHRACE_WERROR=ON -B build -DCMAKE_EXE_LINKER_FLAGS_INIT=-lssp
  cmake --build build -- -j 4

  # package artifact
  tar -czvf mingw-$MATRIX_PLATFORM.tar.gz -C build dethrace.exe -C /tmp/SDL2-$SDL2_VERSION/$sdl_path/bin SDL2.dll
else
  # mingw@Windows
  if test $MATRIX_PLATFORM = "x86"; then
      bin_path="/mingw32/bin"
      pacman_packages="mingw-w64-i686-cmake mingw-w64-i686-make mingw-w64-i686-gcc mingw-w64-i686-SDL2"
  else
      bin_path="/mingw64/bin"
      pacman_packages="mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2"
  fi

  # install deps
  pacman -S --noconfirm $pacman_packages

  # build
  cmake -G "MinGW Makefiles" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON -B build
  cmake --build build -- -j 4

  # package artifact
  tar -czvf mingw-$MATRIX_PLATFORM.tar.gz -C build dethrace.exe -C $bin_path SDL2.dll
fi
