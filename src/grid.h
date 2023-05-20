/*******************************************************************************
** grid.h (spatial partitioning grid and collision checking)
*******************************************************************************/

#ifndef GRID_H
#define GRID_H

#include "subpixel.h"

enum
{
  GRID_OBJECT_NONE = 0,
  GRID_OBJECT_BLOCK,
  GRID_OBJECT_PILLAR,
  GRID_OBJECT_WATER,
  GRID_OBJECT_BRIDGE_VERT,
  GRID_OBJECT_BRIDGE_HORI,
  GRID_OBJECT_BRIDGE_VERT_COLLAPSING,
  GRID_OBJECT_BRIDGE_HORI_COLLAPSING,
  GRID_OBJECT_PADLOCK,
  GRID_OBJECT_PADLOCK_OPENING,
  GRID_OBJECT_SPIKE_DROPPER_SPAWNER,
  GRID_OBJECT_SPIKE_RAISER_SPAWNER,
  GRID_OBJECT_ARROW_SPINNER_CCW_SPAWNER,
  GRID_OBJECT_ARROW_SPINNER_CW_SPAWNER,
  GRID_OBJECT_ARROWS_RIGHT,
  GRID_OBJECT_ARROWS_UP,
  GRID_OBJECT_ARROWS_LEFT,
  GRID_OBJECT_ARROWS_DOWN,
  GRID_OBJECT_FLOOR_SPIKES_DOWN,
  GRID_OBJECT_FLOOR_SPIKES_UP,
  GRID_OBJECT_AUDREY_SPAWNER,
  GRID_OBJECT_WALL,
  GRID_OBJECT_PORTCULLIS_CLOSED,
  GRID_OBJECT_PORTCULLIS_OPEN,
  GRID_OBJECT_SUBMERGED_BOX_WOOD,
  GRID_OBJECT_SUBMERGED_BOX_METAL,
  GRID_OBJECT_SUBMERGED_BOX_STONE,
  GRID_OBJECT_SUBMERGED_ICE_CUBE,
  GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING
};

#define GRID_OBJECT_WILL_BLOCK_THE_VAMPIRE(b)                                  \
  ( (b->object == GRID_OBJECT_BLOCK)                            ||             \
    (b->object == GRID_OBJECT_PILLAR)                           ||             \
    (b->object == GRID_OBJECT_WATER)                            ||             \
    (b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING)           ||             \
    (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING)           ||             \
    (b->object == GRID_OBJECT_PADLOCK)                          ||             \
    (b->object == GRID_OBJECT_FLOOR_SPIKES_UP)                  ||             \
    (b->object == GRID_OBJECT_WALL)                             ||             \
    (b->object == GRID_OBJECT_PORTCULLIS_CLOSED)                ||             \
    ( (b->object == GRID_OBJECT_BRIDGE_VERT) &&                                \
      (b->state == STATE_READY_TO_CHANGE))                      ||             \
    ( (b->object == GRID_OBJECT_BRIDGE_HORI) &&                                \
      (b->state == STATE_READY_TO_CHANGE))                      ||             \
    ( (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING) &&                 \
      (b->state == STATE_READY_TO_CHANGE))                      ||             \
    ( (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN) &&                          \
      (b->state == STATE_READY_TO_CHANGE)))

#define GRID_OBJECT_WILL_BLOCK_THE_BAT(b)                                      \
  ( (b->object == GRID_OBJECT_BLOCK)    ||                                     \
    (b->object == GRID_OBJECT_PILLAR)   ||                                     \
    (b->object == GRID_OBJECT_PADLOCK)  ||                                     \
    (b->object == GRID_OBJECT_WALL)     ||                                     \
    (b->object == GRID_OBJECT_PORTCULLIS_CLOSED))

#define GRID_OBJECT_WILL_BLOCK_WALKING_DENIZENS(b)                             \
  ( GRID_OBJECT_WILL_BLOCK_THE_VAMPIRE(b) ||                                   \
    (b->object == GRID_OBJECT_PORTCULLIS_OPEN))

#define GRID_OBJECT_WILL_BLOCK_FLYING_DENIZENS(b)                              \
  ( GRID_OBJECT_WILL_BLOCK_THE_BAT(b) ||                                       \
    (b->object == GRID_OBJECT_PORTCULLIS_OPEN))

#define GRID_OBJECT_WILL_BLOCK_SPIKE_BREAKING_PUSHABLE_OBJECTS(b)              \
  ( (b->object == GRID_OBJECT_BLOCK)              ||                           \
    (b->object == GRID_OBJECT_PILLAR)             ||                           \
    (b->object == GRID_OBJECT_PADLOCK)            ||                           \
    (b->object == GRID_OBJECT_WALL)               ||                           \
    (b->object == GRID_OBJECT_PORTCULLIS_CLOSED)  ||                           \
    (b->object == GRID_OBJECT_PORTCULLIS_OPEN))

