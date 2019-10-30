#include "loading.h"

// Global variables
char *gWheel_actor_names[6];
int gFunk_groove_flags[30];
char *gNet_avail_names[4];
char *gDamage_names[12];
char *gDrivable_car_names[6];
char *gYour_car_names[2][6];
char gDef_def_water_screen_name[32];
tHeadup_info gHeadup_image_info[31];
int gAllow_open_to_fail;
br_material *gDestn_screen_mat;
br_material *gSource_screen_mat;
char *gRaces_file_names[9];
char *gFloorpan_names[5];
int gCurrent_race_file_index;
int gGroove_funk_offset;
int gDemo_armour;
int gDemo_rank;
int gDecode_thing;
int gDemo_opponents[5];
int gDemo_power;
int gDemo_offensive;
char gDecode_string[14];

// Offset: 0
// Size: 67
// EAX: pF
tU32 ReadU32(FILE *pF) {
  tU32 raw_long;
}

// Offset: 68
// Size: 67
// EAX: pF
tU16 ReadU16(FILE *pF) {
  tU16 raw_short;
}

// Offset: 136
// Size: 67
// EAX: pF
tU8 ReadU8(FILE *pF) {
  tU8 raw_byte;
}

// Offset: 204
// Size: 67
// EAX: pF
tS32 ReadS32(FILE *pF) {
  tS32 raw_long;
}

// Offset: 272
// Size: 67
// EAX: pF
tS16 ReadS16(FILE *pF) {
  tS16 raw_short;
}

// Offset: 340
// Size: 67
// EAX: pF
tS8 ReadS8(FILE *pF) {
  tS8 raw_byte;
}

// Offset: 408
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU32L(FILE *pF, tU32 pNumber) {
  tU32 raw_long;
}

// Offset: 476
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU16L(FILE *pF, tU16 pNumber) {
  tU16 raw_short;
}

// Offset: 544
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU8L(FILE *pF, tU8 pNumber) {
  tU8 raw_byte;
}

// Offset: 612
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS32L(FILE *pF, tS32 pNumber) {
  tS32 raw_long;
}

// Offset: 680
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS16L(FILE *pF, tS16 pNumber) {
  tS16 raw_short;
}

// Offset: 748
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS8L(FILE *pF, tS8 pNumber) {
  tS8 raw_byte;
}

// Offset: 816
// Size: 54
// EAX: pF
// EDX: pBytes_to_skip
void SkipBytes(FILE *pF) {
  int pBytes_to_skip;
}

// Offset: 872
// Size: 70
// EAX: pPtr
tU32 MemReadU32(char **pPtr) {
  tU32 raw_long;
}

// Offset: 944
// Size: 70
// EAX: pPtr
tU16 MemReadU16(char **pPtr) {
  tU16 raw_short;
}

// Offset: 1016
// Size: 55
// EAX: pPtr
tU8 MemReadU8(char **pPtr) {
}

// Offset: 1072
// Size: 70
// EAX: pPtr
tS32 MemReadS32(char **pPtr) {
  tS32 raw_long;
}

// Offset: 1144
// Size: 70
// EAX: pPtr
tS16 MemReadS16(char **pPtr) {
  tS16 raw_short;
}

// Offset: 1216
// Size: 55
// EAX: pPtr
tS8 MemReadS8(char **pPtr) {
  tS8 raw_byte;
}

// Offset: 1272
// Size: 46
// EAX: pPtr
// EDX: pBytes_to_skip
void MemSkipBytes(char **pPtr) {
  int pBytes_to_skip;
}

// Offset: 1320
// Size: 2149
void LoadGeneralParameters() {
  tPath_name the_path;
  int i;
  int temp;
  char s[256];
  char *str;
}

// Offset: 3472
// Size: 53
void FinishLoadingGeneral() {
}

// Offset: 3528
// Size: 292
// EAX: pName
br_pixelmap* LoadPixelmap(char *pName) {
  tPath_name the_path;
  br_pixelmap *pm;
  char *end;
}

