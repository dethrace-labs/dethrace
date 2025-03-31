#ifndef sdl_dyn_common_h
#define sdl_dyn_common_h

#ifdef DETHRACE_SDL_DYNAMIC
#ifdef _WIN32
#include <windows.h>
#ifdef CreateWindow
#undef CreateWindow
#endif
static void *Harness_LoadObject(const char *name) {
    return LoadLibraryA(name);
}
static void Harness_UnloadObject(void *obj) {
    FreeLibrary(obj);
}
static void *Harness_LoadFunction(void *obj, const char *name) {
    return GetProcAddress(obj, name);
}
#else
#include <dlfcn.h>
static void *Harness_LoadObject(const char *name) {
    return dlopen(name, RTLD_NOW | RTLD_LOCAL);
}
static void Harness_UnloadObject(void *obj) {
    dlclose(obj);
}
static void *Harness_LoadFunction(void *obj, const char *name) {
    return dlsym(obj, name);
}
#endif
#endif

#define STR2_JOIN(A,B) A##B
#define STR_JOIN(A,B) STR2_JOIN(A, B)

#define X_TYPEDEF(name, ret, args) typedef ret SDLCALL t##SDL_##name##_fn args;
#define X_STATIC_SYMBOL(name, ret, args) static t##SDL_##name##_fn* STR_JOIN(SYMBOL_PREFIX, name);
#ifdef DETHRACE_SDL_DYNAMIC
#define X_LOAD_FUNCTION(name, ret, args) \
    STR_JOIN(SYMBOL_PREFIX, name) = Harness_LoadFunction(OBJECT_NAME, "SDL_" #name); \
    if (STR_JOIN(SYMBOL_PREFIX, name) == NULL) { \
        goto failure; \
    }
#else
#define X_LOAD_FUNCTION(name, ret, args) STR_JOIN(SYMBOL_PREFIX, name) = SDL_##name;
#endif

FOREACH_SDLX_SYM(X_TYPEDEF)
FOREACH_SDLX_SYM(X_STATIC_SYMBOL)

static int STR_JOIN(SYMBOL_PREFIX,LoadSymbols)(void) {
#ifdef DETHRACE_SDL_DYNAMIC
    for (size_t i = 0; i < BR_ASIZE(possible_locations); i++) {
        OBJECT_NAME = Harness_LoadObject(possible_locations[i]);
        if (OBJECT_NAME != NULL) {
            break;
        }
    }
    if (OBJECT_NAME == NULL) {
        return 1;
    }
#endif
    FOREACH_SDLX_SYM(X_LOAD_FUNCTION)
    return 0;
#ifdef DETHRACE_SDL_DYNAMIC
failure:
    Harness_UnloadObject(OBJECT_NAME);
    return 1;
#endif
}

#endif /* sdl_dyn_common_h */
