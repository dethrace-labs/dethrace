#ifndef _OPPOCAR_H_
#define _OPPOCAR_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 89
// EAX: pCar_spec
void MakeCarStationary(tCar_spec *pCar_spec);

// Offset: 92
// Size: 1323
// EAX: pTime_difference
// EDX: car
void MoveThisCar(tU32 pTime_difference, tCar_spec *car);

#endif
