#ifndef _GRAFDATA_H_
#define _GRAFDATA_H_

#include "dr_types.h"
#include "br_types.h"

extern tGraf_data gGraf_data[2];
extern tGraf_data *gCurrent_graf_data;
extern int gGraf_data_index;

// Offset: 0
// Size: 170
void CalcGrafDataIndex();

#endif
