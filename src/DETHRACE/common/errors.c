#include "errors.h"
#include "cutscene.h"
#include "globvars.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "network.h"
#include "pd/sys.h"
#include "utility.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* gError_messages[126] = {
    "Unable to support this screen depth setting",
    "Couldn't allocate off-screen buffer",
    "Couldn't allocate Z-Buffer",
    "Couldn't allocate root actor",
    "Couldn't calculate the application's pathname",
    "Couldn't allocate camera",
    "Couldn't allocate self",
    "There was a problem with the Preferences file - try deleting it.",
    "Couldn't open General Settings file",
    "Couldn't open Key Map file",
    "Unable to find required palette",
    "Couldn't allocate and install standard lamp",
    "Couldn't allocate and install stupid test bastard",
    "Couldn't load FLIC file '%'",
    "Couldn't allocate memory to play FLIC file '%'",
    "The FLIC file '%' was not 8-bits deep",
    "Couldn't ascertain a frame-rate for FLIC file '%'",
    "A problem occurred whilst trying to free up some FLICs '%'",
    "Couldn't find a spare transient bitmap",
    "Save file was bigger than save structure",
    "Can't load font image '%'",
    "Can't load font width table '%'",
    "Can't load cursor image",
    "Can't load cursor giblet",
    "Screen width/height doesn't match graf data",
    "Can't load car resolution-independant file",
    "Can't load cockpit image",
    "Can't load damage image",
    "Can't open prat cam text file",
    "Premature flic specs end in prat cam text file",
    "Can't open prat cam flic file '%'",
    "Can't allocate memory for prat cam flic file",
    "Too many flic specs in prat cam text file",
    "Premature sequences end in prat cam text file",
    "Too many sequences in prat cam text file",
    "Can't load speedo image",
    "Can't load tacho image",
    "Pratcam sequence % has too many chunks",
    "Pratcam sequence % has too many alternatives",
    "Pratcam sequence % has too many sounds",
    "Can't load hands image",
    "Flying wombat alert: flic palette disposed before it's been allocated",
    "Can't load dials pix",
    "Trying to free a NULL pointer",
    "Trying to free an invalid pointer (%)",
    "Trying to free an already free block (%)",
    "Trying to lock an invalid pointer (%)",
    "Trying to unlock an invalid pointer (%)",
    "Out of memory (insufficient heap space)",
    "Out of memory (insufficient block slots)",
    "Can't open races file",
    "Can't load race scene image",
    "Can't load race map image",
    "Can't load race info image",
    "Can't open opponents file",
    "Opponents count mismatches actual number of opponents",
    "Can't load opponent mug-shot file",
    "Can't load grid image file",
    "Can't load part image file",
    "Can't load chrome font file",
    "Can't load funkotronic file",
    "Can't load groovadelic file",
    "Error within funkotronic file",
    "Error within groovidelic file",
    "Can't find main material '%' referenced in funkotronic file",
    "Can't find actor '%' referenced in groovidelic file",
    "Can't find animation frame pixelmap referenced in funkotronic file",
    "Insufficient pixelmap slots in storage area",
    "Insufficient shade table slots in storage area",
    "Insufficient material slots in storage area",
    "Insufficient model slots in storage area",
    "Can't load car actor",
    "Defined ref-num of controlled groovidelic/funkotronic is out of range",
    "Used ref-num of controlled groovidelic/funkotronic is out of range",
    "MAMS heap is corrupt",
    "Can't open pedestrian file",
    "Can't find pedestrian with ref num %",
    "Can't find pedestrian material '%'",
    "Can't find pedestrian pixelmap '%'",
    "Can't load pixelmap file '%' (or it's empty)",
    "Can't load shade table file '%' (or it's empty)",
    "Can't load material file '%' (or it's empty)",
    "Can't load model file '%' (or it's empty)",
    "Too many frames in pedestrian sequence",
    "Too many choices in pedestrian instructions",
    "Too many bearings in pedestrian sequences",
    "Pedestrian instruction marker not found",
    "Can't load depth-cue shade table",
    "Unrecognised opponent path type '%'",
    "Can't find sky material '%'",
    "Can't find sky pixelmap '%'",
    "Unknown damage type '%'",
    "Confused by format of conditional damage in car file",
    "Too many extra points for car index '%'",
    "Sorry, but Carmageddon has run out of memory (%)",
    "Piping buffer prematurely reached end",
    "Piping buffer prematurely reached beginning",
    "Piped pedestrian made no sense",
    "Something in the piping system went odd",
    "Can't open TEXT.TXT file",
    "Can't open DARE.TXT file",
    "Can't find the FLIC '%' referenced in the interface translation file",
    "Can't open headups file",
    "Can't open partshop file",
    "Can't open car resolution-dependant file",
    "Received new player list - and we're not in it!!!",
    "Can't open key names file",
    "Can't open '%'",
    "Too many planes in special volume (max is %)",
    "Can't load  a generated shade table",
    "Network code self-check failed",
    "Network message corrupted",
    "Memory has become corrupted",
    "Ran out of funk/groove slot bunches",
    "Net contents too big %",
    "File % is corrupted",
    "Random number out of range (%)",
};
int gError_code;
char* gPalette_copy__errors;    // suffix added to avoid duplicate symbol
int gPixel_buffer_size__errors; // suffix added to avoid duplicate symbol
int gMouse_was_started__errors; // suffix added to avoid duplicate symbol
char* gPixels_copy__errors;     // suffix added to avoid duplicate symbol

