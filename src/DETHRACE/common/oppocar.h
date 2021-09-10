#ifndef _OPPOCAR_H_
#define _OPPOCAR_H_

#include "br_types.h"
#include "dr_types.h"

extern int gCollision_detection_on__oppocar; // suffix added to avoid duplicate symbol
extern br_vector3 gGround_normal__oppocar; // suffix added to avoid duplicate symbol
extern void (*ControlCar__oppocar[6])(tCar_spec*, br_scalar); // suffix added to avoid duplicate symbol
extern int gControl__oppocar; // suffix added to avoid duplicate symbol
extern int gFace_num__oppocar; // suffix added to avoid duplicate symbol
extern br_angle gOld_yaw__oppocar; // suffix added to avoid duplicate symbol
extern int gMetal_crunch_sound_id__oppocar[5]; // suffix added to avoid duplicate symbol
extern int gMetal_scrape_sound_id__oppocar[3]; // suffix added to avoid duplicate symbol
extern tFace_ref gFace_list__oppocar[32]; // suffix added to avoid duplicate symbol
extern br_scalar gOur_yaw__oppocar; // suffix added to avoid duplicate symbol
extern br_scalar gGravity__oppocar; // suffix added to avoid duplicate symbol
extern br_vector3 gNew_ground_normal__oppocar; // suffix added to avoid duplicate symbol

void MakeCarStationary(tCar_spec* pCar_spec);

void MoveThisCar(tU32 pTime_difference, tCar_spec* car);

#endif
