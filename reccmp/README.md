# Docker container for running cross-platform MSVC 4.20

To run MSVC 4.20 outside of a non-Windows environment, you can use a Docker image and [Wine](https://www.winehq.org/)

## Original binary
We are targetting being accurate to CARM95.EXE.
- Created date: "16 October 1997"
- SHA256 hash: `c6040203856b71e6a22d2a29053a1eadd1a2ab41bce97b6031d745079bc07bdf`

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

### Generating an HTML diff

This is the primary flow for making a change to the code and viewing the comparison to the original executable.

```sh
docker run --platform linux/amd64 \
    -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
    -v <PATH_TO_DETHRACE_DIR>:/source \
    -v <PATH_TO_DETHRACE_BUILD_DIR>:/build \
    -v <PATH_TO_CARMA_DIR>:/orginal:ro \
    msvc420-wine -- \
    reccmp-reccmp --target CARM95 --silent --html reccmp-report.html --json /source/reccmp/report.json
```

After running, a `reccmp-report.html` file will be created in the build-msvc420 directory.

### Make a pull request change

The pull request will fail if the `reccmp/report.json` file is different from the version generated during build

docker run --platform linux/amd64 \
    -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
    -v .:/source \
    -v ./build_msvc42:/build \
    -v /Users/jeff/Downloads/carma/opt/c1:/orginal:ro \
    msvc420-wine -- \
    ls -lah /orginal
    reccmp-reccmp --target CARM95 --silent --html reccmp-report.html --json /source/reccmp/report.json