// Offset: 3820
// Size: 222
// EAX: pFile_name
// EDX: pPixelmaps
// EBX: pNum
br_uint_32 LoadPixelmaps(char *pFile_name, br_pixelmap **pPixelmaps, br_uint_16 pNum, unsigned char path) {
  int count;
}

// Offset: 4044
// Size: 99
// EAX: pName
br_pixelmap* LoadShadeTable(char *pName) {
  tPath_name the_path;
}

// Offset: 4144
// Size: 129
// EAX: pName
br_material* LoadMaterial(char *pName) {
  tPath_name the_path;
  br_material *result;
}

// Offset: 4276
// Size: 123
// EAX: pName
br_model* LoadModel(char *pName) {
  tPath_name the_path;
  br_model *model;
}

// Offset: 4400
// Size: 104
// EAX: pName
br_actor* LoadActor(char *pName) {
  tPath_name the_path;
}

// Offset: 4504
// Size: 78
// EAX: pPath_name
void DRLoadPalette(char *pPath_name) {
  br_pixelmap *palette_array[100];
  int number_of_palettes;
}

// Offset: 4584
// Size: 78
// EAX: pPath_name
void DRLoadShadeTable(char *pPath_name) {
  br_pixelmap *table_array[100];
  int number_of_tables;
}

// Offset: 4664
// Size: 84
// EAX: pPixelmap_array
// EDX: pCount
void RezeroPixelmaps(br_pixelmap **pPixelmap_array) {
  int pCount;
}

// Offset: 4748
// Size: 97
// EAX: pPath_name
void DRLoadPixelmaps(char *pPath_name) {
  br_pixelmap *pixelmap_array[100];
  int number_of_pixelmaps;
}

// Offset: 4848
// Size: 103
// EAX: pPath_name
void DRLoadMaterials(char *pPath_name) {
  br_material *material_array[100];
  int number_of_materials;
}

// Offset: 4952
// Size: 103
// EAX: pPath_name
void DRLoadModels(char *pPath_name) {
  br_model *model_array[100];
  int number_of_models;
}

// Offset: 5056
// Size: 134
// EAX: pPath_name
void DRLoadActors(char *pPath_name) {
  br_actor *actor_array[100];
  int number_of_actors;
  int i;
}

// Offset: 5192
// Size: 134
// EAX: pPath_name
void DRLoadLights(char *pPath_name) {
  br_actor *light_array[100];
  int number_of_lights;
  int i;
}

// Offset: 5328
// Size: 70
// EAX: pThe_base_path
// EDX: pThe_dir_name
// EBX: pLoad_routine
void LoadInFiles(char *pThe_base_path, char *pThe_dir_name, void (*pLoad_routine)(char*), signed char the_path) {
}

// Offset: 5400
// Size: 208
// EAX: pThe_dir_path
void LoadInRegisteeDir(char *pThe_dir_path) {
  tPath_name the_path;
}

// Offset: 5608
// Size: 44
void LoadInRegistees() {
}

// Offset: 5652
// Size: 182
void LoadKeyMapping() {
  tPath_name the_path;
  int i;
}

// Offset: 5836
// Size: 269
// EAX: pWithin_race
void LoadInterfaceStuff(int pWithin_race) {
  tPath_name path;
  int i;
}

// Offset: 6108
// Size: 272
void UnlockInterfaceStuff() {
}

// Offset: 6380
// Size: 75
void InitInterfaceLoadState() {
}

// Offset: 6456
// Size: 627
// EAX: pBr_map
tS8* ConvertPixTo16BitStripMap(br_pixelmap *pBr_map) {
  int i;
  int j;
  int new_line_length;
  int current_size;
  int counting_blanks;
  int counter;
  int chunk_counter;
  int max_line_bytes;
  tU8 *next_byte;
  tU8 *strip_image;
  tU8 *current_strip_pointer;
  tU8 *temp_strip_image;
  tU8 *new_line;
  tU8 byte;
  tU16 *palette_entry;
}

