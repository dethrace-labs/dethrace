# Docker container for running cross-platform MSVC 4.20

To run MSVC 4.20 outside of a non-Windows environment, you can use a Docker image and [Wine](https://www.winehq.org/)

## Build container image
```sh
docker buildx build --platform linux/amd64 -t msvc420-wine .
```

## Running the container

When running this container, you must pass
1. `CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on".
2. Path to the top-level dethrace directory (for example `/code/dethrace`).
3. Path to a msvc420-specific build directory (for example `/code/dethrace/build-msvc420`). This directory must exist but can start off empty. Note that this build directory _cannot be_ the same as your "regular" build directory.
4. Path to a directory with a copy of the original CARM95.EXE file (for example `/games/carma`). CARM95.EXE must match sha256 `c6040203856b71e6a22d2a29053a1eadd1a2ab41bce97b6031d745079bc07bdf`.

### Generating an HTML diff

This is the primary flow for making a change to the code, recompiling it with MSVC, then viewing the diff.

After running, a `diff.html` file will be created in the build-msvc420 directory.

```sh
docker run --platform linux/amd64 \
    -e CMAKE_FLAGS="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DMSVC_42_FOR_RECCMP=on" \
    -v <PATH_TO_DETHRACE_DIR>:/source \
    -v <PATH_TO_DETHRACE_BUILD_DIR>:/build \
    -v <PATH_TO_CARMA_DIR>:/orginal:ro \
    msvc420-wine -- \
    reccmp-reccmp --target CARM95 --silent --html diff.html
```
