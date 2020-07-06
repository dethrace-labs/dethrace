// Based on https://gist.github.com/jvranish/4441299

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <imagehlp.h>
#include <windows.h>
#else
#include <err.h>
#include <execinfo.h>
#endif

static int stack_nbr = 0;
static char _program_name[1024];

/* Resolve symbol name and source location given the path to the executable 
   and an address */
int addr2line(char const* const program_name, void const* const addr) {
    char addr2line_cmd[512] = { 0 };

/* have addr2line map the address to the relent line in the code */
#ifdef __APPLE__
    /* apple does things differently... */
    sprintf(addr2line_cmd, "atos -o %.256s %p", program_name, addr);
#else
    sprintf(addr2line_cmd, "addr2line -f -p -e %.256s %p", program_name, addr);
#endif

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

#ifdef _WIN32
void windows_print_stacktrace(CONTEXT* context) {
    SymInitialize(GetCurrentProcess(), 0, true);

    STACKFRAME frame = { 0 };

    /* setup initial stack frame */
    frame.AddrPC.Offset = context->Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;

    while (StackWalk(IMAGE_FILE_MACHINE_I386,
        GetCurrentProcess(),
        GetCurrentThread(),
        &frame,
        context,
        0,
        SymFunctionTableAccess,
        SymGetModuleBase,
        0)) {
        addr2line(_program_name, (void*)frame.AddrPC.Offset);
    }

    SymCleanup(GetCurrentProcess());
}

LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* ExceptionInfo) {
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
        windows_print_stacktrace(ExceptionInfo->ContextRecord);
    } else {
        addr2line(_program_name, (void*)ExceptionInfo->ContextRecord->Eip);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

void install_signal_handler(char* program_name) {
    strcpy(_program_name, program_name);
    SetUnhandledExceptionFilter(windows_exception_handler);
}
#else

#define MAX_STACK_FRAMES 64
static void* stack_traces[MAX_STACK_FRAMES];

void posix_print_stack_trace() {
    int i, trace_size = 0;
    char** messages = (char**)NULL;

    fputs("\nStack trace:\n", stderr);

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    messages = backtrace_symbols(stack_traces, trace_size);

    /* skip the first couple stack frames (as they are this function and
     our handler) and also skip the last frame as it's (always?) junk. */
    for (i = 3; i < (trace_size - 1); ++i) {
        if (addr2line(_program_name, stack_traces[i]) != 0) {
            printf("  error determining line # for: %s\n", messages[i]);
        }
    }
    if (messages) {
        free(messages);
    }
}

void posix_signal_handler(int sig, siginfo_t* siginfo, void* context) {
    (void)context;
    fputs("\n******************\n", stderr);

    switch (sig) {
    case SIGSEGV:
        fputs("Caught SIGSEGV\n", stderr);
        break;
    case SIGINT:
        fputs("Caught SIGINT\n", stderr);
        break;
    case SIGFPE:
        switch (siginfo->si_code) {
        case FPE_INTDIV:
            fputs("Caught SIGFPE: FPE_INTDIV\n", stderr);
            break;
        case FPE_INTOVF:
            fputs("Caught SIGFPE: FPE_INTOVF\n", stderr);
            break;
        case FPE_FLTDIV:
            fputs("Caught SIGFPE: FPE_FLTDIV\n", stderr);
            break;
        case FPE_FLTOVF:
            fputs("Caught SIGFPE: FPE_FLTOVF\n", stderr);
            break;
        case FPE_FLTUND:
            fputs("Caught SIGFPE: FPE_FLTUND\n", stderr);
            break;
        case FPE_FLTRES:
            fputs("Caught SIGFPE: FPE_FLTRES\n", stderr);
            break;
        case FPE_FLTINV:
            fputs("Caught SIGFPE: FPE_FLTINV\n", stderr);
            break;
        case FPE_FLTSUB:
            fputs("Caught SIGFPE: FPE_FLTSUB\n", stderr);
            break;
        default:
            fputs("Caught SIGFPE: Arithmetic Exception\n", stderr);
            break;
        }
    case SIGILL:
        switch (siginfo->si_code) {
        case ILL_ILLOPC:
            fputs("Caught SIGILL: ILL_ILLOPC\n", stderr);
            break;
        case ILL_ILLOPN:
            fputs("Caught SIGILL: ILL_ILLOPN\n", stderr);
            break;
        case ILL_ILLADR:
            fputs("Caught SIGILL: ILL_ILLADR\n", stderr);
            break;
        case ILL_ILLTRP:
            fputs("Caught SIGILL: ILL_ILLTRP\n", stderr);
            break;
        case ILL_PRVOPC:
            fputs("Caught SIGILL: ILL_PRVOPC\n", stderr);
            break;
        case ILL_PRVREG:
            fputs("Caught SIGILL: ILL_PRVREG\n", stderr);
            break;
        case ILL_COPROC:
            fputs("Caught SIGILL: ILL_COPROC\n", stderr);
            break;
        case ILL_BADSTK:
            fputs("Caught SIGILL: ILL_BADSTK\n", stderr);
            break;
        default:
            fputs("Caught SIGILL: Illegal Instruction\n", stderr);
            break;
        }
        break;
    case SIGTERM:
        fputs("Caught SIGTERM\n", stderr);
        break;
    case SIGABRT:
        fputs("Caught SIGABRT\n", stderr);
        break;
    default:
        break;
    }
    fputs("******************\n", stderr);
    posix_print_stack_trace();
    exit(1);
}

static uint8_t alternate_stack[SIGSTKSZ];
void install_signal_handler(char* program_name) {

    strcpy(_program_name, program_name);

    /* setup alternate stack */
    {
        stack_t ss = {};
        /* malloc is usually used here, I'm not 100% sure my static allocation
         is valid but it seems to work just fine. */
        ss.ss_sp = (void*)alternate_stack;
        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;

        if (sigaltstack(&ss, NULL) != 0) {
            err(1, "sigaltstack");
        }
    }

    /* register our signal handlers */
    {
        struct sigaction sig_action = {};
        sig_action.sa_sigaction = posix_signal_handler;
        sigemptyset(&sig_action.sa_mask);

#ifdef __APPLE__
        /* for some reason we backtrace() doesn't work on osx
             when we use an alternate stack */
        sig_action.sa_flags = SA_SIGINFO;
#else
        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif

        if (sigaction(SIGSEGV, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGFPE, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGINT, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGILL, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGTERM, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGABRT, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
    }
}
#endif