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
releasename="dethrace-${BUILD_TAG}-darwin-${PLATFORM_ARCH}"
rm -rf "$releasename"
mkdir "$releasename"
cp build/dethrace "$releasename/dethrace"
tar -czvf "$releasename.tar.gz" "$releasename"
echo "::set-output name=filename::$releasename.tar.gz"
