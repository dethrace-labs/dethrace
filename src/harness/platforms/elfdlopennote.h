#ifndef DLOPENNOTE_H
#define DLOPENNOTE_H

#define ELF_NOTE_DLOPEN_PRIORITY_SUGGESTED   "suggested"
#define ELF_NOTE_DLOPEN_PRIORITY_RECOMMENDED "recommended"
#define ELF_NOTE_DLOPEN_PRIORITY_REQUIRED    "required"

#if !defined(__unix__) || defined(__ANDROID__)
#ifndef DISABLE_DLOPEN_NOTES
#define DISABLE_DLOPEN_NOTES
#endif
#elif defined(__GNUC__) && (__GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 1))
/* gcc < 3.1 too old */
#ifndef DISABLE_DLOPEN_NOTES
#define DISABLE_DLOPEN_NOTES
#endif
#endif

#if defined(__ELF__) && !defined(DISABLE_DLOPEN_NOTES)

#define ELF_NOTE_DLOPEN_VENDOR "FDO"
#define ELF_NOTE_DLOPEN_TYPE 0x407c0c0aU

#define ELF_NOTE_INTERNAL2(json, variable_name)                 \
    __attribute__((aligned(4), used, section(".note.dlopen")))      \
    static const struct {                                           \
        struct {                                                    \
            Uint32 n_namesz;                                        \
            Uint32 n_descsz;                                        \
            Uint32 n_type;                                          \
        } nhdr;                                                     \
        char name[4];                                               \
        __attribute__((aligned(4))) char dlopen_json[sizeof(json)]; \
    } variable_name = {                                             \
        {                                                           \
             sizeof(ELF_NOTE_DLOPEN_VENDOR),                    \
             sizeof(json),                                          \
             ELF_NOTE_DLOPEN_TYPE                               \
        },                                                          \
        ELF_NOTE_DLOPEN_VENDOR,                                 \
        json                                                        \
    }

#define ELF_NOTE_INTERNAL(json, variable_name) \
    ELF_NOTE_INTERNAL2(json, variable_name)

#define SONAME_ARRAY1(N1) "[\"" N1 "\"]"
#define SONAME_ARRAY2(N1,N2) "[\"" N1 "\",\"" N2 "\"]"
#define SONAME_ARRAY3(N1,N2,N3) "[\"" N1 "\",\"" N2 "\",\"" N3 "\"]"
#define SONAME_ARRAY4(N1,N2,N3,N4) "[\"" N1 "\",\"" N2 "\",\"" N3 "\",\"" N4 "\"]"
#define SONAME_ARRAY5(N1,N2,N3,N4,N5) "[\"" N1 "\",\"" N2 "\",\"" N3 "\",\"" N4 "\",\"" N5 "\"]"
#define SONAME_ARRAY6(N1,N2,N3,N4,N5,N6) "[\"" N1 "\",\"" N2 "\",\"" N3 "\",\"" N4 "\",\"" N5 "\",\"" N6 "\"]"
#define SONAME_ARRAY7(N1,N2,N3,N4,N5,N6,N7) "[\"" N1 "\",\"" N2 "\",\"" N3 "\",\"" N4 "\",\"" N5 "\",\"" N6 "\",\"" N7 "\"]"
#define SONAME_ARRAY8(N1,N2,N3,N4,N5,N6,N7,N8) "[\"" N1 "\",\"" N2 "\",\"" N3 "\",\"" N4 "\",\"" N5 "\",\"" N6 "\",\"" N7 "\",\"" N8 "\"]"
#define SONAME_ARRAY_GET(N1,N2,N3,N4,N5,N6,N7,N8,NAME,...) NAME
#define SONAME_ARRAY(...) \
    SONAME_ARRAY_GET(__VA_ARGS__, \
         SONAME_ARRAY8, \
         SONAME_ARRAY7, \
         SONAME_ARRAY6, \
         SONAME_ARRAY5, \
         SONAME_ARRAY4, \
         SONAME_ARRAY3, \
         SONAME_ARRAY2, \
         SONAME_ARRAY1 \
    )(__VA_ARGS__)

#define ELF_NOTE_JOIN2(A,B) A##B
#define ELF_NOTE_JOIN(A,B) ELF_NOTE_JOIN2(A,B)
#define ELF_NOTE_UNIQUE_NAME ELF_NOTE_JOIN(s_dlopen_note_, __LINE__)

#define ELF_NOTE_DLOPEN(feature, description, priority, ...) \
    ELF_NOTE_INTERNAL(                                       \
        "[{\"feature\":\"" feature                               \
        "\",\"description\":\"" description                      \
        "\",\"priority\":\"" priority                            \
        "\",\"soname\":" SONAME_ARRAY(__VA_ARGS__) "}]",     \
        ELF_NOTE_UNIQUE_NAME)

#elif defined(__GNUC__) && __GNUC__ < 3

#define ELF_NOTE_DLOPEN(args...)

#elif defined(_MSC_VER) && _MSC_VER < 1400

#else

#define ELF_NOTE_DLOPEN(...)

#endif

#endif /* DLOPENNOTE_H */
