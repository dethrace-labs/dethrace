#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "brender/brender.h"
#include "dr_types.h"

#define MINIMAP_CP_FLASH_PERIOD		300
#define MINIMAP_CP_LINE_COLOUR		255
#define MINIMAP_PED_FLASH_PERIOD	0x100
#define MINIMAP_PED_HUMAN_COLOUR	52
#define MINIMAP_PED_OBJECT_COLOUR	4
#define MINIMAP_CAR_FLASH_PERIOD	0x100
#define MINIMAP_CAR_LINE_COLOUR		52

extern struct minimap {
	br_pixelmap *map;
	br_pixelmap *map_scaled;
	int scale;
	int w;
	int h;
	int map_x;
	int map_y;
	int scroll_x;
	int scroll_y;
	int show;
	int show_cp_line;
	int show_car_line;
} minimap;

void init_minimap(int w, int h);
void dispose_minimap(void);
void display_minimap(tU32 time);

#endif /* _MINIMAP_H_ */
