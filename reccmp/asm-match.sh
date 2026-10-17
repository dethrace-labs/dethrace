#!/usr/bin/env zsh

set -euo pipefail

WORKDIR="$(pwd)"

arg=("$@")

docker run --rm --platform linux/amd64 \
  -e "XDG_RUNTIME_DIR=/tmp" \
  -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
  -v "$WORKDIR:/source" \
  -v "$WORKDIR/build_msvc42:/build" \
  -v "$CARMADIR:/original:ro" \
  msvc420-wine -- \
  reccmp-reccmp --target CARM95 "${arg[@]}"
