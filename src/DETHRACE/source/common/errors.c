#include "errors.h"

char *gError_messages[126];
int gError_code;
char *gPalette_copy;
int gPixel_buffer_size;
int gMouse_was_started;
char *gPixels_copy;

// Offset: 0
// Size: 207
void FatalError(int pStr_index, ...) {
    char the_str[1024];
    char *sub_str;
    char temp_str[1024];
    char *sub_pt;
    va_list ap;
}

// Offset: 208
// Size: 172
void NonFatalError(int pStr_index, ...) {
    char the_str[256];
    char *sub_str;
    char temp_str[256];
    char *sub_pt;
    va_list ap;
}

// Offset: 380
// Size: 34
void CloseDiagnostics() {
}

// Offset: 416
// Size: 34
void OpenDiagnostics() {
}

// Offset: 452
// Size: 34
void dprintf(char *fmt_string) {
}

// Offset: 488
// Size: 57
// EAX: pMisc_text_index
int DoErrorInterface(int pMisc_text_index) {
}

