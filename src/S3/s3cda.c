#include "s3cda.h"
#include "harness/trace.h"

int gS3_cda_enabled;

void S3DisableCDA() {
    S3StopCDAOutlets();
    gS3_cda_enabled = 0;
}

void S3StopCDAOutlets() {
    STUB();
}

int S3OpenCDADevice() {
    // gS3_cda_device.lpstrDeviceType = (LPCSTR)516;
    // if (mciSendCommandA(0, 0x803u, 0x3000u, (DWORD_PTR)&gS3_cda_device)
    //     && mciSendCommandA(0, 0x803u, 0x3100u, (DWORD_PTR)&gS3_cda_device)) {
    //     return 0;
    // }
    // stru_550560.dwTimeFormat = 10; // MCI_FORMAT_TMSF
    // mciSendCommandA(gS3_cda_device.wDeviceID, 0x80Du, 0x400u, (DWORD_PTR)&stru_550560);
    // S3CDAEnable();
    // return 1;

    return 0;
}

int S3PlayCDA(tS3_channel* chan) {
    return 0;
}

int S3StopCDA(tS3_channel* chan) {
    return 0;
}

int S3SetCDAVolume(tS3_channel* chan, int pVolume) {
    // if ( gS3_cda_enabled )
    // {
    //     sub_49CC70();
    // }
    return 0;
}

int S3IsCDAPlaying() {
    return 0;
}