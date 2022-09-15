#ifndef HARNESS_COMPILER_H
#define HARNESS_COMPILER_H

#if defined(_MSC_VER)
#define HARNESS_NORETURN __declspec(noreturn)
#else
#define HARNESS_NORETURN __attribute__((noreturn))
#endif

#endif
