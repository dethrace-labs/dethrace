include(CheckIncludeFile)

# Check if we are missing any posix headers we consider required.
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(dirent.h HAVE_DIRENT_H)
check_include_file(fnmatch.h HAVE_FNMATCH_H)
check_include_file(getopt.h HAVE_GETOPT_H)
check_include_file(unistd.h HAVE_UNISTD_H)

if(HAVE_STRINGS_H AND HAVE_UNISTD_H AND HAVE_LIBGEN_H AND HAVE_DIRENT_H AND HAVE_FNMATCH_H AND HAVE_GETOPT_H)
    add_library(miniposix INTERFACE)
else()
    add_library(miniposix STATIC)

    # Only win32 targets should really need miniposix at all, but just to be safe.
    if(WIN32)
        target_compile_definitions(miniposix PUBLIC _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE WIN32_LEAN_AND_MEAN)
    endif()
endif()

if(NOT HAVE_STRINGS_H)
    target_sources(miniposix PRIVATE strings/strings.c strings/strings.h)
    target_include_directories(miniposix PUBLIC strings)
endif()

if(NOT HAVE_LIBGEN_H)
    target_sources(miniposix PRIVATE libgen/libgen.c libgen/libgen.h)
    target_include_directories(miniposix PUBLIC libgen)
endif()

if(NOT HAVE_DIRENT_H)
    target_sources(miniposix PRIVATE dirent/dirent.c dirent/dirent.h)
    target_include_directories(miniposix PUBLIC dirent)
endif()

if(NOT HAVE_FNMATCH_H)
    target_sources(miniposix PRIVATE fnmatch/fnmatch.c fnmatch/fnmatch.h)
    target_include_directories(miniposix PUBLIC fnmatch)
endif()

if(NOT HAVE_GETOPT_H)
    target_sources(miniposix PRIVATE getopt/getopt.c getopt/getopt.h)
    target_include_directories(miniposix PUBLIC getopt)
endif()

if(NOT HAVE_UNISTD_H)
    target_sources(miniposix PRIVATE unistd/unistd.h)
    target_include_directories(miniposix PUBLIC unistd)
endif()
