/*******************************************************************************
** grid.c (spatial partitioning grid and collision checking)
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

#define GRID_DETERMINE_OVERLAPPED_BOXES(t)                                     \
  /* determine corner positions (in subpixels) */                              \
  corner_low_x  = t->pos_x - t->hx;                                            \
  corner_high_x = t->pos_x + t->hx - 1;                                        \
  corner_low_y  = t->pos_y - t->hy;                                            \
  corner_high_y = t->pos_y + t->hy - 1;                                        \
                                                                               \
  /* bound corner positions */                                                 \
  if (corner_low_x < 0)                                                        \
    corner_low_x = 0;                                                          \
  else if (corner_low_x >= WORLD_WIDTH_IN_SUBPIXELS)                           \
    corner_low_x = WORLD_WIDTH_IN_SUBPIXELS - 1;                               \
                                                                               \
  if (corner_high_x < 0)                                                       \
    corner_high_x = 0;                                                         \
  else if (corner_high_x >= WORLD_WIDTH_IN_SUBPIXELS)                          \
    corner_high_x = WORLD_WIDTH_IN_SUBPIXELS - 1;                              \
                                                                               \
  if (corner_low_y < 0)                                                        \
    corner_low_y = 0;                                                          \
  else if (corner_low_y >= WORLD_HEIGHT_IN_SUBPIXELS)                          \
    corner_low_y = WORLD_HEIGHT_IN_SUBPIXELS - 1;                              \
                                                                               \
  if (corner_high_y < 0)                                                       \
    corner_high_y = 0;                                                         \
  else if (corner_high_y >= WORLD_HEIGHT_IN_SUBPIXELS)                         \
    corner_high_y = WORLD_HEIGHT_IN_SUBPIXELS - 1;                             \
                                                                               \
  /* determine boxes overlapped by this thing */                               \
  box_low_x   = corner_low_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);          \
  box_high_x  = corner_high_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);         \
  box_low_y   = corner_low_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);          \
  box_high_y  = corner_high_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);         \
                                                                               \
  /* determine number of grid boxes in each direction */                       \
  num_boxes_x = box_high_x - box_low_x + 1;                                    \
  num_boxes_y = box_high_y - box_low_y + 1;

#define GRID_COLLISION_TEST_THINGS(t1, t2)                                     \
  /* obtain x projections */                                                   \
  l[0] = t1->pos_x - t1->hx;                                                   \
  h[0] = t1->pos_x + t1->hx - 1;                                               \
                                                                               \
  l[1] = t2->pos_x - t2->hx;                                                   \
  h[1] = t2->pos_x + t2->hx - 1;                                               \
                                                                               \
  /* obtain y projections */                                                   \
  l[2] = t1->pos_y - t1->hy;                                                   \
  h[2] = t1->pos_y + t1->hy - 1;                                               \
                                                                               \
  l[3] = t2->pos_y - t2->hy;                                                   \
  h[3] = t2->pos_y + t2->hy - 1;                                               \
                                                                               \
  /* check for collision */                                                    \
  if ((l[0] <= h[1]) && (l[1] <= h[0]) && (l[2] <= h[3]) && (l[3] <= h[2]))    \
    collision_check = 1;