#define GRID_OBJECT_WILL_BLOCK_NON_SPIKE_BREAKING_PUSHABLE_OBJECTS(b)          \
  ( GRID_OBJECT_WILL_BLOCK_SPIKE_BREAKING_PUSHABLE_OBJECTS(b) ||               \
    (b->object == GRID_OBJECT_FLOOR_SPIKES_UP)                ||               \
    ( (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN) &&                          \
      (b->state == STATE_READY_TO_CHANGE)))

#define GRID_OBJECT_WILL_BLOCK_EVIL_EYE_SPELLS(b)                              \
  ( (b->object == GRID_OBJECT_BLOCK)              ||                           \
    (b->object == GRID_OBJECT_PADLOCK)            ||                           \
    (b->object == GRID_OBJECT_WALL)               ||                           \
    (b->object == GRID_OBJECT_PORTCULLIS_CLOSED)  ||                           \
    (b->object == GRID_OBJECT_PORTCULLIS_OPEN))

#define GRID_OBJECT_WILL_BLOCK_OTHER_SPELLS(b)                                 \
  ( (b->object == GRID_OBJECT_BLOCK)              ||                           \
    (b->object == GRID_OBJECT_PILLAR)             ||                           \
    (b->object == GRID_OBJECT_PADLOCK)            ||                           \
    (b->object == GRID_OBJECT_WALL)               ||                           \
    (b->object == GRID_OBJECT_PORTCULLIS_CLOSED)  ||                           \
    (b->object == GRID_OBJECT_PORTCULLIS_OPEN))

#define GRID_OBJECT_WILL_NOT_BLOCK_THE_VAMPIRE(b)                              \
  (!GRID_OBJECT_WILL_BLOCK_THE_VAMPIRE(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_THE_BAT(b)                                  \
  (!GRID_OBJECT_WILL_BLOCK_THE_BAT(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_WALKING_DENIZENS(b)                         \
  (!GRID_OBJECT_WILL_BLOCK_WALKING_DENIZENS(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_FLYING_DENIZENS(b)                          \
  (!GRID_OBJECT_WILL_BLOCK_FLYING_DENIZENS(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_SPIKE_BREAKING_PUSHABLE_OBJECTS(b)          \
  (!GRID_OBJECT_WILL_BLOCK_SPIKE_BREAKING_PUSHABLE_OBJECTS(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_NON_SPIKE_BREAKING_PUSHABLE_OBJECTS(b)      \
  (!GRID_OBJECT_WILL_BLOCK_NON_SPIKE_BREAKING_PUSHABLE_OBJECTS(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_EVIL_EYE_SPELLS(b)                          \
  (!GRID_OBJECT_WILL_BLOCK_EVIL_EYE_SPELLS(b))

#define GRID_OBJECT_WILL_NOT_BLOCK_OTHER_SPELLS(b)                             \
  (!GRID_OBJECT_WILL_BLOCK_OTHER_SPELLS(b))

enum
{
  GRID_MOVE_THING_MODE_HORIZONTAL,
  GRID_MOVE_THING_MODE_VERTICAL,
  GRID_MOVE_THING_MODE_TEST_POSITION
};

/* the size of each room is 13x14 tiles, including      */
/* the outer walls (it is 11x11 for the playable area)  */
#define GRID_WIDTH_IN_BOXES   13
#define GRID_HEIGHT_IN_BOXES  14
#define GRID_NUM_BOXES        (GRID_WIDTH_IN_BOXES * GRID_HEIGHT_IN_BOXES)

#define GRID_BOX_SIZE_IN_PIXELS       16
#define GRID_BOX_SIZE_IN_PIXELS_HALF  (GRID_BOX_SIZE_IN_PIXELS / 2)

#define GRID_BOX_SIZE_IN_SUBPIXELS       (GRID_BOX_SIZE_IN_PIXELS * SUBPIXEL_MANTISSA_FULL)
#define GRID_BOX_SIZE_IN_SUBPIXELS_HALF  (GRID_BOX_SIZE_IN_SUBPIXELS / 2)

#define GRID_WIDTH_IN_PIXELS  (GRID_WIDTH_IN_BOXES * GRID_BOX_SIZE_IN_PIXELS)
#define GRID_HEIGHT_IN_PIXELS (GRID_HEIGHT_IN_BOXES * GRID_BOX_SIZE_IN_PIXELS)

#define GRID_WIDTH_IN_SUBPIXELS   (GRID_WIDTH_IN_BOXES * GRID_BOX_SIZE_IN_SUBPIXELS)
#define GRID_HEIGHT_IN_SUBPIXELS  (GRID_HEIGHT_IN_BOXES * GRID_BOX_SIZE_IN_SUBPIXELS)

#define GRID_BOX_MAX_THINGS 120

typedef struct grid_box
{
  int           object;
  int           state;
  unsigned int  timer_offset;

  int           thing_indices[GRID_BOX_MAX_THINGS];
  int           num_things;
} grid_box;

extern grid_box G_collision_grid[GRID_NUM_BOXES];

/* function declarations */
short int grid_reset();

short int grid_add_thing(int index);
short int grid_remove_thing(int index);

short int grid_move_thing(int index, int mode, int amount);

short int grid_load_objects_from_tilemap();

#endif
