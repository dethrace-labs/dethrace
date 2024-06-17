# Dethrace  

[![Workflow](https://github.com/dethrace-labs/dethrace/actions/workflows/workflow.yaml/badge.svg)](https://github.com/dethrace-labs/dethrace/actions/workflows/workflow.yml)
[![Twitter](https://badgen.net/badge/icon/twitter?icon=twitter&label)](https://twitter.com/dethrace_labs)
[![Discord Carmageddon server](https://badgen.net/badge/icon/discord?icon=discord&label)](https://discord.gg/f5StsuP)

Dethrace is an attempt to learn how the 1997 driving/mayhem game [Carmageddon](https://en.wikipedia.org/wiki/Carmageddon) works behind the scenes and rebuild it to run natively on modern systems.

## Progress
(Follow us on Discord or Twitter to get notified of updates!)

#### Last updated June 17 2024
- 92% of functions implemented
- Latest screenshot:

<img width="752" alt="Screenshot 2024-05-27 at 8 44 10 AM" src="https://github.com/dethrace-labs/dethrace/assets/1063652/10b3b579-1eb1-4242-8b56-ff062cfff810">


## Background
Watcom debug symbols for an earlier internal build [were discovered](http://1amstudios.com/2014/12/02/carma1-symbols-dumped) named `DETHRSC.SYM` on the [Carmageddon Splat Pack](http://carmageddon.wikia.com/wiki/Carmageddon_Splat_Pack) expansion CD release. The symbols unfortunately did not match any known released executable, meaning they were interesting but not immediately usable to reverse engineer the game.

This is what it looked like from the Watcom debugger - the names of all the methods were present but the code location they were pointing to was junk:

![watcom-debugger](http://1amstudios.com/img/watcom-debugger.jpg)

_CrayzKirk_ from the Carmageddon community picked it up and did a lot of painstaking work manually matching up many functions and data structures in the DOS executable to the debugging symbols.

We are slowly replacing the original assembly code with equivalent C code, function by function.

### Is "dethrace" a typo?
No, well, I don't think so at least. The original files according to the symbol dump were stored in `c:\DETHRACE`, and the symbol file is called `DETHSRC.SYM`. Maybe they removed the "a" to be compatible with [8.3 filenames](https://en.wikipedia.org/wiki/8.3_filename)?

## Game content

Dethrace does not ship with any game content. You'll need access to the data from the original game. If you don't have an original CD then you can [buy Carmageddon from GoG.com](https://www.gog.com/game/carmageddon_max_pack).

`dethrace` also supports the various freeware demos:
- [Original Carmageddon demo](https://rr2000.cwaboard.co.uk/R4/PC/carmdemo.zip)
- [Splat Pack demo](https://rr2000.cwaboard.co.uk/R4/PC/splatdem.zip)
- [Splat Pack Xmas demo](https://rr2000.cwaboard.co.uk/R4/PC/Splatpack_christmas_demo.zip)

Lots of other fun things are available from the [Road Reaction site](https://rr2000.cwaboard.co.uk/pc-files#c1)


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

On cmake has generated the build files for your platform, run the build. For example:
```sh
make
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
./dethrace_test
```

To run a single test
```sh
DETHRACE_TEST_ARGS="-n test_name" make test
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
