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

if [[ "${GITHUB_REF_TYPE}" == "tag" ]]; then
    BUILD_TAG=${GITHUB_REF_NAME}
else 
    BUILD_TAG=$(git rev-parse --short HEAD)
fi

# install deps
sudo apt-get update -qq > /dev/null
sudo apt-get install -qq -y libsdl2-dev > /dev/null

# build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=ON -DDETHRACE_WERROR=ON -B build
cmake --build build -- -j 4

# package artifact
releasename="dethrace-${BUILD_TAG}-linux-amd64"
rm -rf "$releasename"
mkdir "$releasename"
cp build/dethrace "$releasename/dethrace"
tar -czvf "$releasename.tar.gz" "$releasename"
echo "::set-output name=filename::$releasename.tar.gz"