// Offset: 7084
// Size: 578
// EAX: pThe_br_map
tS8* ConvertPixToStripMap(br_pixelmap *pThe_br_map) {
  int i;
  int j;
  int new_line_length;
  int current_size;
  int counting_blanks;
  int counter;
  int chunk_counter;
  tU8 *next_byte;
  tU8 *the_strip_image;
  tU8 *current_strip_pointer;
  tU8 *temp_strip_image;
  tU8 new_line[800];
  tU8 the_byte;
}

// Offset: 7664
// Size: 147
// EAX: pModel
// EDX: pMaterial
void KillWindscreen(br_model *pModel, br_material *pMaterial) {
  br_face *face;
  int i;
}

// Offset: 7812
// Size: 103
// EAX: pCar
void DropOffDyingPeds(tCar_spec *pCar) {
  br_actor *child;
  br_actor *next;
}

// Offset: 7916
// Size: 1113
// EAX: pCar_spec
// EDX: pOwner
void DisposeCar(tCar_spec *pCar_spec) {
  int pOwner;
  int i;
  int j;
}

// Offset: 9032
// Size: 626
// EAX: pCar
void AdjustCarCoordinates(tCar_spec *pCar) {
  int i;
}

// Offset: 9660
// Size: 929
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadSpeedo(FILE *pF, int pIndex) {
  tCar_spec *pCar_spec;
  tPath_name the_path;
  char s[256];
  char *str;
  char the_char1;
  char the_char2;
}

// Offset: 10592
// Size: 764
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadTacho(FILE *pF, int pIndex) {
  tCar_spec *pCar_spec;
  tPath_name the_path;
  char s[256];
  char *str;
  char the_char1;
  char the_char2;
}

// Offset: 11356
// Size: 816
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadHeadups(FILE *pF, int pIndex) {
  tCar_spec *pCar_spec;
  char s[256];
  char *str;
  int j;
  int number_of_slots;
}

// Offset: 12172
// Size: 1819
// EAX: pF
// EDX: non_car
void ReadNonCarMechanicsData(FILE *pF, tNon_car_spec *non_car) {
  int number;
  int i;
  int j;
  char s[256];
  tCollision_info *c;
  br_scalar wid;
  br_scalar het;
  br_scalar len;
  br_scalar ts;
  br_scalar ts1;
  br_scalar snap_angle;
}

// Offset: 13992
// Size: 3476
// EAX: pF
// EDX: c
void ReadMechanicsData(FILE *pF, tCar_spec *c) {
  char s[256];
  char version;
  int i;
  int j;
  br_scalar ratio;
  br_scalar ts1;
  br_scalar theta_front;
  br_scalar theta_back;
  br_scalar theta_comp;
  br_scalar wid;
  br_scalar het;
  br_scalar len;
  br_vector3 *actor_offset;
  br_scalar speed;
  br_scalar force;
}

// Offset: 17468
// Size: 196
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadGear(FILE *pF, int pIndex) {
  tCar_spec *pCar_spec;
  tPath_name the_path;
  char s[256];
  char *str;
}

// Offset: 17664
// Size: 56
// EAX: pRef_holder
void AddRefOffset(int *pRef_holder) {
}

// Offset: 17720
// Size: 697
// EAX: pF
// EDX: pCar_spec
// EBX: pImpact_location
void GetDamageProgram(FILE *pF, tCar_spec *pCar_spec, tImpact_location pImpact_location) {
  tDamage_clause *the_clause;
  int i;
  int j;
  int k;
  int count;
  char s[256];
  char delim[64];
  char *str;
}

// Offset: 18420
// Size: 178
br_uint_32 LinkModel(br_actor *pActor, tModel_pool *pModel_pool) {
  int i;
}

// Offset: 18600
// Size: 198
// EAX: pModel_array
// EDX: pModel_count
void FreeUpBonnetModels(br_model **pModel_array) {
  int pModel_count;
  int i;
}

