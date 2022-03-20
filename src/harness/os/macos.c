// Based on https://gist.github.com/jvranish/4441299

#include "harness/os.h"
#include <assert.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <execinfo.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static int stack_nbr = 0;
static char _program_name[1024];
#define MAX_STACK_FRAMES 64
static void* stack_traces[MAX_STACK_FRAMES];
DIR* directory_iterator;

uint32_t OS_GetTime() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
}

void OS_Sleep(int delay_ms) {
    struct timespec ts;
    ts.tv_sec = delay_ms / 1000;
    ts.tv_nsec = (delay_ms % 1000) * 1000000;
    nanosleep(&ts, &ts);
}

char* OS_GetFirstFileInDirectory(char* path) {
    directory_iterator = opendir(path);
    if (directory_iterator == NULL) {
        return NULL;
    }
    return OS_GetNextFileInDirectory();
}

char* OS_GetNextFileInDirectory(void) {
    struct dirent* entry;

    if (directory_iterator == NULL) {
        return NULL;
    }
    while ((entry = readdir(directory_iterator)) != NULL) {
        if (entry->d_type == DT_REG) {
            return entry->d_name;
        }
    }
    closedir(directory_iterator);
    directory_iterator = NULL;
    return NULL;
}

void OS_Basename(char* path, char* base) {
    strcpy(base, basename(path));
}

// https://developer.apple.com/library/archive/qa/qa1361/_index.html
// FIXME:
//     Important: Because the definition of the kinfo_proc structure (in <sys/sysctl.h>) is conditionalized by __APPLE_API_UNSTABLE,
//     you should restrict use of the above code to the debug build of your program.

int OS_IsDebuggerPresent()
// Returns true if the current process is being debugged (either
// running under the debugger or has a debugger attached post facto).
{
    int junk;
    int mib[4];
    struct kinfo_proc info;
    size_t size;

    // Initialize the flags so that, if sysctl fails for some bizarre
    // reason, we get a predictable result.

    info.kp_proc.p_flag = 0;

    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    // Call sysctl.

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    assert(junk == 0);

    // We're being debugged if the P_TRACED flag is set.

    return ((info.kp_proc.p_flag & P_TRACED) != 0);
}

// Resolve symbol name and source location given the path to the executable and an address
int addr2line(char const* const program_name, void const* const addr) {
    char addr2line_cmd[512] = { 0 };

    /* have addr2line map the address to the related line in the code */
    sprintf(addr2line_cmd, "atos -o %.256s %p", program_name, addr);

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

void print_stack_trace() {
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

void signal_handler(int sig, siginfo_t* siginfo, void* context) {
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
        break;
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
    print_stack_trace();
    exit(1);
}

static uint8_t alternate_stack[SIGSTKSZ];

void resolve_full_path(char* path, const char* argv0) {
    if (argv0[0] == '/') { // run with absolute path
        strcpy(path, argv0);
    } else { // run with relative path
        if (NULL == getcwd(path, PATH_MAX)) {
            perror("getcwd error");
            return;
        }
        strcat(path, "/");
        strcat(path, argv0);
    }
}

void OS_InstallSignalHandler(char* program_name) {

    resolve_full_path(_program_name, program_name);

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
        sig_action.sa_sigaction = signal_handler;
        sigemptyset(&sig_action.sa_mask);

        sig_action.sa_flags = SA_SIGINFO;

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
