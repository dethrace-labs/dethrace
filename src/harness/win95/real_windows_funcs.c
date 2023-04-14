#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#include <io.h>

HANDLE Real_FindFirstFile(char* lpFileName, char* lpFindFileData_filename) {
    WIN32_FIND_DATA find_data;
    HANDLE res = FindFirstFile(lpFileName, &find_data);
    if (res) {
        strcpy(lpFindFileData_filename, find_data.cFileName);
    }
    return res;
}

BOOL Real_FindNextFile(HANDLE hFindFile, char* lpFindFileData_filename) {
    WIN32_FIND_DATA find_data;
    HANDLE res = FindNextFile(hFindFile, &find_data);
    if (res) {
        strcpy(lpFindFileData_filename, find_data.cFileName);
    }
    return res;
}

BOOL Real_FindClose(HANDLE hFindFile) {
    FindClose(hFindFile);
}

#endif
