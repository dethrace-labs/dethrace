# Dethrace

[![Workflow](https://github.com/dethrace-labs/dethrace/actions/workflows/workflow.yaml/badge.svg)](https://github.com/dethrace-labs/dethrace/actions/workflows/workflow.yml)
[![Twitter](https://badgen.net/badge/icon/twitter?icon=twitter&label)](https://twitter.com/dethrace_labs)
[![Discord Carmageddon server](https://badgen.net/badge/icon/discord?icon=discord&label)](https://discord.gg/f5StsuP)

Dethrace is an attempt to learn how the 1997 driving/mayhem game [Carmageddon](https://en.wikipedia.org/wiki/Carmageddon) works behind the scenes and rebuild it to run natively on modern systems.

## Status

<img width="50%" src="https://raw.githubusercontent.com/dethrace-labs/reccmp-report/refs/heads/main/progress.svg">

## Building

### Dependencies

Dethrace using CMake to build, and SDL2 at runtime. The easiest way to install them is via your favorite package manager.

OSX:

```sh
brew install SDL2 cmake
```

Linux:

```sh
apt-get install libsdl2-dev cmake
```

### Clone

Dethrace uses [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules), so we must pull them after the inital clone:

```sh
git clone https://github.com/dethrace-labs/dethrace
cd dethrace
git submodule update --init --recursive
```

### Build

Dethrace uses [cmake](https://cmake.org/)

To generate the build files:

```sh
mkdir build
cd build
cmake ..
```

Once cmake has generated the build files for your platform, run the build. For example:

```sh
make
```

## Running the game

Dethrace does not ship with any content. You'll need access to the data from the original game. If you don't have an original CD then you can [buy Carmageddon from GoG.com](https://www.gog.com/game/carmageddon_max_pack).

`dethrace` also supports the various freeware demos:

- [Original Carmageddon demo](https://rr2000.cwaboard.co.uk/R4/PC/carmdemo.zip)
- [Splat Pack demo](https://rr2000.cwaboard.co.uk/R4/PC/splatdem.zip)
- [Splat Pack Xmas demo](https://rr2000.cwaboard.co.uk/R4/PC/Splatpack_christmas_demo.zip)

Dethrace generally expects to be placed into the top level Carmageddon folder. You know you have the right folder when you see the original `CARMA.EXE` there. If you are on Windows, you must also place `SDL2.dll` in the same folder.

### Configuration INI file

Alternatively, you may configure a different Carmageddon directory and settings by providing a [dethrace.ini file](docs/CONFIGURATION.md).

### CD audio

Dethrace supports the GOG cd audio convention. If there is a `MUSIC` folder in the Carmageddon folder containing files `Track02.ogg`, `Track03.ogg` etc, then Dethrace will use those files in place of the original CD audio functions.

<img width="571" alt="Screenshot 2024-09-30 at 8 31 59 AM" src="https://github.com/user-attachments/assets/cec72203-9156-4c2a-a15a-328609e65c68">

## Background

Watcom debug symbols for an earlier internal build [were discovered](http://1amstudios.com/2014/12/02/carma1-symbols-dumped) named `DETHRSC.SYM` on the [Carmageddon Splat Pack](http://carmageddon.wikia.com/wiki/Carmageddon_Splat_Pack) expansion CD release. The symbols unfortunately did not match any known released executable, meaning they were interesting but not immediately usable to reverse engineer the game.

This is what it looked like from the Watcom debugger - the names of all the methods were present but the code location they were pointing to was junk:

![watcom-debugger](http://1amstudios.com/img/watcom-debugger.jpg)

We are slowly replacing the original assembly code with equivalent C code, function by function.

### Is "dethrace" a typo?

No, well, I don't think so at least. The original files according to the symbol dump were stored in `c:\DETHRACE`, and the symbol file is called `DETHSRC.SYM`. Maybe they removed the "a" to be compatible with [8.3 filenames](https://en.wikipedia.org/wiki/8.3_filename)?

## Changelog

[From the beginning until release](docs/CHANGELOG.md)

## Credits

- CrayzKirk (did the first manual matching up functions and data structures in the DOS executable to the debugging symbols and proved it was possible!)
- The developer at Stainless Software who left an old debugging .SYM file on the Splat Pack CD ;)
- https://github.com/isledecomp/reccmp tooling

## Legal

Dethrace is released to the Public Domain. The documentation and function provided by Dethrace may only be utilized with assets provided by ownership of Carmageddon.

The source code in this repository is for non-commerical use only. If you use the source code you may not charge others for access to it or any derivative work thereof.

Dethrace and any of its' maintainers are in no way associated with or endorsed by SCi, Stainless Software or THQ Nordic.
