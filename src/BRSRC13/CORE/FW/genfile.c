#include "genfile.h"

br_file_struct_member file_info_FM[2];
br_file_struct file_info_F;
br_file_enum_member file_type_FM[10];
br_file_enum file_type_F;
char rscid[50];

// Offset: 12
// Size: 50
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_END(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
}

// Offset: 75
// Size: 63
// EAX: df
int FopWrite_END(br_datafile* df) {
}

// Offset: 157
// Size: 120
// EAX: df
// EDX: type
int FopWrite_FILE_INFO(br_datafile* df, br_uint_32 type) {
    file_info fi;
}