#define GRID_COLLISION_TEST_BOXES(t1, m, n)                                    \
  /* obtain x projections */                                                   \
  l[0] = t1->pos_x - t1->hx;                                                   \
  h[0] = t1->pos_x + t1->hx - 1;                                               \
                                                                               \
  l[1] = m * (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);                            \
  h[1] = ((m + 1) * (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) - 1;                \
                                                                               \
  /* obtain y projections */                                                   \
  l[2] = t1->pos_y - t1->hy;                                                   \
  h[2] = t1->pos_y + t1->hy - 1;                                               \
                                                                               \
  l[3] = n * (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);                            \
  h[3] = ((n + 1) * (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) - 1;                \
                                                                               \
  /* check for collision */                                                    \
  if ((l[0] <= h[1]) && (l[1] <= h[0]) && (l[2] <= h[3]) && (l[3] <= h[2]))    \
    collision_check = 1;

grid_box G_collision_grid[GRID_NUM_BOXES];

/*******************************************************************************
** grid_reset()
*******************************************************************************/
short int grid_reset()
{
  int i;
  int k;

  for (i = 0; i < GRID_NUM_BOXES; i++)
  {
    /* reset objects */
    G_collision_grid[i].object = GRID_OBJECT_NONE;
    G_collision_grid[i].state  = STATE_NONE;
    G_collision_grid[i].timer_offset = 0;

    /* reset things */
    for (k = 0; k < GRID_BOX_MAX_THINGS; k++)
      G_collision_grid[i].thing_indices[k] = 0;

    G_collision_grid[i].num_things = 0;
  }

  return 0;
}

/*******************************************************************************
** grid_add_thing()
*******************************************************************************/
short int grid_add_thing(int index)
{
  thing*    t;
  grid_box* b;

  int corner_low_x;
  int corner_high_x;
  int corner_low_y;
  int corner_high_y;

  int box_low_x;
  int box_high_x;
  int box_low_y;
  int box_high_y;

  int num_boxes_x;
  int num_boxes_y;

  int m;
  int n;

  int i;
  int j;

  int k;
  int thing_i;
  int insert_i;

  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 1;

  t = &G_world_all_things[index];

  /* determine boxes overlapped by this thing */
  GRID_DETERMINE_OVERLAPPED_BOXES(t)

  /* add thing to appropriate grid boxes */
  for (j = 0; j < num_boxes_y; j++)
  {
    for (i = 0; i < num_boxes_x; i++)
    {
      n = (box_low_y + j) % GRID_HEIGHT;
      m = (box_low_x + i) % GRID_WIDTH;

      b = &(G_collision_grid[(n * GRID_WIDTH) + m]);

      /* check if this thing is already in this grid box    */
      /* note that the indices should be sorted (ascending) */
      /* if it is not in this box, set the insert index     */
      thing_i = -1;
      insert_i = -1;

      for (k = 0; k < b->num_things; k++)
      {
        if (b->thing_indices[k] == index)
        {
          thing_i = k;
          break;
        }
        else if (b->thing_indices[k] > index)
        {
          insert_i = k;
          break;
        }
      }

      /* if this thing is already in this grid box, continue */
      if (thing_i != -1)
        continue;

      /* if an insert index was not found, insert this thing at the end */
      if (insert_i == -1)
      {
        b->thing_indices[b->num_things] = index;
        b->num_things += 1;
        continue;
      }

      /* add the thing to this grid box */
      if (b->num_things < GRID_BOX_MAX_THINGS - 1)
      {
        for (k = b->num_things - 1; k >= insert_i; k--)
          b->thing_indices[k + 1] = b->thing_indices[k];

        b->thing_indices[insert_i] = index;
        b->num_things += 1;
      }
    }
  }

  return 0;
}

/*******************************************************************************
** grid_remove_thing()
*******************************************************************************/
short int grid_remove_thing(int index)
{
  thing*    t;
  grid_box* b;

  int corner_low_x;
  int corner_high_x;
  int corner_low_y;
  int corner_high_y;

  int box_low_x;
  int box_high_x;
  int box_low_y;
  int box_high_y;

  int num_boxes_x;
  int num_boxes_y;

  int m;
  int n;

  int i;
  int j;

  int k;
  int thing_i;

  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 1;

  t = &G_world_all_things[index];

  /* determine boxes overlapped by this thing */
  GRID_DETERMINE_OVERLAPPED_BOXES(t)

  /* remove thing from appropriate grid boxes */
  for (j = 0; j < num_boxes_y; j++)
  {
    for (i = 0; i < num_boxes_x; i++)
    {
      n = (box_low_y + j) % GRID_HEIGHT;
      m = (box_low_x + i) % GRID_WIDTH;

      b = &(G_collision_grid[(n * GRID_WIDTH) + m]);

      /* find index of thing in this grid box */
      thing_i = -1;

      for (k = 0; k < b->num_things; k++)
      {
        if (b->thing_indices[k] == index)
        {
          thing_i = k;
          break;
        }
      }

      /* if this thing was not found in this grid box, continue */
      if (thing_i == -1)
        continue;

      /* remove this thing from this grid box */
      if (b->num_things > 0)
      {
        for (k = thing_i; k < b->num_things - 1; k++)
          b->thing_indices[k] = b->thing_indices[k + 1];

        b->num_things -= 1;
      }
    }
  }

  return 0;
}

/*******************************************************************************
** grid_move_thing()
*******************************************************************************/
short int grid_move_thing(int index, int mode, int amount)
{
  thing*    t1;
  thing*    t2;
  grid_box* b;

  int corner_low_x;
  int corner_high_x;
  int corner_low_y;
  int corner_high_y;

  int box_low_x;
  int box_high_x;
  int box_low_y;
  int box_high_y;

  int num_boxes_x;
  int num_boxes_y;

  int m;
  int n;

  int i;
  int j;

  int k;

  int l[4];
  int h[4];

  int collision_check;

  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 1;

  t1 = &G_world_all_things[index];

  /* make sure the mode is valid */
  if ((mode != GRID_MOVE_THING_MODE_HORIZONTAL) && 
      (mode != GRID_MOVE_THING_MODE_VERTICAL)   && 
      (mode != GRID_MOVE_THING_MODE_TEST_POSITION))
  {
    return 0;
  }

  /* initialize collision flag */
  t1->flag = THING_COLLISION_FLAG_NONE;

  /* make sure a move is occuring here */
  if ((mode == GRID_MOVE_THING_MODE_HORIZONTAL) && (amount == 0))
    return 0;

  if ((mode == GRID_MOVE_THING_MODE_VERTICAL) && (amount == 0))
    return 0;

  /* remove thing from grid */
  if (mode != GRID_MOVE_THING_MODE_TEST_POSITION)
    grid_remove_thing(index);

  /* horizontal movement */
  if (mode == GRID_MOVE_THING_MODE_HORIZONTAL)
  {
    /* move thing */
    t1->pos_x += amount;

    /* bound thing position */
    if (t1->pos_x - t1->hx < 0)
      t1->pos_x = t1->hx;
    else if (t1->pos_x + t1->hx > WORLD_WIDTH_IN_SUBPIXELS)
      t1->pos_x = WORLD_WIDTH_IN_SUBPIXELS - t1->hx;
  }
  /* vertical movement */
  else if (mode == GRID_MOVE_THING_MODE_VERTICAL)
  {
    /* move thing */
    t1->pos_y += amount;

    /* bound thing position */
    if (t1->pos_y - t1->hy < 0)
      t1->pos_y = t1->hy;
    else if (t1->pos_y + t1->hy > WORLD_HEIGHT_IN_SUBPIXELS)
      t1->pos_y = WORLD_HEIGHT_IN_SUBPIXELS - t1->hy;
  }

  /* determine boxes overlapped by this thing */
  GRID_DETERMINE_OVERLAPPED_BOXES(t1)

  /* test thing position against other things */
  for (j = 0; j < num_boxes_y; j++)
  {
    for (i = 0; i < num_boxes_x; i++)
    {
      n = (box_low_y + j) % GRID_HEIGHT;
      m = (box_low_x + i) % GRID_WIDTH;

      b = &(G_collision_grid[(n * GRID_WIDTH) + m]);

      for (k = 0; k < b->num_things; k++)
      {
        t2 = &G_world_all_things[b->thing_indices[k]];

        /* iniitalize collision check */
        collision_check = 0;

        /* standard collision check */
        GRID_COLLISION_TEST_THINGS(t1, t2)

        /* if a collision is found, process it */
        if (collision_check == 1)
        {
          /* vampire collisions */
          if (t1->type == THING_TYPE_VAMPIRE)
          {
            if (THING_IS_A_PICKUP(t2))
              t2->flag = THING_COLLISION_FLAG_PICKUP_COLLECTED;
          }
          /* evil eye spell collisions */
          else if (t1->type == THING_TYPE_EVIL_EYE_SPELL)
          {
            /* the evil eye can pass through pickups, pots, and flasks */
            if (THING_IS_NOT_A_PICKUP(t2) && 
                THING_CANNOT_BE_FLOWN_OVER(t2))
            {
              t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;

              if (THING_IS_A_SPELL(t2))
                t2->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
              else if (THING_IS_THE_PLAYER(t2))
              {
                if ((t2->flag == THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL)  || 
                    (t2->flag == THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL)   || 
                    (t2->flag == THING_COLLISION_FLAG_HIT_WITH_RING_SPELL)  || 
                    (t2->flag == THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL))
                {
                  t2->flag = THING_COLLISION_FLAG_NEGATE_SPELLS;
                }
                else if (t2->flag != THING_COLLISION_FLAG_NEGATE_SPELLS)
                  t2->flag = THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL;
              }
            }
          }
          /* fire spell collisions */
          else if (t1->type == THING_TYPE_FIRE_SPELL)
          {
            t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;

            if (THING_IS_A_SPELL(t2))
              t2->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
            else if ( THING_CAN_BE_BURNED(t2)           || 
                      (t2->type == THING_TYPE_ICE_CUBE) || 
                      (t2->type == THING_TYPE_ICE_CUBE_THAWING))
            {
              if ((t2->flag == THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL)  || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL)       || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_RING_SPELL)      || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL))
              {
                t2->flag = THING_COLLISION_FLAG_NEGATE_SPELLS;
              }
              else if (t2->flag != THING_COLLISION_FLAG_NEGATE_SPELLS)
                t2->flag = THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL;
            }
          }
          /* ice spell collisions */
          else if (t1->type == THING_TYPE_ICE_SPELL)
          {
            t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;

            if (THING_IS_A_SPELL(t2))
              t2->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
            else if ( THING_CAN_BE_FROZEN(t2)           || 
                      (t2->type == THING_TYPE_WISP_RT)  || 
                      (t2->type == THING_TYPE_WISP_LT)  || 
                      (t2->type == THING_TYPE_ICE_CUBE) || 
                      (t2->type == THING_TYPE_ICE_CUBE_THAWING))
            {
              if ((t2->flag == THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL)  || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL)      || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_RING_SPELL)      || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL))
              {
                t2->flag = THING_COLLISION_FLAG_NEGATE_SPELLS;
              }
              else if (t2->flag != THING_COLLISION_FLAG_NEGATE_SPELLS)
                t2->flag = THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL;
            }
          }
          /* ring spell collisions */
          else if (t1->type == THING_TYPE_RING_SPELL)
          {
            t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;

            if (THING_IS_A_SPELL(t2))
              t2->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
            else if (THING_CAN_BE_DAMAGED(t2))
            {
              if ((t2->flag == THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL)  || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL)      || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL)       || 
                  (t2->flag == THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL))
              {
                t2->flag = THING_COLLISION_FLAG_NEGATE_SPELLS;
              }
              else if (t2->flag != THING_COLLISION_FLAG_NEGATE_SPELLS)
                t2->flag = THING_COLLISION_FLAG_HIT_WITH_RING_SPELL;
            }
          }
          /* twin spell collisions */
          else if (t1->type == THING_TYPE_TWIN_SPELL)
          {
            /* the twin spell can pass through pickups, pots, and flasks */
            if (THING_IS_NOT_A_PICKUP(t2) && 
                THING_CANNOT_BE_FLOWN_OVER(t2))
            {
              t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;

              if (THING_IS_A_SPELL(t2))
                t2->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
              else if (THING_CAN_BE_DAMAGED(t2))
              {
                if ((t2->flag == THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL)  || 
                    (t2->flag == THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL)      || 
                    (t2->flag == THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL)       || 
                    (t2->flag == THING_COLLISION_FLAG_HIT_WITH_RING_SPELL))
                {
                  t2->flag = THING_COLLISION_FLAG_NEGATE_SPELLS;
                }
                else if (t2->flag != THING_COLLISION_FLAG_NEGATE_SPELLS)
                  t2->flag = THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL;
              }
            }
          }
        }
      }
    }
  }

  /* test thing position against grid objects */
  for (j = 0; j < num_boxes_y; j++)
  {
    for (i = 0; i < num_boxes_x; i++)
    {
      n = (box_low_y + j) % GRID_HEIGHT;
      m = (box_low_x + i) % GRID_WIDTH;

      b = &(G_collision_grid[(n * GRID_WIDTH) + m]);

      /* see if there is an object to collide with here */
      if (b->object == GRID_OBJECT_NONE)
        continue;

      /* iniitalize collision check */
      collision_check = 0;

      /* standard collision check */
      GRID_COLLISION_TEST_BOXES(t1, m, n)

      /* if a collision is found, process it */
      if (collision_check == 1)
      {
        /* vampire or bat */
        if (THING_IS_THE_PLAYER(t1))
        {
          if (b->object == GRID_OBJECT_PORTCULLIS_OPEN)
            G_portcullis_reached = 1;
        }
        /* evil eye & twin spells */
        else if ( (t1->type == THING_TYPE_EVIL_EYE_SPELL) || 
                  (t1->type == THING_TYPE_TWIN_SPELL))
        {
          if (GRID_OBJECT_WILL_BLOCK_EVIL_EYE_SPELLS(b))
          {
            t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
          }
        }
        /* other spells */
        else if ( (t1->type == THING_TYPE_FIRE_SPELL) || 
                  (t1->type == THING_TYPE_ICE_SPELL)  || 
                  (t1->type == THING_TYPE_RING_SPELL))
        {
          if (GRID_OBJECT_WILL_BLOCK_OTHER_SPELLS(b))
          {
            t1->flag = THING_COLLISION_FLAG_WE_HIT_SOMETHING;
          }
        }
      }
    }
  }

  /* add thing back to grid */
  if (mode != GRID_MOVE_THING_MODE_TEST_POSITION)
    grid_add_thing(index);

  return 0;
}

