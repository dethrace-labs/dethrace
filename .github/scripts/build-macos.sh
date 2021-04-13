#!/bin/bash
set -e

# install deps
brew install SDL2

# build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON -B build
cmake --build build -- -j 4

# package artifact
tar -czvf darwin-amd64.tar.gz dethrace