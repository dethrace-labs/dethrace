# Dethrace  

![CI](https://github.com/jeff-1amstudios/dethrace/workflows/CI/badge.svg)

![Discord Chat Channel](docs/discord-badge.jpg) [Discord Chat Channel](https://discord.gg/f5StsuP)

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

```sh
make build
```

## Run tests

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
