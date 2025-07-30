#!/usr/bin/env bash
set -e

export WINEPREFIX=/wineprefix

# Configure build with CMake
wine cmake -B build source $CMAKE_FLAGS

# Build
wine cmake --build build -- -j1

# Update path to original binary
cd /source
reccmp-project detect --search-path /original

# Fix up wine paths to underlying linux paths so we can run reccmp outside of wine
cd /build
sed -i 's/Z://g' reccmp-build.yml

if [ "$#" -gt 0 ]; then
    exec "$@"
fi


# Unlock directories
#chmod -R 777 source build
