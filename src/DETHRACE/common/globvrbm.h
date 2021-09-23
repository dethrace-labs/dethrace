#ifndef _GLOBVRBM_H_
#define _GLOBVRBM_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern br_scalar gYon_factor;
extern br_material* gMaterial[2];
extern int g16bit_palette_valid;
extern tException_list gExceptions;
extern br_actor* g2d_camera;
extern int gAlready_copied;
extern br_actor* gBlend_actor;
extern br_actor* gLine_actor;
extern br_model* gBlend_model;
extern br_model* gPrat_model;
extern char* gExceptions_general_file;
extern br_model* gDim_model;
extern br_material* gPrat_material;
extern br_material* gBlend_material;
extern char* gExceptions_file_suffix;
extern br_material* gDim_material;
extern br_actor* gPrat_actor;
extern br_material* gLine_material;
extern br_model* gLine_model;
extern br_actor* gDim_actor;
extern int gNo_transients;
extern int gSmall_frames_are_slow;
extern int gShade_tables_do_not_work;
extern int gInterpolate_textures;
extern int gNo_2d_effects;
extern int gPerspective_is_fast;
extern int gUse_mip_maps;
extern int gBlitting_is_slow;
extern int gTextures_need_powers_of_2;
extern int gMax_texture_side;
extern int gDevious_2d;
extern int gMax_texture_aspect_ratio;
extern int gMaterial_fogging;

#endif
