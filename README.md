# Dethrace  

![CI](https://github.com/jeff-1amstudios/dethrace/workflows/CI/badge.svg)

![Discord Chat Channel](docs/discord-badge.jpg) [Discord Chat Channel](https://discord.gg/f5StsuP)

## Install dependencies

### SDL2

OSX:
```sh
brew install SDL2
```

## Run tests

```sh
make test
```

### Run a single test
```sh
DR_TEST_ARGS="-n test_name" make test
```

### Run game
```sh
export DETHRACE_ROOT_DIR=/path/to/carmageddon
make run
```