// IDA: void __cdecl FatalError(int pStr_index, ...)
void FatalError(int pStr_index, ...) {
    char the_str[1024];
    char* sub_str;
    char temp_str[1024];
    char* sub_pt;
    va_list ap;
    int i;
    LOG_TRACE("(%d)", pStr_index);

    va_start(ap, pStr_index);

    gLast_demo_end_anim = 0x20000000 + PDGetTotalTime();
    strcpy(the_str, gError_messages[pStr_index]);
    sub_pt = temp_str;

    while (1) {

        sub_pt = strchr(the_str, '%');
        if (!sub_pt) {
            break;
        }
        sub_str = va_arg(ap, char*);
        StripCR(sub_str);
        strcpy(temp_str, sub_pt + 1);
        strcpy(sub_pt, sub_str);
        strcat(the_str, temp_str);
    }
    va_end(ap);
    dr_dprintf(the_str);
    FadePaletteUp();
    PDFatalError(the_str);
}

// IDA: void __cdecl NonFatalError(int pStr_index, ...)
void NonFatalError(int pStr_index, ...) {
    char the_str[256];
    char* sub_str;
    char temp_str[256];
    char* sub_pt;
    va_list ap;
    int i;
    LOG_TRACE("(%d)", pStr_index);

    va_start(ap, pStr_index);

    strcpy(the_str, gError_messages[pStr_index - 1]);
    sub_pt = temp_str;

    for (i = 0; i < strlen(the_str); i++) {
        if (the_str[i] == '%') {
            sub_str = va_arg(ap, char*);
            StripCR(sub_str);
            strcpy(sub_pt, sub_str);
            sub_pt += strlen(sub_str);
        } else {
            *sub_pt = the_str[i];
            sub_pt++;
        }
    }
    *sub_pt = 0;
    va_end(ap);
    PDNonFatalError(temp_str);
}

// IDA: void __cdecl CloseDiagnostics()
// This function is stripped from the retail binary, we've guessed at the implementation
void CloseDiagnostics(void) {
    LOG_TRACE("()");

    if (harness_game_config.enable_diagnostics == 0) {
        return;
    }

    fclose(gDiagnostic_file);
}

// IDA: void __cdecl OpenDiagnostics()
// This function is stripped from the retail binary, we've guessed at the implementation
void OpenDiagnostics(void) {
    LOG_TRACE("()");

    if (harness_game_config.enable_diagnostics == 0) {
        return;
    }

    gDiagnostic_file = fopen("DIAGNOST.TXT", "w");

    fputs("DIAGNOSTIC OUTPUT\n", gDiagnostic_file);
    // todo: generate a real date
    fprintf(gDiagnostic_file, "Date / time : %s\n\n\n", "Mon Mar 24 16 : 32 : 33 1997");
}

// Renamed from dprintf to avoid collisions to stdio
// This function is stripped from the retail binary, we've guessed at the implementation
void dr_dprintf(char* fmt_string, ...) {
    static tU32 first_time = 0;
    va_list args;
    tU32 the_time;

    if (harness_game_config.enable_diagnostics == 0) {
        return;
    }

    if (gDiagnostic_file == NULL) {
        return;
    }

    if (first_time == 0) {
        first_time = GetTotalTime();
    }
    the_time = GetTotalTime() - first_time;

    fprintf(gDiagnostic_file, "%7d.%02d: ", the_time / 1000, the_time % 100);

    va_start(args, fmt_string);
    vfprintf(gDiagnostic_file, fmt_string, args);
    va_end(args);
    fputs("\n", gDiagnostic_file);
    fflush(gDiagnostic_file);

    // Added by dethrace for debugging
    // va_start(args, fmt_string);
    // vprintf(fmt_string, args);
    // va_end(args);
    // printf("\n");
}

// IDA: int __usercall DoErrorInterface@<EAX>(int pMisc_text_index@<EAX>)
int DoErrorInterface(int pMisc_text_index) {
    LOG_TRACE("(%d)", pMisc_text_index);

    NetFullScreenMessage(pMisc_text_index, 0);
    return 0;
}