/*******************************************************************************
** grid_load_objects_from_tilemap()
*******************************************************************************/
short int grid_load_objects_from_tilemap()
{
  int m;
  int n;

  unsigned char* room_tilemap;

  int portcullis_position;

  /* set room tilemap */
  if ((G_current_room >= 0) && (G_current_room < LEVEL_NUM_ROOMS))
    room_tilemap = &G_room_tilemaps[G_current_room][0];
  else
    room_tilemap = &G_room_tilemaps[0][0];

  /* reset grid */
  grid_reset();

  /* load walls to grid */

  /* left & right */
  for (n = 0; n < GRID_HEIGHT; n++)
  {
    G_collision_grid[(n * GRID_WIDTH) + 0].object = GRID_OBJECT_WALL;
    G_collision_grid[(n * GRID_WIDTH) + 12].object = GRID_OBJECT_WALL;
  }

  /* top & bottom */
  for (m = 1; m < GRID_WIDTH - 1; m++)
  {
    G_collision_grid[(0 * GRID_WIDTH) + m].object = GRID_OBJECT_WALL;
    G_collision_grid[(1 * GRID_WIDTH) + m].object = GRID_OBJECT_WALL;
    G_collision_grid[(13 * GRID_WIDTH) + m].object = GRID_OBJECT_WALL;
  }

  /* load portcullis to grid */
  portcullis_position = room_tilemap[LEVEL_ROOM_NUM_TILES - 1];

  if ((portcullis_position >= 0) && (portcullis_position <= 10))
    G_collision_grid[(1 * GRID_WIDTH) + portcullis_position + 1].object = GRID_OBJECT_PORTCULLIS_CLOSED;
  else
    G_collision_grid[(1 * GRID_WIDTH) + 1].object = GRID_OBJECT_PORTCULLIS_CLOSED;

  /* load room tilemap to grid */
  for (n = 0; n < LEVEL_ROOM_HEIGHT; n++)
  {
    for (m = 0; m < LEVEL_ROOM_WIDTH; m++)
    {
      if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BLOCK)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_BLOCK;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_PILLAR)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_PILLAR;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WATER)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_WATER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BRIDGE_VERT)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_BRIDGE_VERT;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BRIDGE_HORI)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_BRIDGE_HORI;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_PADLOCK)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_PADLOCK;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SPIKE_DROPPER)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_SPIKE_DROPPER_SPAWNER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_SPIKE_RAISER)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_SPIKE_RAISER_SPAWNER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROW_SPINNER_CCW)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_ARROW_SPINNER_CCW_SPAWNER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROW_SPINNER_CW)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_ARROW_SPINNER_CW_SPAWNER;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROWS_RIGHT)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_ARROWS_RIGHT;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROWS_UP)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_ARROWS_UP;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROWS_LEFT)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_ARROWS_LEFT;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_ARROWS_DOWN)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_ARROWS_DOWN;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_FLOOR_SPIKES_DOWN)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_FLOOR_SPIKES_DOWN;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_FLOOR_SPIKES_UP)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_FLOOR_SPIKES_UP;
      else if (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_AUDREY)
        G_collision_grid[((n + 2) * GRID_WIDTH) + m + 1].object = GRID_OBJECT_AUDREY_SPAWNER;
    }
  }

  return 0;
}