// Offset: 18800
// Size: 67
// EAX: pActor
// EDX: pModel_array
// EBX: pModel_count
void LinkModelsToActor(br_actor *pActor, br_model **pModel_array) {
  int pModel_count;
  tModel_pool model_pool;
}

// Offset: 18868
// Size: 132
// EAX: pF
// EDX: pCar_spec
void ReadShrapnelMaterials(FILE *pF, tCollision_info *pCar_spec) {
  char s[256];
  char version;
  int i;
}

// Offset: 19000
// Size: 200
// EAX: pOutput_car
// EDX: pInput_car
void CloneCar(tCar_spec **pOutput_car, tCar_spec *pInput_car) {
  int i;
}

// Offset: 19200
// Size: 65
// EAX: pCar
void DisposeClonedCar(tCar_spec *pCar) {
}

// Offset: 19268
// Size: 523
// EAX: pModel
int RemoveDoubleSided(br_model *pModel) {
  br_face *faces;
  br_face *face;
  int temp;
  int num_double_sided_faces;
  int i;
  int orig_nfaces;
  int result;
}

// Offset: 19792
// Size: 201
// EAX: pModel
void MungeWindscreen(br_model *pModel) {
  br_face *face;
  int i;
}

// Offset: 19996
// Size: 141
// EAX: pModel
// EDX: pOwner
void SetModelFlags(br_model *pModel) {
  int pOwner;
}

// Offset: 20140
// Size: 8202
// EAX: pCar_name
// EDX: pDriver
// EBX: pCar_spec
// ECX: pOwner
void LoadCar(char *pCar_name, tDriver pDriver, tCar_spec *pCar_spec, int pOwner, char *pDriver_name, tBrender_storage *pStorage_space, signed char f, _complex g) {
  tPath_name the_path;
  int i;
  int j;
  int k;
  int its_a_floorpan;
  int number_of_floorpans;
  int point_num;
  int initial_vertex;
  int old_model_count;
  int old_material_count;
  int temp_index;
  int vertex_array_size;
  char s[256];
  char *str;
  br_pixelmap *the_image;
  float rate;
  float temp_float;
  br_model *model;
  br_vector3 tv;
  int v;
  int v_num;
  int group;
  int vertex_total;
}

// Offset: 28344
// Size: 186
void LoadHeadupImages() {
  tPath_name the_path;
}

// Offset: 28532
// Size: 94
void DisposeHeadupImages() {
  tPath_name the_path;
}

// Offset: 28628
// Size: 109
FILE* OpenRaceFile() {
  tPath_name the_path;
}

// Offset: 28740
// Size: 219
// EAX: pF
void SkipRestOfRace(FILE *pF) {
  int j;
  int k;
  int text_chunk_count;
  int line_count;
  char s[256];
}

// Offset: 28960
// Size: 411
// EAX: pRace_list
// EDX: pCount
// EBX: pRace_type_index
void LoadRaces(tRace_list_spec *pRace_list, int *pCount) {
  int pRace_type_index;
  FILE *f;
  int i;
  int j;
  int k;
  int number_of_racers;
  int last_race;
  char s[256];
  char *str;
}

// Offset: 29372
// Size: 88
// EAX: pIndex
void UnlockOpponentMugshot(int pIndex) {
}

// Offset: 29460
// Size: 168
// EAX: pIndex
void LoadOpponentMugShot(int pIndex) {
}

// Offset: 29628
// Size: 136
// EAX: pRace_info
// EDX: pIndex
void DisposeOpponentGridIcon(tRace_info *pRace_info) {
  int pIndex;
}

// Offset: 29764
// Size: 155
// EAX: pRace_info
// EDX: pIndex
void LoadOpponentGridIcon(tRace_info *pRace_info) {
  int pIndex;
}

// Offset: 29920
// Size: 804
// EAX: pRace_index
// EDX: pRace_info
void LoadRaceInfo(int pRace_index, tRace_info *pRace_info, signed char f) {
  int i;
  int j;
  int k;
  int duplicate;
  int substitute;
  int auto_scum_count;
  int old_index;
  int temp_index;
  char s[256];
  char *str;
  float temp_float;
  tText_chunk *the_chunk;
}

