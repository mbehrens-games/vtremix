/*******************************************************************************
** world.h (the world of right now)
*******************************************************************************/

#ifndef WORLD_H
#define WORLD_H

#include "thing.h"

/* things array start / end indices and counts */

/* note that the maximum number of vampires is 1, so  */
/* we add an additional 1 to the max things amount.   */
/* the maximum number of things should add up to 120! */
#define WORLD_MAX_BOXES_ETC 64
#define WORLD_MAX_DENIZENS  24
#define WORLD_MAX_PICKUPS   16
#define WORLD_MAX_SPELLS    15

#define WORLD_MAX_THINGS  ( WORLD_MAX_BOXES_ETC +                              \
                            WORLD_MAX_DENIZENS +                               \
                            WORLD_MAX_PICKUPS +                                \
                            WORLD_MAX_SPELLS + 1)

#define WORLD_BOXES_ETC_START_INDEX 0
#define WORLD_BOXES_ETC_END_INDEX   (WORLD_BOXES_ETC_START_INDEX + WORLD_MAX_BOXES_ETC)

#define WORLD_DENIZENS_START_INDEX  WORLD_BOXES_ETC_END_INDEX
#define WORLD_DENIZENS_END_INDEX    (WORLD_DENIZENS_START_INDEX + WORLD_MAX_DENIZENS)

#define WORLD_PICKUPS_START_INDEX   WORLD_DENIZENS_END_INDEX
#define WORLD_PICKUPS_END_INDEX     (WORLD_PICKUPS_START_INDEX + WORLD_MAX_PICKUPS)

#define WORLD_SPELLS_START_INDEX    WORLD_PICKUPS_END_INDEX
#define WORLD_SPELLS_END_INDEX      (WORLD_SPELLS_START_INDEX + WORLD_MAX_SPELLS)

#define WORLD_VAMPIRE_INDEX         WORLD_SPELLS_END_INDEX

extern thing  G_world_all_things[WORLD_MAX_THINGS];

/* function declarations */
short int world_reset();

short int world_spawn_thing(int type, int facing, 
                            int pos_x, int pos_y, int vel_x, int vel_y, 
                            unsigned int timer_offset);
short int world_despawn_thing(int index);

short int world_load_things_from_tilemap();

#endif
