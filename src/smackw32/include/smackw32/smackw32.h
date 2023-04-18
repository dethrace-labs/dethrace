#include <stddef.h>
#include <stdint.h>

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
    void* smk_handle;
} Smack;

Smack* SmackOpen(const char* name, uint32_t flags, uint32_t extrabuf);
int SmackSoundUseDirectSound(void* dd); // NULL mean create instance (apparently)
void SmackToBuffer(Smack* smack, uint32_t left, uint32_t top, uint32_t pitch, uint32_t destheight, void* buf, uint32_t flags);
uint32_t SmackDoFrame(Smack* smack);
void SmackNextFrame(Smack* smack);
uint32_t SmackWait(Smack* smack);
void SmackClose(Smack* smack);
