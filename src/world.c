/*******************************************************************************
** world.c (the world of right now)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "animate.h"
#include "global.h"
#include "grid.h"
#include "level.h"
#include "state.h"
#include "thing.h"
#include "world.h"

thing G_world_all_things[WORLD_MAX_THINGS];

/*******************************************************************************
** world_reset()
*******************************************************************************/
short int world_reset()
{
  int i;

  thing* t;

  /* reset all things */
  for (i = 0; i < WORLD_MAX_THINGS; i++)
  {
    t = &G_world_all_things[i];

    t->type = THING_TYPE_NONE;
    t->state = STATE_NONE;
    t->facing = THING_FACING_RIGHT;

    t->freeze_type = THING_TYPE_NONE;
    t->freeze_facing = THING_FACING_RIGHT;
    t->freeze_timer_offset = 0;

    t->flag = THING_COLLISION_FLAG_NONE;

    t->pos_x = 0;
    t->pos_y = 0;

    t->vel_x = 0;
    t->vel_y = 0;

    t->hx = 0;
    t->hy = 0;

    t->timer_offset = 0;
  }

  return 0;
}

/*******************************************************************************
** world_spawn_thing()
*******************************************************************************/
short int world_spawn_thing(int type, int facing, 
                            int pos_x, int pos_y, int vel_x, int vel_y, 
                            unsigned int timer_offset)
{
  int i;

  thing* t;

  int start_index;
  int end_index;
  int insert_index;

  /* determine start & end indices */
  if ((type == THING_TYPE_VAMPIRE) || 
      (type == THING_TYPE_BAT))
  {
    start_index = WORLD_VAMPIRE_INDEX;
    end_index = WORLD_VAMPIRE_INDEX + 1;
  }
  else if ( (type == THING_TYPE_BOX_WOOD)   || 
            (type == THING_TYPE_BOX_METAL)  || 
            (type == THING_TYPE_BOX_STONE)  || 
            (type == THING_TYPE_POT_WOOD)   || 
            (type == THING_TYPE_POT_METAL)  || 
            (type == THING_TYPE_POT_STONE)  || 
            (type == THING_TYPE_ACID_FLASK) || 
            (type == THING_TYPE_STONE_FLASK))
  {
    start_index = WORLD_BOXES_ETC_START_INDEX;
    end_index = WORLD_BOXES_ETC_END_INDEX;
  }
  else if ( (type == THING_TYPE_PUMPKIN)                || 
            (type == THING_TYPE_AUDREY)                 || 
            (type == THING_TYPE_GHOST_ASLEEP)           || 
            (type == THING_TYPE_GHOST_AWAKE)            || 
            (type == THING_TYPE_SNAKE_ASLEEP)           || 
            (type == THING_TYPE_SNAKE_AWAKE)            || 
            (type == THING_TYPE_SHADE)                  || 
            (type == THING_TYPE_WITCH_ASLEEP)           || 
            (type == THING_TYPE_WITCH_AWAKE)            || 
            (type == THING_TYPE_SORCERESS)              || 
            (type == THING_TYPE_IMP_ASLEEP)             || 
            (type == THING_TYPE_IMP_AWAKE)              || 
            (type == THING_TYPE_LIL_BOT)                || 
            (type == THING_TYPE_FAERIE_HORIZONTAL)      || 
            (type == THING_TYPE_FAERIE_VERTICAL)        || 
            (type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
            (type == THING_TYPE_DOUBLE_NORMAL_BAT)      || 
            (type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
            (type == THING_TYPE_DOUBLE_REVERSE_BAT)     || 
            (type == THING_TYPE_WISP_RT)                || 
            (type == THING_TYPE_WISP_LT)                || 
            (type == THING_TYPE_KNIGHT_RT)              || 
            (type == THING_TYPE_KNIGHT_LT)              || 
            (type == THING_TYPE_MUMMY_RT)               || 
            (type == THING_TYPE_MUMMY_LT)               || 
            (type == THING_TYPE_JIANGSHI_RT)            || 
            (type == THING_TYPE_JIANGSHI_LT))
  {
    start_index = WORLD_DENIZENS_START_INDEX;
    end_index = WORLD_DENIZENS_END_INDEX;
  }
  else if ( (type == THING_TYPE_BLOOD_VIAL)         || 
            (type == THING_TYPE_BAT_VIAL)           || 
            (type == THING_TYPE_MANA_PRISM)         || 
            (type == THING_TYPE_SKELETON_KEY)       || 
            (type == THING_TYPE_SPIKE_DROPPER)      || 
            (type == THING_TYPE_SPIKE_RAISER)       || 
            (type == THING_TYPE_ARROW_SPINNER_CCW)  || 
            (type == THING_TYPE_ARROW_SPINNER_CW))
  {
    start_index = WORLD_PICKUPS_START_INDEX;
    end_index = WORLD_PICKUPS_END_INDEX;
  }
  else if ( (type == THING_TYPE_EVIL_EYE_SPELL) || 
            (type == THING_TYPE_FIRE_SPELL)     || 
            (type == THING_TYPE_ICE_SPELL)      || 
            (type == THING_TYPE_RING_SPELL)     || 
            (type == THING_TYPE_TWIN_SPELL))
  {
    start_index = WORLD_SPELLS_START_INDEX;
    end_index = WORLD_SPELLS_END_INDEX;
  }
  else
    return 1;

  /* find open spot, if possible */
  insert_index = -1;

  for (i = start_index; i < end_index; i++)
  {
    if (G_world_all_things[i].type == THING_TYPE_NONE)
    {
      insert_index = i;
      break;
    }
  }

  /* insert this thing into the world */
  if (insert_index == -1)
    return 1;

  t = &G_world_all_things[insert_index];

  /* set type */
  t->type = type;

  /* set state */
  t->state = STATE_NONE;

  /* set facing */
  /* note that the passed in argument is only used in */
  /* the case of the player, denizens, and spells.    */
  /* otherwise, it is ignored.                        */
  if ((type == THING_TYPE_VAMPIRE)                || 
      (type == THING_TYPE_BAT)                    || 
      (type == THING_TYPE_EVIL_EYE_SPELL)         || 
      (type == THING_TYPE_FIRE_SPELL)             || 
      (type == THING_TYPE_ICE_SPELL)              || 
      (type == THING_TYPE_RING_SPELL)             || 
      (type == THING_TYPE_TWIN_SPELL)             || 
      (type == THING_TYPE_PUMPKIN)                || 
      (type == THING_TYPE_AUDREY)                 || 
      (type == THING_TYPE_GHOST_ASLEEP)           || 
      (type == THING_TYPE_GHOST_AWAKE)            || 
      (type == THING_TYPE_SNAKE_ASLEEP)           || 
      (type == THING_TYPE_SNAKE_AWAKE)            || 
      (type == THING_TYPE_SHADE)                  || 
      (type == THING_TYPE_WITCH_ASLEEP)           || 
      (type == THING_TYPE_WITCH_AWAKE)            || 
      (type == THING_TYPE_SORCERESS)              || 
      (type == THING_TYPE_IMP_ASLEEP)             || 
      (type == THING_TYPE_IMP_AWAKE)              || 
      (type == THING_TYPE_LIL_BOT)                || 
      (type == THING_TYPE_FAERIE_HORIZONTAL)      || 
      (type == THING_TYPE_FAERIE_VERTICAL)        || 
      (type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
      (type == THING_TYPE_DOUBLE_NORMAL_BAT)      || 
      (type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
      (type == THING_TYPE_DOUBLE_REVERSE_BAT)     || 
      (type == THING_TYPE_WISP_RT)                || 
      (type == THING_TYPE_WISP_LT)                || 
      (type == THING_TYPE_KNIGHT_RT)              || 
      (type == THING_TYPE_KNIGHT_LT)              || 
      (type == THING_TYPE_MUMMY_RT)               || 
      (type == THING_TYPE_MUMMY_LT)               || 
      (type == THING_TYPE_JIANGSHI_RT)            || 
      (type == THING_TYPE_JIANGSHI_LT))
  {
    if ((facing == THING_FACING_RIGHT)  || 
        (facing == THING_FACING_UP)     || 
        (facing == THING_FACING_LEFT)   || 
        (facing == THING_FACING_DOWN))
    {
      t->facing = facing;
    }
    else
      t->facing = THING_FACING_RIGHT;
  }
  else
    t->facing = THING_FACING_RIGHT;

  /* set hitbox size based on type */
  if ((type == THING_TYPE_VAMPIRE)                || 
      (type == THING_TYPE_BAT)                    || 
      (type == THING_TYPE_EVIL_EYE_SPELL)         || 
      (type == THING_TYPE_FIRE_SPELL)             || 
      (type == THING_TYPE_ICE_SPELL)              || 
      (type == THING_TYPE_RING_SPELL)             || 
      (type == THING_TYPE_TWIN_SPELL)             || 
      (type == THING_TYPE_PUMPKIN)                || 
      (type == THING_TYPE_AUDREY)                 || 
      (type == THING_TYPE_GHOST_ASLEEP)           || 
      (type == THING_TYPE_GHOST_AWAKE)            || 
      (type == THING_TYPE_SNAKE_ASLEEP)           || 
      (type == THING_TYPE_SNAKE_AWAKE)            || 
      (type == THING_TYPE_SHADE)                  || 
      (type == THING_TYPE_WITCH_ASLEEP)           || 
      (type == THING_TYPE_WITCH_AWAKE)            || 
      (type == THING_TYPE_SORCERESS)              || 
      (type == THING_TYPE_IMP_ASLEEP)             || 
      (type == THING_TYPE_IMP_AWAKE)              || 
      (type == THING_TYPE_LIL_BOT)                || 
      (type == THING_TYPE_FAERIE_HORIZONTAL)      || 
      (type == THING_TYPE_FAERIE_VERTICAL)        || 
      (type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
      (type == THING_TYPE_DOUBLE_NORMAL_BAT)      || 
      (type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
      (type == THING_TYPE_DOUBLE_REVERSE_BAT)     || 
      (type == THING_TYPE_WISP_RT)                || 
      (type == THING_TYPE_WISP_LT)                || 
      (type == THING_TYPE_KNIGHT_RT)              || 
      (type == THING_TYPE_KNIGHT_LT)              || 
      (type == THING_TYPE_MUMMY_RT)               || 
      (type == THING_TYPE_MUMMY_LT)               || 
      (type == THING_TYPE_JIANGSHI_RT)            || 
      (type == THING_TYPE_JIANGSHI_LT)            || 
      (type == THING_TYPE_ICE_CUBE)               || 
      (type == THING_TYPE_ICE_CUBE_THAWING)       || 
      (type == THING_TYPE_BOX_WOOD)               || 
      (type == THING_TYPE_BOX_METAL)              || 
      (type == THING_TYPE_BOX_STONE)              || 
      (type == THING_TYPE_POT_WOOD)               || 
      (type == THING_TYPE_POT_METAL)              || 
      (type == THING_TYPE_POT_STONE)              || 
      (type == THING_TYPE_ACID_FLASK)             || 
      (type == THING_TYPE_STONE_FLASK))
  {
    t->hx = 8 * THING_NUM_SUBPIXELS;
    t->hy = 8 * THING_NUM_SUBPIXELS;
  }
  else if ( (type == THING_TYPE_BLOOD_VIAL)         || 
            (type == THING_TYPE_BAT_VIAL)           || 
            (type == THING_TYPE_MANA_PRISM)         || 
            (type == THING_TYPE_SKELETON_KEY)       || 
            (type == THING_TYPE_SPIKE_DROPPER)      || 
            (type == THING_TYPE_SPIKE_RAISER)       || 
            (type == THING_TYPE_ARROW_SPINNER_CCW)  || 
            (type == THING_TYPE_ARROW_SPINNER_CW))
  {
    t->hx = 5 * THING_NUM_SUBPIXELS;
    t->hy = 7 * THING_NUM_SUBPIXELS;
  }
  else
  {
    t->hx = 8 * THING_NUM_SUBPIXELS;
    t->hy = 8 * THING_NUM_SUBPIXELS;
  }

  t->freeze_type = THING_TYPE_NONE;
  t->freeze_facing = THING_FACING_RIGHT;
  t->freeze_timer_offset = 0;

  t->flag = THING_COLLISION_FLAG_NONE;

  t->pos_x = pos_x;
  t->pos_y = pos_y;
  t->vel_x = vel_x;
  t->vel_y = vel_y;

  t->timer_offset = timer_offset;

  /* add this thing to the grid */
  grid_add_thing(insert_index);

  return 0;
}

/*******************************************************************************
** world_despawn_thing()
*******************************************************************************/
short int world_despawn_thing(int index)
{
  thing* t;

  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  t = &G_world_all_things[index];

  if (t->type == THING_TYPE_NONE)
    return 0;

  /* remove this thing from the grid */
  grid_remove_thing(index);

  /* reset this thing */
  t->type   = THING_TYPE_NONE;
  t->state  = STATE_NONE;
  t->facing = THING_FACING_RIGHT;

  t->freeze_type = THING_TYPE_NONE;
  t->freeze_facing = THING_FACING_RIGHT;
  t->freeze_timer_offset = 0;

  t->flag   = THING_COLLISION_FLAG_NONE;

  t->pos_x  = 0;
  t->pos_y  = 0;

  t->vel_x  = 0;
  t->vel_y  = 0;

  t->hx     = 0;
  t->hy     = 0;

  t->timer_offset = 0;

  return 0;
}

/*******************************************************************************
** world_load_things_from_tilemap()
*******************************************************************************/
short int world_load_things_from_tilemap()
{
  int m;
  int n;

  int type;
  int facing;

  int pos_x;
  int pos_y;

  int vel_x;
  int vel_y;

  unsigned int timer_offset;

  unsigned char* room_tilemap;

  /* set room tilemap */
  if ((G_current_room >= 0) && (G_current_room < LEVEL_NUM_ROOMS))
    room_tilemap = &G_room_tilemaps[G_current_room][0];
  else
    room_tilemap = &G_room_tilemaps[0][0];

  /* reset world */
  world_reset();

  /* reset globals */
  G_vials_remaining = 0;
  G_portcullis_reached = 0;
  G_player_said_bye_bye = 0;

  G_num_bat_vials = 0;
  G_num_mana_prisms = 0;

  G_num_audrey_respawns = 0;

  /* load room tilemap to world */
  for (n = 0; n < LEVEL_ROOM_HEIGHT; n++)
  {
    for (m = 0; m < LEVEL_ROOM_WIDTH; m++)
    {
      /* initialize type, state and facing */
      type = THING_TYPE_NONE;
      facing = THING_FACING_RIGHT;

      /* determine position */
      pos_x = (LEVEL_ROOM_TILE_SIZE * (m + 1) + LEVEL_ROOM_TILE_SIZE_HALF) * THING_NUM_SUBPIXELS;
      pos_y = (LEVEL_ROOM_TILE_SIZE * (n + 2) + LEVEL_ROOM_TILE_SIZE_HALF) * THING_NUM_SUBPIXELS;

      /* initialize velocity */
      vel_x = 0;
      vel_y = 0;

      /* determine thing type and starting velocity */
      if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_PLAYER_START)
      {
        type = THING_TYPE_VAMPIRE;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BLOOD_VIAL)
        type = THING_TYPE_BLOOD_VIAL;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BAT_VIAL)
        type = THING_TYPE_BAT_VIAL;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MANA_PRISM)
        type = THING_TYPE_MANA_PRISM;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SKELETON_KEY)
        type = THING_TYPE_SKELETON_KEY;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SPIKE_DROPPER)
        type = THING_TYPE_SPIKE_DROPPER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SPIKE_RAISER)
        type = THING_TYPE_SPIKE_RAISER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROW_SPINNER_CCW)
        type = THING_TYPE_ARROW_SPINNER_CCW;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROW_SPINNER_CW)
        type = THING_TYPE_ARROW_SPINNER_CW;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BOX_WOOD)
        type = THING_TYPE_BOX_WOOD;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BOX_METAL)
        type = THING_TYPE_BOX_METAL;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BOX_STONE)
        type = THING_TYPE_BOX_STONE;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_POT_WOOD)
        type = THING_TYPE_POT_WOOD;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_POT_METAL)
        type = THING_TYPE_POT_METAL;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_POT_STONE)
        type = THING_TYPE_POT_STONE;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ACID_FLASK)
        type = THING_TYPE_ACID_FLASK;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_STONE_FLASK)
        type = THING_TYPE_STONE_FLASK;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_PUMPKIN)
        type = THING_TYPE_PUMPKIN;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_AUDREY)
        type = THING_TYPE_AUDREY;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_GHOST_RIGHT)
      {
        type = THING_TYPE_GHOST_ASLEEP;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_GHOST_UP)
      {
        type = THING_TYPE_GHOST_ASLEEP;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_GHOST_LEFT)
      {
        type = THING_TYPE_GHOST_ASLEEP;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_GHOST_DOWN)
      {
        type = THING_TYPE_GHOST_ASLEEP;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SNAKE_RIGHT)
      {
        type = THING_TYPE_SNAKE_ASLEEP;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SNAKE_UP)
      {
        type = THING_TYPE_SNAKE_ASLEEP;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SNAKE_LEFT)
      {
        type = THING_TYPE_SNAKE_ASLEEP;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SNAKE_DOWN)
      {
        type = THING_TYPE_SNAKE_ASLEEP;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SHADE_RIGHT)
      {
        type = THING_TYPE_SHADE;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SHADE_UP)
      {
        type = THING_TYPE_SHADE;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SHADE_LEFT)
      {
        type = THING_TYPE_SHADE;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SHADE_DOWN)
      {
        type = THING_TYPE_SHADE;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WITCH_RIGHT)
      {
        type = THING_TYPE_WITCH_ASLEEP;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WITCH_UP)
      {
        type = THING_TYPE_WITCH_ASLEEP;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WITCH_LEFT)
      {
        type = THING_TYPE_WITCH_ASLEEP;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WITCH_DOWN)
      {
        type = THING_TYPE_WITCH_ASLEEP;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SORCERESS_RIGHT)
      {
        type = THING_TYPE_SORCERESS;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SORCERESS_UP)
      {
        type = THING_TYPE_SORCERESS;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SORCERESS_LEFT)
      {
        type = THING_TYPE_SORCERESS;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SORCERESS_DOWN)
      {
        type = THING_TYPE_SORCERESS;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_IMP_RIGHT)
      {
        type = THING_TYPE_IMP_ASLEEP;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_IMP_UP)
      {
        type = THING_TYPE_IMP_ASLEEP;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_IMP_LEFT)
      {
        type = THING_TYPE_IMP_ASLEEP;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_IMP_DOWN)
      {
        type = THING_TYPE_IMP_ASLEEP;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_LIL_BOT_RIGHT)
      {
        type = THING_TYPE_LIL_BOT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_LIL_BOT_UP)
      {
        type = THING_TYPE_LIL_BOT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_LIL_BOT_LEFT)
      {
        type = THING_TYPE_LIL_BOT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_LIL_BOT_DOWN)
      {
        type = THING_TYPE_LIL_BOT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_FAERIE_HORIZONTAL)
      {
        type = THING_TYPE_FAERIE_HORIZONTAL;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_FAERIE_VERTICAL)
      {
        type = THING_TYPE_FAERIE_VERTICAL;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_DOUBLE_NORMAL)
      {
        type = THING_TYPE_DOUBLE_NORMAL_VAMPIRE;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_DOUBLE_REVERSE)
      {
        type = THING_TYPE_DOUBLE_REVERSE_VAMPIRE;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_RT_RIGHT)
      {
        type = THING_TYPE_WISP_RT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_RT_UP)
      {
        type = THING_TYPE_WISP_RT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_RT_LEFT)
      {
        type = THING_TYPE_WISP_RT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_RT_DOWN)
      {
        type = THING_TYPE_WISP_RT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_LT_RIGHT)
      {
        type = THING_TYPE_WISP_LT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_LT_UP)
      {
        type = THING_TYPE_WISP_LT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_LT_LEFT)
      {
        type = THING_TYPE_WISP_LT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WISP_LT_DOWN)
      {
        type = THING_TYPE_WISP_LT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_RT_RIGHT)
      {
        type = THING_TYPE_KNIGHT_RT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_RT_UP)
      {
        type = THING_TYPE_KNIGHT_RT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_RT_LEFT)
      {
        type = THING_TYPE_KNIGHT_RT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_RT_DOWN)
      {
        type = THING_TYPE_KNIGHT_RT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_LT_RIGHT)
      {
        type = THING_TYPE_KNIGHT_LT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_LT_UP)
      {
        type = THING_TYPE_KNIGHT_LT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_LT_LEFT)
      {
        type = THING_TYPE_KNIGHT_LT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_KNIGHT_LT_DOWN)
      {
        type = THING_TYPE_KNIGHT_LT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_RT_RIGHT)
      {
        type = THING_TYPE_MUMMY_RT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_RT_UP)
      {
        type = THING_TYPE_MUMMY_RT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_RT_LEFT)
      {
        type = THING_TYPE_MUMMY_RT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_RT_DOWN)
      {
        type = THING_TYPE_MUMMY_RT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_LT_RIGHT)
      {
        type = THING_TYPE_MUMMY_LT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_LT_UP)
      {
        type = THING_TYPE_MUMMY_LT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_LT_LEFT)
      {
        type = THING_TYPE_MUMMY_LT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MUMMY_LT_DOWN)
      {
        type = THING_TYPE_MUMMY_LT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_RT_RIGHT)
      {
        type = THING_TYPE_JIANGSHI_RT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_RT_UP)
      {
        type = THING_TYPE_JIANGSHI_RT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_RT_LEFT)
      {
        type = THING_TYPE_JIANGSHI_RT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_RT_DOWN)
      {
        type = THING_TYPE_JIANGSHI_RT;
        facing = THING_FACING_DOWN;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_LT_RIGHT)
      {
        type = THING_TYPE_JIANGSHI_LT;
        facing = THING_FACING_RIGHT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_LT_UP)
      {
        type = THING_TYPE_JIANGSHI_LT;
        facing = THING_FACING_UP;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_LT_LEFT)
      {
        type = THING_TYPE_JIANGSHI_LT;
        facing = THING_FACING_LEFT;
      }
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_JIANGSHI_LT_DOWN)
      {
        type = THING_TYPE_JIANGSHI_LT;
        facing = THING_FACING_DOWN;
      }
      else
        continue;

      /* set timer offsets */
      if ((room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BLOOD_VIAL)        || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BAT_VIAL)          || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_MANA_PRISM)        || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SKELETON_KEY)      || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SPIKE_DROPPER)     || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SPIKE_RAISER)      || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROW_SPINNER_CCW) || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROW_SPINNER_CW))
      {
        timer_offset = 240 - (((n + m) % ANIM_PICKUP_NUM_FRAMES) * ANIM_PICKUP_FRAME_LENGTH);
      }
      else
      {
        timer_offset = 0;
      }

      /* spawn thing */
      world_spawn_thing(type, facing, 
                        pos_x, pos_y, vel_x, vel_y, timer_offset);

      /* if this is a vial, increment vials remaining count */
      if ((type == THING_TYPE_BLOOD_VIAL) || 
          (type == THING_TYPE_BAT_VIAL))
      {
        G_vials_remaining += 1;
      }
    }
  }

  return 0;
}

