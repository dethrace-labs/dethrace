#include "minimap.h"

#include "brender/brender.h"
#include "displays.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "network.h"
#include "opponent.h"
#include "pedestrn.h"
#include "trig.h"
#include "utility.h"

struct minimap minimap;

void init_minimap(int w, int h)
{
#if !defined(DETHRACE_FIX_BUGS)
	return;
#endif

	minimap.scale = gReal_graf_data_index + 1;

	minimap.map = BrPixelmapAllocate(gScreen->type,
					  w / minimap.scale,
					  h / minimap.scale,
					  NULL,
					  BR_PMAF_NORMAL);
	minimap.w = minimap.map->width;
	minimap.h = minimap.map->height;

	if (minimap.scale == 1) {
		minimap.map_scaled = minimap.map;
	} else {
		minimap.map_scaled = BrPixelmapAllocate(gScreen->type,
							w,
							h,
							NULL,
							BR_PMAF_NORMAL);
	}

	/* Hardcode these for now. */
	minimap.show_cp_line = 1;
	minimap.show_car_line = 1;
}

void dispose_minimap(void)
{
#if !defined(DETHRACE_FIX_BUGS)
	return;
#endif

	if (minimap.map_scaled != minimap.map)
		BrPixelmapFree(minimap.map_scaled);
	BrPixelmapFree(minimap.map);
}

static int minimap_in_bounds(int x, int y)
{
	return (x >= 0 && x < gCurrent_race.map_image->width &&
		y >= 0 && y < gCurrent_race.map_image->height);
}

static void minimap_checkpoint(tCheckpoint *cp, tU32 time)
{
	br_vector3 cp_pos;
	static tU32 last_flash;
	static int flash_state;
	int cp_x, cp_y, cp_l, cp_t, cp_r, cp_b, scr_x, scr_y;

	/* Dim the checkpoint box. */
	if (Flash(MINIMAP_CP_FLASH_PERIOD, &last_flash, &flash_state)) {
		cp_l = cp->map_left[0] - minimap.scroll_x;
		cp_t = cp->map_top[0] - minimap.scroll_y;
		cp_r = cp->map_right[0] - minimap.scroll_x;
		cp_b = cp->map_bottom[0] - minimap.scroll_y;

		/* Boundary check for the box. */
		if (cp_l < 0)
			cp_l = 0;
		else if (cp_l > minimap.w)
			cp_l = minimap.w;
		if (cp_r < 0)
			cp_r = 0;
		else if (cp_r > minimap.w)
			cp_r = minimap.w;
		if (cp_t < 0)
			cp_t = 0;
		else if (cp_t > minimap.h)
			cp_t = minimap.h;
		if (cp_b < 0)
			cp_b = 0;
		else if (cp_b > minimap.h)
			cp_b = minimap.h;

		DimRectangle(minimap.map, cp_l, cp_t, cp_r, cp_b, 0);
	}

	if (!minimap.show_cp_line ||
	    !minimap_in_bounds(minimap.map_x, minimap.map_y))
		return;

	/* Draw a line to the current checkpoint. */
	BrMatrix34ApplyP(&cp_pos,
			 cp->vertices[0],
			 &gCurrent_race.map_transformation);

	cp_x = cp_pos.v[0] - minimap.map_x;
	cp_y = cp_pos.v[1] - minimap.map_y;
	scr_x = minimap.map_x - minimap.scroll_x;
	scr_y = minimap.map_y - minimap.scroll_y;

	BrPixelmapLine(minimap.map,
		       scr_x,
		       scr_y,
		       scr_x + cp_x,
		       scr_y + cp_y,
		       MINIMAP_CP_LINE_COLOUR);
}

static void minimap_peds(tU32 time)
{
	br_vector3 ped_pos, map_pos;
	int ped_type, count, x, y, i;

	if (!gShow_peds_on_map || (time & MINIMAP_PED_FLASH_PERIOD))
		return;

	count = GetPedCount();

	for (i = 0; i < count; i++) {
		ped_type = GetPedPosition(i, &ped_pos);
		if (ped_type <= 0 && (gNet_mode == eNet_mode_none ||
		    !gCurrent_net_game->options.show_powerups_on_map))
			continue;

		BrMatrix34ApplyP(&map_pos,
				 &ped_pos,
				 &gCurrent_race.map_transformation);

		x = map_pos.v[0] - minimap.scroll_x;
		y = map_pos.v[1] - minimap.scroll_y;

		BrPixelmapPixelSet(minimap.map,
				   x,
				   y,
				   ped_type > 0 ? MINIMAP_PED_HUMAN_COLOUR :
						  MINIMAP_PED_OBJECT_COLOUR);
	}
}

static void minimap_arrow_blip(int pos_x, int pos_y, float bearing, int colour)
{
	int *arrow_ptr, arrow_idx, point_count;
	int colours[2];
	int px, py, i, j, x, y, tmp;

	arrow_idx = ((int)bearing) % 16;
	colours[0] = colour;
	colours[1] = OppositeColour(colour);

	for (i = 0; i < COUNT_OF(colours); i++) {
		point_count = gArrows[i][arrow_idx & 0x3][0];
		arrow_ptr = &gArrows[i][arrow_idx & 0x3][1];

		for (j = 0; j < point_count; j++, arrow_ptr += 2) {
			x = (arrow_idx & 0x8) ? -arrow_ptr[0] : arrow_ptr[0];
			y = (arrow_idx & 0x8) ? -arrow_ptr[1] : arrow_ptr[1];

			if (arrow_idx & 0x4) {
				tmp = x;
				x = -y;
				y = tmp;
			}

			px = pos_x + x;
			py = pos_y + y;

			/* Cull out of bounds pixels. */
			if (px < 0 || px >= minimap.w ||
			    py < 0 || py >= minimap.h)
				continue;

			BrPixelmapPixelSet(minimap.map, px, py, colours[i]);
		}
	}
}

