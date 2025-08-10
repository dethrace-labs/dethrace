FROM docker.io/library/debian:stable-slim

# Gather dependencies
RUN dpkg --add-architecture i386
RUN apt-get update -y
RUN apt-get install git wine wine64 wine32 wget unzip pip -y

ENV WINEPREFIX=/wineprefix
# Silence debug warnings in wine (creates noise during compile)
ENV WINEDEBUG="-all"

COPY set-env.reg /tmp/set-env.reg

# Create and initialize Wine prefix
RUN mkdir -p $WINEPREFIX && \
    wineboot --init && \
    # wait for wineboot to finish
    wineserver -w && \
    wine regedit /S /tmp/set-env.reg && \
    # wait for regedit to finish
    wineserver -w

# Install MSVC 4.20 and CMake for Windows
RUN git clone https://github.com/itsmattkc/MSVC420 $WINEPREFIX/drive_c/msvc

# Install CMake for Windows
RUN wget --quiet https://github.com/Kitware/CMake/releases/download/v3.26.6/cmake-3.26.6-windows-i386.zip
RUN unzip -q cmake-3.26.6-windows-i386.zip -d $WINEPREFIX/drive_c
RUN mv $WINEPREFIX/drive_c/cmake-3.26.6-windows-i386 $WINEPREFIX/drive_c/cmake
RUN rm cmake-3.26.6-windows-i386.zip

# Install Ninja for Windows
RUN wget --quiet https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-win.zip
RUN unzip -q ninja-win.zip -d $WINEPREFIX/drive_c/ninja
RUN rm ninja-win.zip

# Install reccmp
RUN pip install --break-system-packages git+https://github.com/isledecomp/reccmp

# Set up entrypoint script to perform the build
COPY entrypoint.sh entrypoint.sh
ENTRYPOINT [ "./entrypoint.sh" ]
