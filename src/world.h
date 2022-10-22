/*******************************************************************************
** world.h (the world of right now)
*******************************************************************************/

#ifndef WORLD_H
#define WORLD_H

#include "thing.h"

#define WORLD_MAX_THINGS 120

#define WORLD_VAMPIRE_INDEX 119

#define WORLD_BOXES_ETC_START_INDEX 0
#define WORLD_DENIZENS_START_INDEX  64
#define WORLD_PICKUPS_START_INDEX   88
#define WORLD_SPELLS_START_INDEX    104

#define WORLD_BOXES_ETC_END_INDEX   (WORLD_BOXES_ETC_START_INDEX + 64)
#define WORLD_DENIZENS_END_INDEX    (WORLD_DENIZENS_START_INDEX + 24)
#define WORLD_PICKUPS_END_INDEX     (WORLD_PICKUPS_START_INDEX + 16)
#define WORLD_SPELLS_END_INDEX      (WORLD_SPELLS_START_INDEX + 16)

/* the room is 13x14 tiles (including the walls), with each tile being 16x16 */
#define WORLD_WIDTH_IN_SUBPIXELS  (208 * THING_NUM_SUBPIXELS)
#define WORLD_HEIGHT_IN_SUBPIXELS (224 * THING_NUM_SUBPIXELS)

extern thing  G_world_all_things[WORLD_MAX_THINGS];

/* function declarations */
short int world_reset();

short int world_spawn_thing(int type, int facing, 
                            int pos_x, int pos_y, int vel_x, int vel_y, 
                            unsigned int timer_offset);
short int world_despawn_thing(int index);

short int world_load_things_from_tilemap();

#endif