// Offset: 30724
// Size: 351
// EAX: pRace_info
void DisposeRaceInfo(tRace_info *pRace_info) {
  int i;
  int j;
  int k;
  tText_chunk *the_chunk;
}

// Offset: 31076
// Size: 122
// EAX: pRace_info
void LoadGridIcons(tRace_info *pRace_info) {
  int i;
}

// Offset: 31200
// Size: 119
// EAX: pRace_info
void DisposeGridIcons(tRace_info *pRace_info) {
  int i;
}

// Offset: 31320
// Size: 1075
void LoadOpponents() {
  tPath_name the_path;
  int i;
  int j;
  int k;
  char s[256];
  char *str;
  tText_chunk *the_chunk;
}

// Offset: 32396
// Size: 647
// EAX: pName
br_font* LoadBRFont(char *pName) {
  FILE *f;
  tPath_name the_path;
  br_font *the_font;
  tU32 data_size;
  int i;
}

// Offset: 33044
// Size: 262
void LoadParts() {
  int j;
}

// Offset: 33308
// Size: 149
void UnlockParts() {
  int j;
}

// Offset: 33460
// Size: 72
br_pixelmap* LoadChromeFont() {
}

// Offset: 33532
// Size: 49
// EAX: pThe_font
void DisposeChromeFont(br_pixelmap *pThe_font) {
}

// Offset: 33584
// Size: 140
// EAX: pF
// EDX: pString_list
// EBX: pCount
int GetALineAndInterpretCommand(FILE *pF, char **pString_list) {
  int pCount;
  int i;
  char s[256];
  char *str;
}

// Offset: 33724
// Size: 101
// EAX: pF
int GetAnInt(FILE *pF) {
  char s[256];
  char *str;
  int result;
}

// Offset: 33828
// Size: 101
// EAX: pF
float GetAFloat(FILE *pF) {
  char s[256];
  char *str;
  float result;
}

// Offset: 33932
// Size: 107
// EAX: pF
float GetAFloatPercent(FILE *pF) {
  char s[256];
  char *str;
  float result;
}

// Offset: 34040
// Size: 131
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfFloats(FILE *pF, float *pF1, float *pF2) {
  char s[256];
  char *str;
}

// Offset: 34172
// Size: 169
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeFloats(FILE *pF, float *pF1, float *pF2, float *pF3) {
  char s[256];
  char *str;
}

// Offset: 34344
// Size: 131
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfInts(FILE *pF, int *pF1) {
  int *pF2;
  char s[256];
  char *str;
}

// Offset: 34476
// Size: 169
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeInts(FILE *pF, int *pF1) {
  int *pF2;
  int *pF3;
  char s[256];
  char *str;
}

// Offset: 34648
// Size: 197
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeIntsAndAString(FILE *pF, int *pF1, int *pF2) {
  int *pF3;
  char *pS;
  char s[256];
  char *str;
}

// Offset: 34848
// Size: 208
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFourInts(FILE *pF, int *pF1, int *pF2) {
  int *pF3;
  int *pF4;
  char s[256];
  char *str;
}

// Offset: 35056
// Size: 51
// EAX: pF
br_scalar GetAScalar(FILE *pF) {
}

// Offset: 35108
// Size: 69
// EAX: pF
// EDX: pS1
// EBX: pS2
void GetPairOfScalars(FILE *pF, br_scalar *pS1, br_scalar *pS2) {
}

// Offset: 35180
// Size: 81
// EAX: pF
// EDX: pS1
// EBX: pS2
// ECX: pS3
void GetThreeScalars(FILE *pF, br_scalar *pS1, br_scalar *pS2, br_scalar *pS3) {
}

