# Dethrace  

![CI](https://github.com/jeff-1amstudios/dethrace/workflows/CI/badge.svg)

![Discord Chat Channel](docs/discord-badge.jpg) [Discord Chat Channel](https://discord.gg/f5StsuP)

Dethrace is an attempt to learn how the 1997 driving/mayhem game [Carmageddon](https://en.wikipedia.org/wiki/Carmageddon) works behind the scenes and run it on modern systems.

## Introduction
In 2014, Watcom debugging symbols for an earlier internal build [were discovered](http://1amstudios.com/2014/12/02/carma1-symbols-dumped) on the [Carmageddon Splat Pack](http://carmageddon.wikia.com/wiki/Carmageddon_Splat_Pack) expansion CD release. The symbols unfortunately did not match any known released executable, meaning they were interesting but not immediately usable to reverse engineer the game.

This is what it looked like from the Watcom debugger - the names of all the methods were present but the code location they were pointing to was junk:

![watcom-debugger](http://1amstudios.com/img/watcom-debugger.jpg)

_CrayzKirk_ from the Carmageddon community then picked it up and did the painstaking work of manually matching up functions and data structures in the DOS executable to the debugging symbols.

After months of work, we are slowly replacing the original assembly code with equivalent C code.

## Dependencies

### SDL2

The easiest way to install SDL is via your favorite package manager.

OSX:
```sh
brew install SDL2
```

Linux:
```sh
apt-get install libsdl2-dev
```

### Root content directory

Dethrace does not ship with any game content. You'll need access to the data from the original game. If you don't have an original CD then you can [buy Carmageddon from GoG.com](https://www.gog.com/game/carmageddon_max_pack). 

Point Dethrace at the Carmageddon install directory:
```sh
export DETHRACE_ROOT_DIR=/path/to/carmageddon
```

## Build

Dethrace uses [cmake](https://cmake.org/) for generating build files.

To generate the build files (generally only required once):
```sh
mkdir build
cd build
cmake ..
```

Build:
```sh
make && ./dethrace
```

### Run

After building, `build/dethrace` is created

```sh
./dethrace [args]
```


## Run tests

A subset of tests do not require `DETHRACE_ROOT_DIR`. They run via Github actions when code is committed to this repo. This allows us to keep nice and clean and avoid storing any potentially legally problematic resouces in our repo.

The majority of tests _do_ require `DETHRACE_ROOT_DIR`. 

To run the full test suite, you must have a copy of the original *Splat Pack* data.

```sh
export DETHRACE_ROOT_DIR=/path/to/carmageddon_splat_pack
```

To run 

```sh
make test
```

To run a single test
```sh
DETHRACE_TEST_ARGS="-n test_name" make test
```

## Run game
```sh
make run
```

## Changelog
[From the beginning until release](docs/CHANGELOG.md)

## Credits
- CrayzKirk (manually matching up functions and data structures in the executable to the debugging symbols)
- The developer at Stainless Software who left an old debugging .SYM file on the Splat Pack CD ;)

## Legal
Dethrace is released to the Public Domain. The documentation and function provided by Dethrace may only be utilized with assets provided by ownership of Carmageddon.

The source code in this repository is for non-commerical use only. If you use the source code you may not charge others for access to it or any derivative work thereof.

Dethrace and any of its' maintainers are in no way associated with or endorsed by SCi, Stainless Software or THQ Nordic.
