#include <dirent.h>
#include <stdlib.h>
#include <windows.h>
#include <winioctl.h>

struct __dir
{
    struct dirent *entries;
    HANDLE fd;
    long int count;
    long int index;
};

static void __seterrno(int value)
{
#ifdef _MSC_VER
    _set_errno(value);
#else /* _MSC_VER */
    errno = value;
#endif /* _MSC_VER */
}

int closedir(DIR *dirp)
{
    struct __dir *data = NULL;
    if (!dirp) {
        __seterrno(EBADF);
        return -1;
    }
    data = (struct __dir *)dirp;
    CloseHandle((HANDLE)data->fd);
    free(data->entries);
    free(data);
    return 0;
}

static int __islink(const wchar_t *name, char *buffer)
{
    DWORD io_result = 0;
    DWORD bytes_returned = 0;
    HANDLE hFile =
        CreateFileW(name, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return 0;

    io_result = DeviceIoControl(
        hFile, FSCTL_GET_REPARSE_POINT, NULL, 0, buffer, MAXIMUM_REPARSE_DATA_BUFFER_SIZE, &bytes_returned, NULL);

    CloseHandle(hFile);

    if (io_result == 0)
        return 0;

    return ((REPARSE_GUID_DATA_BUFFER *)buffer)->ReparseTag == IO_REPARSE_TAG_SYMLINK;
}

static __ino_t __inode(const wchar_t *name)
{
    __ino_t value = { 0 };
    BOOL result;
    FILE_ID_INFO fileid;
    BY_HANDLE_FILE_INFORMATION info;
    HANDLE hFile = CreateFileW(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return value;

    result = GetFileInformationByHandleEx(hFile, FileIdInfo, &fileid, sizeof(fileid));
    if (result) {
        value.serial = fileid.VolumeSerialNumber;
        memcpy(value.fileid, fileid.FileId.Identifier, 16);
    } else {
        result = GetFileInformationByHandle(hFile, &info);
        if (result) {
            value.serial = info.dwVolumeSerialNumber;
            memcpy(value.fileid + 8, &info.nFileIndexHigh, 4);
            memcpy(value.fileid + 12, &info.nFileIndexLow, 4);
        }
    }
    CloseHandle(hFile);
    return value;
}

static DIR *__internal_opendir(wchar_t *wname, int size)
{
    struct __dir *data = NULL;
    struct dirent *tmp_entries = NULL;
    static char default_char = '?';
    static wchar_t *prefix = L"\\\\?\\";
    static wchar_t *suffix = L"\\*.*";
    int extra_prefix = 4; /* use prefix "\\?\" to handle long file names */
    static int extra_suffix = 4; /* use suffix "\*.*" to find everything */
    WIN32_FIND_DATAW w32fd = { 0 };
    HANDLE hFindFile = INVALID_HANDLE_VALUE;
    static int grow_factor = 2;
    char *buffer = NULL;

    /* Ensure path only uses windows separator, of FindFirstFileW will fail. */
    wchar_t* rep = wname;
    while ((rep = wcschr(rep, L'/')) != NULL) {
        *rep++ = L'\\';
    }

    memcpy(wname + extra_prefix + size - 1, suffix, sizeof(wchar_t) * extra_prefix);
    wname[size + extra_prefix + extra_suffix - 1] = 0;

    if (memcmp(wname + extra_prefix, L"\\\\?\\", sizeof(wchar_t) * extra_prefix) == 0) {
        wname += extra_prefix;
        extra_prefix = 0;
    }

    hFindFile = FindFirstFileW(wname, &w32fd);
    if (INVALID_HANDLE_VALUE == hFindFile) {
        __seterrno(ENOENT);
        return NULL;
    }

    data = (struct __dir *)malloc(sizeof(struct __dir));
    if (!data)
        goto out_of_memory;
    wname[extra_prefix + size - 1] = 0;
    data->fd = CreateFileW(wname, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
    wname[extra_prefix + size - 1] = L'\\';
    data->count = 16;
    data->index = 0;
    data->entries = (struct dirent *)malloc(sizeof(struct dirent) * data->count);
    if (!data->entries)
        goto out_of_memory;
    buffer = malloc(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
    if (!buffer)
        goto out_of_memory;
    do {
        WideCharToMultiByte(
            CP_UTF8, 0, w32fd.cFileName, -1, data->entries[data->index].d_name, NAME_MAX, &default_char, NULL);

        memcpy(wname + extra_prefix + size, w32fd.cFileName, sizeof(wchar_t) * NAME_MAX);

        if (((w32fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT)
            && __islink(wname, buffer))
            data->entries[data->index].d_type = DT_LNK;
        else if ((w32fd.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) == FILE_ATTRIBUTE_DEVICE)
            data->entries[data->index].d_type = DT_CHR;
        else if ((w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
            data->entries[data->index].d_type = DT_DIR;
        else
            data->entries[data->index].d_type = DT_REG;

        data->entries[data->index].d_ino = __inode(wname);
        data->entries[data->index].d_reclen = sizeof(struct dirent);
        data->entries[data->index].d_namelen = (unsigned char)wcslen(w32fd.cFileName);
        data->entries[data->index].d_off = 0;

        if (++data->index == data->count) {
            tmp_entries = (struct dirent *)realloc(data->entries, sizeof(struct dirent) * data->count * grow_factor);
            if (!tmp_entries)
                goto out_of_memory;
            data->entries = tmp_entries;
            data->count *= grow_factor;
        }
    } while (FindNextFileW(hFindFile, &w32fd) != 0);

    free(buffer);
    FindClose(hFindFile);

    data->count = data->index;
    data->index = 0;
    return (DIR *)data;
out_of_memory:
    if (data) {
        if (INVALID_HANDLE_VALUE != (HANDLE)data->fd)
            CloseHandle((HANDLE)data->fd);
        free(data->entries);
    }
    free(buffer);
    free(data);
    if (INVALID_HANDLE_VALUE != hFindFile)
        FindClose(hFindFile);
    __seterrno(ENOMEM);
    return NULL;
}

static wchar_t *__get_buffer()
{
    wchar_t *name = malloc(sizeof(wchar_t) * (NTFS_MAX_PATH + NAME_MAX + 8));
    if (name)
        memcpy(name, L"\\\\?\\", sizeof(wchar_t) * 4);
    return name;
}

DIR *opendir(const char *name)
{
    DIR *dirp = NULL;
    wchar_t *wname = __get_buffer();
    int size = 0;
    if (!wname) {
        __seterrno(ENOMEM);
        return NULL;
    }
    size = MultiByteToWideChar(CP_UTF8, 0, name, -1, wname + 4, NTFS_MAX_PATH);
    if (0 == size) {
        free(wname);
        return NULL;
    }
    dirp = __internal_opendir(wname, size);
    free(wname);
    return dirp;
}

DIR *_wopendir(const wchar_t *name)
{
    DIR *dirp = NULL;
    wchar_t *wname = __get_buffer();
    int size = 0;
    if (!wname) {
        __seterrno(ENOMEM);
        return NULL;
    }
    size = (int)wcslen(name);
    if (size > NTFS_MAX_PATH) {
        free(wname);
        return NULL;
    }
    memcpy(wname + 4, name, sizeof(wchar_t) * (size + 1));
    dirp = __internal_opendir(wname, size + 1);
    free(wname);
    return dirp;
}

DIR *fdopendir(int fd)
{
    DIR *dirp = NULL;
    wchar_t *wname = __get_buffer();
    int size = 0;
    if (!wname) {
        __seterrno(ENOMEM);
        return NULL;
    }
    size = GetFinalPathNameByHandleW((HANDLE)((intptr_t)fd), wname, NTFS_MAX_PATH, FILE_NAME_NORMALIZED);
    if (0 == size) {
        free(wname);
        __seterrno(ENOTDIR);
        return NULL;
    }
    dirp = __internal_opendir(wname, size + 1);
    free(wname);
    return dirp;
}

struct dirent *readdir(DIR *dirp)
{
    struct __dir *data = (struct __dir *)dirp;
    if (!data) {
        __seterrno(EBADF);
        return NULL;
    }
    if (data->index < data->count) {
        return &data->entries[data->index++];
    }
    return NULL;
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    struct __dir *data = (struct __dir *)dirp;
    if (!data) {
        return EBADF;
    }
    if (data->index < data->count) {
        if (entry)
            memcpy(entry, &data->entries[data->index++], sizeof(struct dirent));
        if (result)
            *result = entry;
    } else if (result)
        *result = NULL;
    return 0;
}

void seekdir(DIR *dirp, long int offset)
{
    if (dirp) {
        struct __dir *data = (struct __dir *)dirp;
        data->index = (offset < data->count) ? offset : data->index;
    }
}

void rewinddir(DIR *dirp)
{
    seekdir(dirp, 0);
}

long int telldir(DIR *dirp)
{
    if (!dirp) {
        __seterrno(EBADF);
        return -1;
    }
    return ((struct __dir *)dirp)->count;
}

int dirfd(DIR *dirp)
{
    if (!dirp) {
        __seterrno(EINVAL);
        return -1;
    }
    return (int)((struct __dir *)dirp)->fd;
}

int scandir(const char *dirp,
    struct dirent ***namelist,
    int (*filter)(const struct dirent *),
    int (*compar)(const struct dirent **, const struct dirent **))
{
    struct dirent **entries = NULL, **tmp_entries = NULL;
    long int i = 0, index = 0, count = 16;
    DIR *d = opendir(dirp);
    struct __dir *data = (struct __dir *)d;
    if (!data) {
        closedir(d);
        __seterrno(ENOENT);
        return -1;
    }
    entries = (struct dirent **)malloc(sizeof(struct dirent *) * count);
    if (!entries) {
        closedir(d);
        __seterrno(ENOMEM);
        return -1;
    }
    for (i = 0; i < data->count; ++i) {
        if (!filter || filter(&data->entries[i])) {
            entries[index] = (struct dirent *)malloc(sizeof(struct dirent));
            if (!entries[index]) {
                closedir(d);
                for (i = 0; i < index; ++i)
                    free(entries[index]);
                free(entries);
                __seterrno(ENOMEM);
                return -1;
            }
            memcpy(entries[index], &data->entries[i], sizeof(struct dirent));
            if (++index == count) {
                tmp_entries = (struct dirent **)realloc(entries, sizeof(struct dirent *) * count * 2);
                if (!tmp_entries) {
                    closedir(d);
                    for (i = 0; i < index; ++i)
                        free(entries[index - 1]);
                    free(entries);
                    __seterrno(ENOMEM);
                    return -1;
                }
                entries = tmp_entries;
                count *= 2;
            }
        }
    }
    qsort(entries, index, sizeof(struct dirent *), compar);
    entries[index] = NULL;
    if (namelist)
        *namelist = entries;
    closedir(d);
    return 0;
}

int alphasort(const void *a, const void *b)
{
    struct dirent **dira = (struct dirent **)a, **dirb = (struct dirent **)b;
    if (!dira || !dirb)
        return 0;
    return strcoll((*dira)->d_name, (*dirb)->d_name);
}

static int __strverscmp(const char *s1, const char *s2)
{
    return alphasort(s1, s2);
}

int versionsort(const void *a, const void *b)
{
    struct dirent **dira = (struct dirent **)a, **dirb = (struct dirent **)b;
    if (!dira || !dirb)
        return 0;
    return __strverscmp((*dira)->d_name, (*dirb)->d_name);
}
