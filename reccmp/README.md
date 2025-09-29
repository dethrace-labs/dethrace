# Docker container for running cross-platform MSVC 4.20

To run MSVC 4.20 outside of a non-Windows environment, you can use a Docker image and [Wine](https://www.winehq.org/)

## Original binary
We are targetting being accurate to CARM95.EXE.
- Created date: "16 October 1997"
- SHA256 hash: `c6040203856b71e6a22d2a29053a1eadd1a2ab41bce97b6031d745079bc07bdf`
- [Download from archive.org](https://archive.org/download/carm-95/CARM95.EXE)

## Build container image
```sh
docker buildx build --platform linux/amd64 -t msvc420-wine .
```

## Running the container

When running this container, you must define:

| Name       | Example value | Description
|------------|----------|-------|
| `CMAKE_FLAGS` | `-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on` | Environment variable |
| `/source` | `/code/dethrace` | Volume mount. Path to the top-level dethrace directory |
| `/build` | `/code/dethrace/build-msvc420` | Volume mount. This directory must exist but can start off empty. Note that this build directory _cannot be_ the same as your "regular" build directory. |
| `/original` | `/games/carma` | Volume mount. Path to a directory with a copy of the original CARM95.EXE file |

### Diffing a single function
This is the primary flow for making a change to the code and viewing the comparison to the original executable.

```sh
docker run --platform linux/amd64 \
    -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
    -v <PATH_TO_DETHRACE_DIR>:/source \
    -v <PATH_TO_DETHRACE_BUILD_DIR>:/build \
    -v <PATH_TO_CARMA_DIR>:/original:ro \
    msvc420-wine -- \
    reccmp-reccmp --target CARM95 --verbose FUNCTION_ADDRESS
```

### Generating an HTML diff

```sh
docker run --platform linux/amd64 \
    -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
    -v <PATH_TO_DETHRACE_DIR>:/source \
    -v <PATH_TO_DETHRACE_BUILD_DIR>:/build \
    -v <PATH_TO_CARMA_DIR>:/original:ro \
    msvc420-wine -- \
    reccmp-reccmp --target CARM95 --silent --html report.html
```

After running, a `report.html` file will be created in the build-msvc420 directory.

### Wrapping it up

I wrapped this logic up into a little `zsh` function, which makes it easy to run from command line.
- `dr-reccmp` to generate the overall html report
- `dr-reccmp 0x123456` to diff a single function.

```zsh
function dr-reccmp {
  local arg=()

  if [[ -n "$1" ]]; then
    arg+=(--verbose "$1")
  else
    # only download latest report if changed
    curl -fsSL --etag-save reccmp-report-etag-new.txt --etag-compare reccmp-report-etag.txt -o reccmp-report-main.json https://raw.githubusercontent.com/dethrace-labs/reccmp-report/main/report.json
    if [ -s reccmp-report-etag-new.txt ]; then
      mv -f reccmp-report-etag-new.txt reccmp-report-etag.txt
    fi
    arg=()
    arg+=(--html /source/reccmp-report.html)
    arg+=(--diff /source/reccmp-report-main.json)
    arg+=(--svg /source/reccmp.svg)
  fi

  docker run --platform linux/amd64 \
    -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
    -v .:/source \
    -v ./build_msvc42:/build \
    -v /opt/carma/:/original:ro \
    msvc420-wine -- \
    reccmp-reccmp --target CARM95 "${arg[@]}"
}
```

### Make a pull request change

reccmp will run against the code in the PR branch. If any functions decrease in accuracy the PR validation will emit a warning. If the functions which decreased were not changed in the PR, the PR can still be accepted, as the decrease will generally be due to compiler entropy.

When the PR is merged into `main`, the updated report is stored in https://github.com/dethrace-labs/reccmp-report, and this is used to compare the next PR
