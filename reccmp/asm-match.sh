#!/usr/bin/env zsh

set -euo pipefail

# Always operate relative to where the script is called
WORKDIR="$(pwd)"

arg=()

if [[ $# -gt 0 ]]; then
  arg+=(--verbose "$1")
  arg+=(--no-color)
else
  REPORT_URL="https://raw.githubusercontent.com/dethrace-labs/reccmp-report/main/report.json"
  REPORT_FILE="$WORKDIR/reccmp-report-main.json"

  curl -fsSL -o "$REPORT_FILE" "$REPORT_URL"

  if [[ -s "$WORKDIR/reccmp-report-etag-new.txt" ]]; then
    mv -f \
      "$WORKDIR/reccmp-report-etag-new.txt" \
      "$WORKDIR/reccmp-report-etag.txt"
  fi

  arg+=(--html /source/reccmp-report.html)
  arg+=(--diff /source/reccmp-report-main.json)
  # arg+=(--svg /source/reccmp/progress.svg)
  # arg+=(--svg-icon /source/reccmp/carma_32x32_4.png)
fi

docker run --rm --platform linux/amd64 \
  -e "XDG_RUNTIME_DIR=/tmp" \
  -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
  -v "$WORKDIR:/source" \
  -v "$WORKDIR/build_msvc42:/build" \
  -v "/Users/jeff/Downloads/carma/opt/c1:/original:ro" \
  msvc420-wine -- \
  reccmp-reccmp --target CARM95 "${arg[@]}"
