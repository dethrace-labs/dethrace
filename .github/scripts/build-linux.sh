#!/bin/bash
set -e

if [ "${MATRIX_PLATFORM}" = "clang" ]
then
    export CC=clang
    export CXX=clang++
else
    export CC=gcc
    export CXX=g++
fi

# install deps
sudo apt-get update -qq > /dev/null
sudo apt-get install -qq -y libsdl2-dev > /dev/null

# build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON -DDETHRACE_WERROR=ON -B build
cmake --build build -- -j 4

# package artifact
tar -czvf linux-amd64.tar.gz build/dethrace
