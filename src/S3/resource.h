#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "brender/brender.h"
#include "s3_defs.h"

typedef enum s3_memory_classes {
    kMem_S3_scan_name = 223,                                             //  0xdf
    kMem_S3_sound_header = 224,                                          //  0xe0
    kMem_S3_sample = 225,                                                //  0xe1
    kMem_S3_mac_channel = 226,                                           //  0xe2
    kMem_S3_mac_path = 227,                                              //  0xe3
    kMem_S3_sentinel = 228,                                              //  0xe4
    kMem_S3_outlet = 229,                                                //  0xe5
    kMem_S3_channel = 230,                                               //  0xe6
    kMem_S3_descriptor = 231,                                            //  0xe7
    kMem_S3_reverse_buffer = 232,                                        //  0xe8
    kMem_S3_source = 233,                                                //  0xe9
    kMem_S3_DOS_SOS_channel = 234,                                       //  0xea
    kMem_S3_PC_DOS_path = 235,                                           //  0xeb
    kMem_S3_DOS_SOS_patch = 236,                                         //  0xec
    kMem_S3_DOS_SOS_song_structure = 237,                                //  0xed
    kMem_S3_DOS_SOS_song_data = 238,                                     //  0xee
    kMem_S3_Windows_95_load_WAV_file = 239,                              //  0xef
    kMem_S3_Windows_95_create_temp_buffer_space_to_reverse_sample = 240, //  0xf0
    kMem_S3_Windows_95_path = 241,                                       //  0xf1
    kMem_DOS_HMI_file_open = 242,                                        //  0xf2
} s3_memory_classes;

void* S3MemAllocate(br_size_t size, br_uint_8 type);
void S3MemFree(void* p);
void S3CopyBrVector(tS3_vector3* a1, br_vector3* a2);

#endif
