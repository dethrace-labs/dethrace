#!/bin/bash
set -e

if [ "${MATRIX_PLATFORM}" = "clang" ]
then
    CC=clang
    CXX=clang++
else
    CC=gcc
    CXX=g++
fi

# install deps
sudo apt-get update -qq > /dev/null
sudo apt-get install -qq -y libsdl2-dev > /dev/null

# build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON -B build
cmake --build build -- -j 4

# test
CTEST_OUTPUT_ON_FAILURE=1 make test

# package artifact
tar -czvf linux-amd64.tar.gz build/dethrace