static void minimap_opponents(tU32 time)
{
	br_vector3 *car_pos, map_pos;
	br_matrix34 *trans, car_in_map_space;
	tCar_spec *car;
	tU32 last_touched;
	float bearing;
	int colour, target, category, count, scr_x, scr_y, i;

	category = gShow_opponents ? eVehicle_opponent : eVehicle_self;

	/* Draw a line to the tracked opponent. */
	if (minimap.show_car_line &&
	    category == eVehicle_opponent &&
	    gNet_mode == eNet_mode_none &&
	    minimap_in_bounds(minimap.map_x, minimap.map_y)) {
		scr_x = minimap.map_x - minimap.scroll_x;
		scr_y = minimap.map_y - minimap.scroll_y;
		count = GetCarCount(category);

		for (i = 0, target = -1, last_touched = 0; i < count; i++) {
			car = GetCarSpec(category, i);
			if (car->knackered)
				continue;
			if (car->last_time_we_touched_a_player <= last_touched)
				continue;

			last_touched = car->last_time_we_touched_a_player;
			target = i;
		}

		if (target != -1) {
			car = GetCarSpec(category, target);
			car_pos = &car->car_master_actor->t.t.euler.t;
			trans = &car->car_master_actor->t.t.mat;
			BrMatrix34ApplyP(&map_pos,
					 car_pos,
					 &gCurrent_race.map_transformation);
			map_pos.v[0] -= minimap.map_x;
			map_pos.v[1] -= minimap.map_y;

			BrPixelmapLine(minimap.map,
				       scr_x,
				       scr_y,
				       scr_x + map_pos.v[0],
				       scr_y + map_pos.v[1],
				       MINIMAP_CAR_LINE_COLOUR);
		}
	}

	/* Do an ugly loop, decrementing through vehicle categories. */
	do {
		count = GetCarCount(category);

		for (i = 0; i < count; i++) {
			car = GetCarSpec(category, i);

			if (gNet_mode != eNet_mode_none &&
			    (car->knackered || NetPlayerFromCar(car)->wasted))
				    continue;

			/* Targeted vehicle's blip blinks. */
			if (category == eVehicle_opponent &&
			    i == target &&
			    !(time & MINIMAP_CAR_FLASH_PERIOD))
				continue;

			if (category)
				car_pos = &car->car_master_actor->t.t.euler.t;
			else
				car_pos = &gSelf->t.t.euler.t;
			trans = &car->car_master_actor->t.t.mat;

			BrMatrix34Mul(&car_in_map_space,
				      trans,
				      &gCurrent_race.map_transformation);
			bearing = FastScalarArcTan2(car_in_map_space.m[2][0],
						    car_in_map_space.m[2][1]);
			bearing = (360.f - bearing + 12.25) / 22.5f;

			BrMatrix34ApplyP(&map_pos,
					 car_pos,
					 &gCurrent_race.map_transformation);

			if (!minimap_in_bounds(map_pos.v[0], map_pos.v[1]))
				continue;

			map_pos.v[0] -= minimap.scroll_x;
			map_pos.v[1] -= minimap.scroll_y;
			colour = car->knackered ? 0 : gMap_colours[category];

			minimap_arrow_blip(map_pos.v[0],
					   map_pos.v[1],
					   bearing,
					   colour);
		}
	} while (--category >= eVehicle_self);

}

void display_minimap(tU32 time)
{
	br_vector3 *car_pos, map_pos;
	tCheckpoint *cp;
	int map_w, map_h;

	map_w = gCurrent_race.map_image->width;
	map_h = gCurrent_race.map_image->height;

	car_pos = &gSelf->t.t.euler.t;
	BrMatrix34ApplyP(&map_pos, car_pos, &gCurrent_race.map_transformation);
	minimap.map_x = map_pos.v[0];
	minimap.map_y = map_pos.v[1];
	minimap.scroll_x = minimap.map_x - minimap.w / 2;
	minimap.scroll_y = minimap.map_y - minimap.h / 2;

	/* Don't let the minimap scroll outside map boundaries. */
	if (minimap.scroll_x < 0) {
		minimap.scroll_x = 0;
	} else if (minimap.scroll_x >= gCurrent_race.map_image->width -
					minimap.w) {
		minimap.scroll_x = gCurrent_race.map_image->width -
				    minimap.w - 1;
	}
	if (minimap.scroll_y < 0) {
		minimap.scroll_y = 0;
	} else if (minimap.scroll_y >= gCurrent_race.map_image->height -
					minimap.h) {
		minimap.scroll_y = gCurrent_race.map_image->height -
				    minimap.h - 1;
	}

	/* Probably not needed, as we don't draw outside map boundaries. */
	BrPixelmapRectangleFill(minimap.map,
				0,
				0,
				minimap.w,
				minimap.h,
				0);

	DRPixelmapRectangleCopy(minimap.map,
				0,
				0,
				gCurrent_race.map_image,
				minimap.scroll_x,
				minimap.scroll_y,
				minimap.w,
				minimap.h);

	minimap_checkpoint(&gCurrent_race.checkpoints[gCheckpoint - 1], time);
	minimap_peds(time);
	minimap_opponents(time);

	if (minimap.scale > 1)
		DRPixelmapDoubledCopy(minimap.map_scaled,
				      minimap.map,
				      minimap.map->width,
				      minimap.map->height,
				      0,
				      0);
}