// Offset: 35264
// Size: 240
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFourScalars(FILE *pF, br_scalar *pF1, br_scalar *pF2, br_scalar *pF3, br_scalar *pF4) {
  char s[256];
  char *str;
  float f1;
  float f2;
  float f3;
  float f4;
}

// Offset: 35504
// Size: 285
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFiveScalars(FILE *pF, br_scalar *pF1, br_scalar *pF2, br_scalar *pF3, br_scalar *pF4, br_scalar *pF5) {
  char s[256];
  char *str;
  float f1;
  float f2;
  float f3;
  float f4;
  float f5;
}

// Offset: 35792
// Size: 152
// EAX: pF
// EDX: pNumber
// EBX: pScalars
void GetNScalars(FILE *pF, int pNumber) {
  br_scalar *pScalars;
  char s[256];
  char *str;
  float fleurting_point_numero;
  int i;
}

// Offset: 35944
// Size: 157
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfFloatPercents(FILE *pF, float *pF1, float *pF2) {
  char s[256];
  char *str;
}

// Offset: 36104
// Size: 208
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeFloatPercents(FILE *pF, float *pF1, float *pF2, float *pF3) {
  char s[256];
  char *str;
}

// Offset: 36312
// Size: 82
// EAX: pF
// EDX: pString
void GetAString(FILE *pF) {
  char *pString;
  char s[256];
  char *str;
}

// Offset: 36396
// Size: 49
void AboutToLoadFirstCar() {
}

// Offset: 36448
// Size: 275
// EAX: pRace_info
void LoadOpponentsCars(tRace_info *pRace_info) {
  int i;
}

// Offset: 36724
// Size: 184
// EAX: pRace_info
void DisposeOpponentsCars(tRace_info *pRace_info) {
  int i;
}

// Offset: 36908
// Size: 211
void LoadMiscStrings() {
  FILE *f;
  char s[256];
  tPath_name the_path;
}

// Offset: 37120
// Size: 53
// EAX: pThe_race
void FillInRaceInfo(tRace_info *pThe_race) {
}

// Offset: 37176
// Size: 1156
// EAX: pFilename
// EDX: pMode
FILE* OldDRfopen(char *pFilename, char *pMode) {
  FILE *fp;
  FILE *file_ptr;
  FILE *test1;
  FILE *test2;
  char *data_dir;
  tPath_name CD_dir;
  tPath_name path_file;
  tPath_name source_check;
  int source_exists;
  int len;
}

// Offset: 38332
// Size: 44
void AllowOpenToFail() {
}

// Offset: 38376
// Size: 34
void DoNotAllowOpenToFail() {
}

// Offset: 38412
// Size: 189
// EAX: pFilename
// EDX: pMode
FILE* DRfopen(char *pFilename, char *pMode) {
  FILE *result;
  tPath_name CD_dir;
  char msg[336];
}

// Offset: 38604
// Size: 178
// EAX: pPath_name
int GetCDPathFromPathsTxtFile(char *pPath_name) {
  int got_it_already;
  tPath_name cd_pathname;
  FILE *paths_txt_fp;
  tPath_name paths_txt;
}

// Offset: 38784
// Size: 44
int TestForOriginalCarmaCDinDrive() {
}

// Offset: 38828
// Size: 45
int OriginalCarmaCDinDrive() {
}

// Offset: 38876
// Size: 63
int CarmaCDinDriveOrFullGameInstalled() {
}

// Offset: 38940
// Size: 191
// EAX: pF
// EDX: pOptions
void ReadNetworkSettings(FILE *pF, tNet_game_options *pOptions) {
}

// Offset: 39132
// Size: 378
// EAX: pF
// EDX: pIndex
int PrintNetOptions(FILE *pF) {
  int pIndex;
}

// Offset: 39512
// Size: 1398
int SaveOptions() {
  FILE *f;
}

// Offset: 40912
// Size: 1643
int RestoreOptions() {
  FILE *f;
  char line[80];
  char token[80];
  char *s;
  float arg;
}

// Offset: 42556
// Size: 75
void InitFunkGrooveFlags() {
}

