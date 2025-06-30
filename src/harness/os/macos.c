// Based on https://gist.github.com/jvranish/4441299

#include "harness/config.h"
#include "harness/os.h"
#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <inttypes.h>
#include <libgen.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <netdb.h> // for getaddrinfo() and freeaddrinfo()
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h> // for close()
#include <unistd.h>
#include <SDL.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

static int stack_nbr = 0;
static char _program_name[1024];
#define MAX_STACK_FRAMES 64
static void* stack_traces[MAX_STACK_FRAMES];
static char name_buf[4096];
static DIR* directory_iterator;

// Resolve symbol name and source location given the path to the executable and an address
int addr2line(char const* const program_name, intptr_t slide, void const* const addr) {
    char addr2line_cmd[512] = { 0 };

    /* have addr2line map the address to the related line in the code */
    sprintf(addr2line_cmd, "atos -s %" PRIxPTR " -o %.256s %p", slide, program_name, addr);

    // printf("addr2line command: %s\n", addr2line_cmd);

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

static void print_stack_trace(void) {
    int i, trace_size = 0;
    char** messages = (char**)NULL;
    intptr_t slide;

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    messages = backtrace_symbols(stack_traces, trace_size);
    slide = _dyld_get_image_vmaddr_slide(0);

    fputs("\nStack trace:\n", stderr);

    /* skip the first couple stack frames (as they are this function and
     our handler) and also skip the last frame as it's (always?) junk. */
    for (i = 3; i < (trace_size - 1); i++) {

        // seem to get a duplicate frame right where the error happens
        if (stack_traces[i] == stack_traces[i - 1]) {
            continue;
        }
        if (addr2line(_program_name, slide, stack_traces[i]) != 0) {
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

FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f;

    f = fopen(pathname, mode);
    if (harness_game_config.verbose) {
        if (f == NULL) {
            fprintf(stderr, "Failed to open \"%s\" (%s)\n", pathname, strerror(errno));
        }
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
    strcpy(name_buf, path);
    return dirname(name_buf);
}

char* OS_Basename(const char* path) {
    strcpy(name_buf, path);
    return basename(name_buf);
}

char* OS_GetWorkingDirectory(char* argv0) {
    // Looks for the data in "~/Library/Application Support/dethrace"
    return SDL_GetPrefPath(NULL, "dethrace");
}

int OS_GetAdapterAddress(char* name, void* pSockaddr_in) {
    struct ifaddrs *ifaddr, *ifa;
    int found = 0;

    // Get the list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 0;
    }

    // Loop through all interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (strlen(name) == 0 || strcmp(name, ifa->ifa_name) == 0) {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                *((struct sockaddr_in*)pSockaddr_in) = *(struct sockaddr_in*)ifa->ifa_addr;
                found = 1;
                break;
            }
        }
    }

    // Free the memory allocated by getifaddrs
    freeifaddrs(ifaddr);
    return found;
}

int OS_InitSockets(void) {
    return 0;
}

int OS_GetLastSocketError(void) {
    return errno;
}

void OS_CleanupSockets(void) {
    // no-op
}

int OS_SetSocketNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(socket, F_SETFL, flags);
}

int OS_CloseSocket(int socket) {
    return close(socket);
}
