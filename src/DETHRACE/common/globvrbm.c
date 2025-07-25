#include "globvrbm.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x00520038
br_scalar gYon_factor;

// GLOBAL: CARM95 0x00530c80
br_material* gMaterial[2];

// These variables are all used only by the 3dfx patch
int g16bit_palette_valid;
tException_list gExceptions;
br_actor* g2d_camera;
int gAlready_copied;
br_actor* gBlend_actor;
br_actor* gLine_actor;
br_model* gBlend_model;
br_model* gPrat_model;
char* gExceptions_general_file;
br_model* gDim_model;
br_material* gPrat_material;
br_material* gBlend_material;
char* gExceptions_file_suffix;
br_material* gDim_material;
br_actor* gPrat_actor;
br_material* gLine_material;
br_model* gLine_model;
br_actor* gDim_actor;
int gNo_transients;
int gSmall_frames_are_slow;
int gShade_tables_do_not_work;
int gInterpolate_textures;
int gNo_2d_effects;
int gPerspective_is_fast;
int gUse_mip_maps;
int gBlitting_is_slow;
int gTextures_need_powers_of_2;
int gMax_texture_side;
int gDevious_2d;
int gMax_texture_aspect_ratio;
int gMaterial_fogging;

// Added
int gVoodoo_rush_mode;
