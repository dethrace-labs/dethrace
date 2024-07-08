#include <stddef.h>
#include <stdint.h>

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
    int audio_sample_rate;
    int audio_frame_size_in_bytes; // TODO: consider using SndSize for storage instead?
    void* audio_paged_buffer;      // opaque pointer to a miniaudio ma_paged_audio_buffer struct
    void* audio_paged_buffer_data; // opaque pointer to a miniaudio ma_paged_audio_buffer_data struct
    void* audio_converter;         // opaque pointerto a miniaudio ma_data_converter struct
    void* audio_track;             // opaque pointer to a miniaudio ma_sound struct

    void* audio_stream;
} Smack;

Smack* SmackOpen(const char* name, uint32_t flags, uint32_t extrabuf);
int SmackSoundUseDirectSound(void* dd); // NULL mean create instance (apparently)
void SmackToBuffer(Smack* smack, uint32_t left, uint32_t top, uint32_t pitch, uint32_t destheight, void* buf, uint32_t flags);
uint32_t SmackDoFrame(Smack* smack);
void SmackNextFrame(Smack* smack);
uint32_t SmackWait(Smack* smack);
void SmackClose(Smack* smack);
