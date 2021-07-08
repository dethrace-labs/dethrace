#include "oppocar.h"
#include <stdlib.h>

tFace_ref gFace_list[32];
void (*ControlCar[6])(tCar_spec*, br_scalar);
int gMetal_crunch_sound_id[5];
br_scalar gOur_yaw;
br_scalar gGravity;
br_vector3 gNew_ground_normal;
int gFace_num;
int gCollision_detection_on;
int gControl;
br_vector3 gGround_normal;
int gMetal_scrape_sound_id[3];
br_angle gOld_yaw_oppo; // Added _oppo suffix

// IDA: void __usercall MakeCarStationary(tCar_spec *pCar_spec@<EAX>)
void MakeCarStationary(tCar_spec* pCar_spec) {
    LOG_TRACE("(%p)", pCar_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MoveThisCar(tU32 pTime_difference@<EAX>, tCar_spec *car@<EDX>)
void MoveThisCar(tU32 pTime_difference, tCar_spec* car) {
    br_scalar dt;
    br_scalar ts;
    br_vector3 r;
    br_vector3 minus_k;
    int i;
    int j;
    br_angle phi;
    LOG_TRACE("(%d, %p)", pTime_difference, car);
    NOT_IMPLEMENTED();
}
