#ifndef _OPPOCAR_H_
#define _OPPOCAR_H_

#include "br_types.h"
#include "dr_types.h"

void MakeCarStationary(tCar_spec* pCar_spec);

void MoveThisCar(tU32 pTime_difference, tCar_spec* car);

#endif
