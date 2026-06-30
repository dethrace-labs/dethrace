#ifndef SMACKW32_H
#define SMACKW32_H

#include "harness/audio.h"
#include <stddef.h>

#define SMACKTRACK1 0x02000 // Play audio track 1
#define SMACKTRACK2 0x04000 // Play audio track 2
#define SMACKTRACK3 0x08000 // Play audio track 3
#define SMACKTRACK4 0x10000 // Play audio track 4
#define SMACKTRACK5 0x20000 // Play audio track 5
#define SMACKTRACK6 0x40000 // Play audio track 6
#define SMACKTRACK7 0x80000 // Play audio track 7
#define SMACKTRACKS (SMACKTRACK1 | SMACKTRACK2 | SMACKTRACK3 | SMACKTRACK4 | SMACKTRACK5 | SMACKTRACK6 | SMACKTRACK7)
#define SMACKAUTOEXTRA 0xffffffff

typedef struct SmackTag {
    unsigned long Version;
    unsigned long Width;
    unsigned long Height;
    unsigned long Frames;
    unsigned long MSPerFrame;
    unsigned long SmackerType;
    unsigned long LargestInTrack[7];
    unsigned long tablesize;
    unsigned long codesize;
    unsigned long absize;
    unsigned long detailsize;
    unsigned long typesize;
    unsigned long TrackType[7];
    unsigned long extra;
    unsigned long NewPalette;
    unsigned char Palette[772];
    unsigned long PalType;
    unsigned long FrameNum;
    unsigned long FrameSize;
    unsigned long SndSize;
    unsigned long LastRectx;
    unsigned long LastRecty;
    unsigned long LastRectw;
    unsigned long LastRecth;
    unsigned long OpenFlags;
    unsigned long LeftOfs;
    unsigned long TopOfs;
    unsigned long ReadError;
    unsigned long addr32;

    // added by dethrace
    void* smk_handle; // opaque pointer to the libsmacker instance
    void* f;          // opaque file pointer
    tAudioBackend_stream* audio_stream;
} Smack;

#if defined(_WIN32)
#define SMACK_CALL __stdcall
#else
#define SMACK_CALL
#endif

extern Smack* SMACK_CALL SmackOpen(const char* name, unsigned int flags, unsigned int extrabuf);
extern int SMACK_CALL SmackSoundUseDirectSound(void* dd); // NULL mean create instance (apparently)
extern void SMACK_CALL SmackToBuffer(Smack* smack, unsigned int left, unsigned int top, unsigned int pitch, unsigned int destheight, void* buf, unsigned int flags);
extern int SMACK_CALL SmackDoFrame(Smack* smack);
extern void SMACK_CALL SmackNextFrame(Smack* smack);
extern int SMACK_CALL SmackWait(Smack* smack);
extern void SMACK_CALL SmackClose(Smack* smack);

#endif SMACKW32_H
