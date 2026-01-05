# Dethrace Contributing Guidelines
Thank you for investing your time in contributing to our project! Contributions can be in the form of bug reports or code
additions / fixes. Please read this document to learn how to contribute as effectively as possible. If you have any
questions or concerns, please ask in #dethrace channel on our [Discord chat](https://discord.gg/f5StsuP)

## Reporting bugs
To report a bug, ensure you have a GitHub account. Search the issues page to see if the bug has already been reported.
If not, create a new issue and write the steps to reproduce. A screenshot or a video can often be useful.
Please state which architecture and version of the game you are running, e.g.
```
Harness_Init version: v0.4.0-15-g31afabc
Windows (x86-64)
```

## Binary accuracy
We aims to be as accurate as possible, matching the recompiled instructions to the original machine code as much as possible. The goal is to provide a workable codebase that can be modified, improved, and ported to other platforms later on. We are using [reccmp](https://github.com/isledecomp/reccmp?tab=readme-ov-file) to diff the recompiled code with the original Windows 95 Splatpack binary (originally compiled with MSVC 4.2).

See: [details](https://github.com/dethrace-labs/dethrace/blob/main/reccmp/README.md) of how we are using reccmp.

## Project goals

- Faithfully reproduce the original gameplay from Carmageddon 1
- Provide documentation in the form of code and docs of the internal workings of the Carmageddon 1 engine
- Fix bugs in the original game logic
- Provide quality of life improvements (eg support for modern platforms, higher resolutions, higher framerates, increased limits for cars/tracks etc)

In contrast, extending or altering the gameplay of the base game is not encouraged.

### I do not agree with the goals, what can I do instead?

Fork us!

Many of these will never be accepted into main, but we love seeing creative and interesting ways to modify Dethrace.

Please post your work (screenshots, videos etc) in our [Interesting Forks](https://github.com/dethrace-labs/dethrace/discussions/categories/interesting-forks) discussion thread.


## Legal stuff

### License

By contributing your code, you agree to license your contribution under [GPL v3](https://github.com/dethrace-labs/dethrace/blob/main/LICENSE).


# Contributing code

Join us in the #dethrace channel on our [Discord chat](https://discord.gg/f5StsuP). We'd love to talk!

## Code style
We expect code to be formatted with our [clang-format](https://github.com/dethrace-labs/dethrace/blob/main/.clang-format) style.

We recommend to configure your IDE to run clang-format on save. Heres how to enable it in Visual Studio Code for example:
![image](https://user-images.githubusercontent.com/78985374/200776372-8d5ec29d-8f39-4970-be69-7cc2abaf724d.png)

## Fixing a bug in the original logic
Please wrap fixes for bugs in the original game logic.
```c
#if defined(DETHRACE_FIX_BUGS)
  // describe the original logic bug
  <code to fix bug>
#endif
```

## Function and variable names
Functions and global variables and local variables are all generated from a symbol dump and captured in our [codegen-output](https://github.com/dethrace-labs/codegen-output). In very uncommon cases, it is required to add extra local variables in cases where the retail code differs from the code at the time of the symbol dump (the symbol dump does not match the retail binary). If this is required, please clearly mark those variables as being added.

```c
// Added by dethrace
float velocity;

```

## Inline BRender functions
Please use the [inline BRender functions](https://github.com/dethrace-labs/dethrace/blob/main/src/BRSRC13/include/brender/br_inline_funcs.h) where possible.

Instead of
```c
vector_a->v[0] = vector_b->v[0] * 6.0f;
vector_a->v[1] = vector_b->v[1] * 6.0f;
vector_a->v[2] = vector_b->v[2] * 6.0f;
```

it should look like
```c
BrVector3Scale(&vector_a, &vector_b, 6.0f);
```

## Magic values
Many "magic" values are already defined as enums from the code-gen.

Before adding something like `#define DEPTH_EFFECT_WATER 1`, look at [dr_types.h](https://github.com/dethrace-labs/dethrace/blob/main/src/DETHRACE/dr_types.h) and try to find the existing enum. For example:

```c
typedef enum tSpec_vol_depth_effect {
    eSpec_dep_acid = 0,
    eSpec_dep_water = 1,
    eSpec_dep_slight_fog = 2,
    eSpec_dep_med_fog = 3,
    eSpec_dep_thick_fog = 4
} tSpec_vol_depth_effect;
```

In this case, can just replace the "1" with `eSpec_dep_water`.

## Modern platform code
If you need to add new code to interface with modern platforms or cross-platform (for example, audio, rendering, get system time), please add this code into `src/harness`. `harness` contains only new code written by the dethrace project, and its goal is to provide a simple cross-platform interface to `BRSRC13` and `DETHRACE`. We want to keep the code in `BRSRC13` and `DETHRACE` as faithful to the original as possible, and not be polluted with extra modern code or dependencies. Instead, that code goes into `harness`.

Why is it called `harness`? Good question! It contains the _real_ `main` function, so harness starts up first, reads the command line, configures a few things, then calls into the _original_ main function in `src/DETHRACE`. The original game calls harness functions for platform services like audio and display. In this way, it acts like a harness for the original game engine.

## Language
This is a C project. No C++ please.

## Unit tests

Tests were written to cover some basic functionality at the start of the project. There is very little coverage, but tests must pass before we allow code to be merged.

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
