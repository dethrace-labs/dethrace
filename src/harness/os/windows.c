// Based on https://gist.github.com/jvranish/4441299

// this has to be first
#include <windows.h>

#include <imagehlp.h>

#include "harness/os.h"
#include "harness/trace.h"

#include <assert.h>
#include <direct.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN64) || defined(_M_ARM64)
#define Esp Rsp
#define Eip Rip
#define Ebp Rbp
#endif

void dr_dprintf(char* fmt_string, ...);

static int stack_nbr = 0;
static char windows_program_name[1024];

static char dirname_buf[_MAX_DIR];
static char fname_buf[_MAX_FNAME];

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) ||defined( __i386) || defined(_M_IX86)
#define DETHRACE_CPU_X86 1
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define DETHRACE_CPU_X64 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#define DETHRACE_CPU_ARM64 1
#endif

#if defined(DETHRACE_CPU_X86) || defined(DETHRACE_CPU_X64) || defined(DETHRACE_CPU_ARM64)
#define DETHRACE_STACKWALK 1
#else
#pragma message("Unsupported architecture: don't know how to StackWalk")
#endif

#ifdef DETHRACE_STACKWALK
static int addr2line(char const* const program_name, void const* const addr) {
    char addr2line_cmd[512] = { 0 };

    sprintf(addr2line_cmd, "addr2line -f -p -e %.256s %p", program_name, addr);

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

static void print_stacktrace(CONTEXT* context) {

    SymInitialize(GetCurrentProcess(), 0, true);

    STACKFRAME frame = { 0 };

    /* setup initial stack frame */
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
#if defined(DETHRACE_CPU_X86) || defined(DETHRACE_CPU_X64)
#if defined(DETHRACE_CPU_X86)
    DWORD machine_type = IMAGE_FILE_MACHINE_I386;
#else
    DWORD machine_type = IMAGE_FILE_MACHINE_AMD64;
#endif
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrPC.Offset = context->Eip;
#elif defined(DETHRACE_CPU_ARM64)
    DWORD machine_type = IMAGE_FILE_MACHINE_ARM64;
    frame.AddrFrame.Offset = context->Fp;
    frame.AddrStack.Offset = context->Sp;
    frame.AddrPC.Offset = context->Pc;
#endif

    while (StackWalk(machine_type,
        GetCurrentProcess(),
        GetCurrentThread(),
        &frame,
        context,
        0,
        SymFunctionTableAccess,
        SymGetModuleBase,
        0)) {
        addr2line(windows_program_name, (void*)frame.AddrPC.Offset);
    }

    SymCleanup(GetCurrentProcess());
}

static LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* ExceptionInfo) {
    switch (ExceptionInfo->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        fputs("Error: EXCEPTION_ACCESS_VIOLATION\n", stderr);
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        fputs("Error: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n", stderr);
        break;
    case EXCEPTION_BREAKPOINT:
        fputs("Error: EXCEPTION_BREAKPOINT\n", stderr);
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        fputs("Error: EXCEPTION_DATATYPE_MISALIGNMENT\n", stderr);
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        fputs("Error: EXCEPTION_FLT_DENORMAL_OPERAND\n", stderr);
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        fputs("Error: EXCEPTION_FLT_DIVIDE_BY_ZERO\n", stderr);
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        fputs("Error: EXCEPTION_FLT_INEXACT_RESULT\n", stderr);
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        fputs("Error: EXCEPTION_FLT_INVALID_OPERATION\n", stderr);
        break;
    case EXCEPTION_FLT_OVERFLOW:
        fputs("Error: EXCEPTION_FLT_OVERFLOW\n", stderr);
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        fputs("Error: EXCEPTION_FLT_STACK_CHECK\n", stderr);
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        fputs("Error: EXCEPTION_FLT_UNDERFLOW\n", stderr);
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        fputs("Error: EXCEPTION_ILLEGAL_INSTRUCTION\n", stderr);
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        fputs("Error: EXCEPTION_IN_PAGE_ERROR\n", stderr);
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        fputs("Error: EXCEPTION_INT_DIVIDE_BY_ZERO\n", stderr);
        break;
    case EXCEPTION_INT_OVERFLOW:
        fputs("Error: EXCEPTION_INT_OVERFLOW\n", stderr);
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        fputs("Error: EXCEPTION_INVALID_DISPOSITION\n", stderr);
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        fputs("Error: EXCEPTION_NONCONTINUABLE_EXCEPTION\n", stderr);
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        fputs("Error: EXCEPTION_PRIV_INSTRUCTION\n", stderr);
        break;
    case EXCEPTION_SINGLE_STEP:
        fputs("Error: EXCEPTION_SINGLE_STEP\n", stderr);
        break;
    case EXCEPTION_STACK_OVERFLOW:
        fputs("Error: EXCEPTION_STACK_OVERFLOW\n", stderr);
        break;
    default:
        fputs("Error: Unrecognized Exception\n", stderr);
        break;
    }
    fflush(stderr);
    /* If this is a stack overflow then we can't walk the stack, so just show
      where the error happened */
    if (EXCEPTION_STACK_OVERFLOW != ExceptionInfo->ExceptionRecord->ExceptionCode) {
        print_stacktrace(ExceptionInfo->ContextRecord);
    } else {
#if defined(DETHRACE_CPU_X86) || defined(DETHRACE_CPU_X64)
        void *addr = (void*)ExceptionInfo->ContextRecord->Eip;
#elif defined(DETHRACE_CPU_ARM64)
        void *addr = (void*)ExceptionInfo->ContextRecord->Pc;
#endif
        addr2line(windows_program_name, addr);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void OS_InstallSignalHandler(char* program_name) {
#ifdef DETHRACE_STACKWALK
    strcpy(windows_program_name, program_name);
    SetUnhandledExceptionFilter(windows_exception_handler);
#else
    LOG_WARN("Unsupported architecture. No signal handlers installed");
#endif
}

FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f;
    errno_t err;

    f = NULL;
    err = fopen_s(&f, pathname, mode);
    if (err != 0) {
        fprintf(stderr, "Failed to open \"%s\" (%s)\n", pathname, strerror(err));
    }

    return f;
}

size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen) {
    // FIXME: unsafe implementation (echos the password)
    pBuffer[0] = '\0';
    fgets(pBuffer, pBufferLen, stdin);
    return strlen(pBuffer);
}

char* OS_Dirname(const char* path) {
    _splitpath(path, NULL, dirname_buf, NULL, NULL);
    return dirname_buf;
}

char* OS_Basename(const char* path) {
    _splitpath(path, NULL, NULL, fname_buf, NULL);
    return fname_buf;
}
