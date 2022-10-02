#!/bin/bash
set -e

if [[ "${GITHUB_REF_TYPE}" == "tag" ]]; then
    BUILD_TAG=${GITHUB_REF_NAME}
else 
    BUILD_TAG=$(git rev-parse --short HEAD)
fi

# install deps
brew install SDL2

# build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_OSX_ARCHITECTURES=x86_64 -DBUILD_TESTS=ON -DDETHRACE_WERROR=ON -B build
cmake --build build -- -j 4

# package artifact
tar -czvf dethrace-${BUILD_TAG}-darwin-amd64.tar.gz build/dethrace

echo "::set-output name=filename::dethrace-${BUILD_TAG}-darwin-amd64.tar.gz"
