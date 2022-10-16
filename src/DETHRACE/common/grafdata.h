#ifndef _GRAFDATA_H_
#define _GRAFDATA_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern tGraf_data gGraf_data[2];
extern tGraf_data* gCurrent_graf_data;
extern int gGraf_data_index;

void CalcGrafDataIndex(void);

#endif
