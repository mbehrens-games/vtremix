/*******************************************************************************
** logic.c (grid and world update functions)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "animate.h"
#include "doremi.h"
#include "global.h"
#include "grid.h"
#include "lifetime.h"
#include "state.h"
#include "thing.h"
#include "vampire.h"
#include "world.h"

/*******************************************************************************
** logic_attempt_move()
*******************************************************************************/
short int logic_attempt_move(int index, int facing)
{
  int k;

  thing* t1;
  thing* t2;

  grid_box* b;

  int box_x;
  int box_y;

  int blocked_check;

  /* make sure index is valid */
  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  /* make sure facing is valid */
  if ((facing != THING_FACING_RIGHT)  && 
      (facing != THING_FACING_UP)     && 
      (facing != THING_FACING_LEFT)   && 
      (facing != THING_FACING_DOWN))
  {
    return 0;
  }

  /* obtain thing pointer */
  t1 = &G_world_all_things[index];

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_TURNING)                  || 
      (t1->state == STATE_BLOCKED_IN)               || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* update facing */
  t1->facing = facing;

  /* determine grid box for space we are moving into */
  box_x = t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  box_y = t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);

  if (t1->facing == THING_FACING_RIGHT)
    box_x += 1;
  else if (t1->facing == THING_FACING_UP)
    box_y -= 1;
  else if (t1->facing == THING_FACING_LEFT)
    box_x -= 1;
  else if (t1->facing == THING_FACING_DOWN)
    box_y += 1;

  /* make sure the box index is valid */
  if ((box_x < 0) || (box_x >= GRID_WIDTH))
    return 0;

  if ((box_y < 0) || (box_y >= GRID_HEIGHT))
    return 0;

  b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

  /* check if this thing can move into this space */
  blocked_check = 0;

  /* check against grid objects (other than arrows) */

  /* vampire */
  if (t1->type == THING_TYPE_VAMPIRE)
  {
    if (GRID_OBJECT_WILL_BLOCK_THE_VAMPIRE(b))
      blocked_check = 1;
  }
  /* bat */
  else if (t1->type == THING_TYPE_BAT)
  {
    if (GRID_OBJECT_WILL_BLOCK_THE_BAT(b))
      blocked_check = 1;
  }
  /* doubles in bat form */
  else if (THING_IS_A_FLYING_DENIZEN(t1))
  {
    if (GRID_OBJECT_WILL_BLOCK_FLYING_DENIZENS(b))
      blocked_check = 1;
  }
  /* other denizens */
  else if (THING_IS_A_WALKING_DENIZEN(t1))
  {
    if (GRID_OBJECT_WILL_BLOCK_WALKING_DENIZENS(b))
      blocked_check = 1;
  }
  /* non-spike breaking pushable objects */
  else if (THING_IS_A_NON_SPIKE_BREAKING_PUSHABLE_OBJECT(t1))
  {
    if (GRID_OBJECT_WILL_BLOCK_NON_SPIKE_BREAKING_PUSHABLE_OBJECTS(b))
      blocked_check = 1;
  }
  /* spike breaking pushable objects */
  else if (THING_IS_A_SPIKE_BREAKING_PUSHABLE_OBJECT(t1))
  {
    if (GRID_OBJECT_WILL_BLOCK_SPIKE_BREAKING_PUSHABLE_OBJECTS(b))
      blocked_check = 1;
  }

  /* check against arrows */
  if (THING_IS_THE_PLAYER(t1) || 
      THING_IS_A_MOVING_DENIZEN(t1))
  {
    /* check for opposing arrows */
    if ((t1->facing == THING_FACING_RIGHT) && 
        (b->object == GRID_OBJECT_ARROWS_LEFT))
    {
      blocked_check = 1;
    }

    if ((t1->facing == THING_FACING_UP) && 
        (b->object == GRID_OBJECT_ARROWS_DOWN))
    {
      blocked_check = 1;
    }

    if ((t1->facing == THING_FACING_LEFT) && 
        (b->object == GRID_OBJECT_ARROWS_RIGHT))
    {
      blocked_check = 1;
    }

    if ((t1->facing == THING_FACING_DOWN) && 
        (b->object == GRID_OBJECT_ARROWS_UP))
    {
      blocked_check = 1;
    }
  }

  /* if we were blocked by a grid object, return */
  if (blocked_check == 1)
    return 0;

  /* check against things */
  for (k = 0; k < b->num_things; k++)
  {
    t2 = &G_world_all_things[b->thing_indices[k]];

    /* vampire, normal double in vampire form */
    if (t1->type == THING_TYPE_VAMPIRE)
    {
      /* pushable objects, normal doubles */
      if (THING_IS_A_PUSHABLE_OBJECT(t2)                  || 
          (t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
          (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
      {
        /* try to move this thing */
        if ((t2->state != STATE_STARTED_MOVING)   && 
            (t2->state != STATE_MOVING_ONE_SPACE) && 
            (t2->state != STATE_AWAITING_SNAP_TO_GRID))
        {
          logic_attempt_move(b->thing_indices[k], facing);

          if (t2->state != STATE_STARTED_MOVING)
            blocked_check = 1;
        }
        /* if this thing is already moving, but in a    */
        /* different direction, the vampire is blocked  */
        else if ( (t2->state == STATE_STARTED_MOVING) || 
                  (t2->state == STATE_MOVING_ONE_SPACE))
        {
          if (t2->facing != t1->facing)
            blocked_check = 1;
        }
        else
          blocked_check = 1;
      }
      /* otherwise, if this is not a pickup or spell, the vampire is blocked */
      else if ( THING_IS_NOT_A_PICKUP(t2) && 
                THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
    /* normal double in vampire form */
    else if (t1->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)
    {
      /* pushable objects, player, normal doubles */
      if (THING_IS_A_PUSHABLE_OBJECT(t2)                  || 
          THING_IS_THE_PLAYER(t2)                         || 
          (t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
          (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
      {
        /* try to move this pushable object */
        if ((t2->state != STATE_STARTED_MOVING)   && 
            (t2->state != STATE_MOVING_ONE_SPACE) && 
            (t2->state != STATE_AWAITING_SNAP_TO_GRID))
        {
          logic_attempt_move(b->thing_indices[k], facing);

          if (t2->state != STATE_STARTED_MOVING)
            blocked_check = 1;
        }
        /* if this thing is already moving, but in a    */
        /* different direction, the vampire is blocked  */
        else if ( (t2->state == STATE_STARTED_MOVING) || 
                  (t2->state == STATE_MOVING_ONE_SPACE))
        {
          if (t2->facing != t1->facing)
            blocked_check = 1;
        }
        else
          blocked_check = 1;
      }
      /* otherwise, if this is not a spell, the vampire is blocked */
      else if (THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
    /* reverse double in vampire form */
    else if (t1->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)
    {
      /* pushable objects */
      if (THING_IS_A_PUSHABLE_OBJECT(t2))
      {
        /* try to move this pushable object */
        if ((t2->state != STATE_STARTED_MOVING)   && 
            (t2->state != STATE_MOVING_ONE_SPACE) && 
            (t2->state != STATE_AWAITING_SNAP_TO_GRID))
        {
          logic_attempt_move(b->thing_indices[k], facing);

          if (t2->state != STATE_STARTED_MOVING)
            blocked_check = 1;
        }
        /* if this thing is already moving, but in a    */
        /* different direction, the double is blocked   */
        else if ( (t2->state == STATE_STARTED_MOVING) || 
                  (t2->state == STATE_MOVING_ONE_SPACE))
        {
          if (t2->facing != t1->facing)
            blocked_check = 1;
        }
        else
          blocked_check = 1;
      }
      /* otherwise, if this is not a spell, the vampire is blocked */
      else if (THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
    /* bat, normal double in bat form */
    else if ( (t1->type == THING_TYPE_BAT) || 
              (t1->type == THING_TYPE_DOUBLE_NORMAL_BAT))
    {
      /* first, see if this is a double moving in the same direction */
      if (THING_IS_THE_PLAYER(t2)                         || 
          (t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
          (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
      {
        if ((t2->state != STATE_STARTED_MOVING)   && 
            (t2->state != STATE_MOVING_ONE_SPACE) && 
            (t2->state != STATE_AWAITING_SNAP_TO_GRID))
        {
          logic_attempt_move(b->thing_indices[k], facing);

          if (t2->state != STATE_STARTED_MOVING)
            blocked_check = 1;
        }
        /* if this thing is already moving, but in    */
        /* a different direction, the bat is blocked  */
        else if ( (t2->state == STATE_STARTED_MOVING) || 
                  (t2->state == STATE_MOVING_ONE_SPACE))
        {
          if (t2->facing != t1->facing)
            blocked_check = 1;
        }
        else
          blocked_check = 1;
      }
      /* otherwise, if this is not a pot, flask, or spell, the bat is blocked */
      else if ( THING_CANNOT_BE_FLOWN_OVER(t2) && 
                THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
    /* reverse double in bat form */
    else if (t1->type == THING_TYPE_DOUBLE_REVERSE_BAT)
    {
      /* if this is not a pot, flask, or spell, the bat is blocked */
      if (THING_CANNOT_BE_FLOWN_OVER(t2) && 
          THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
    /* lil bots */
    else if (t1->type == THING_TYPE_LIL_BOT)
    {
      /* pushable objects */
      if (THING_IS_A_PUSHABLE_OBJECT(t2))
      {
        /* try to move this pushable object */
        if ((t2->state != STATE_STARTED_MOVING)   && 
            (t2->state != STATE_MOVING_ONE_SPACE) && 
            (t2->state != STATE_AWAITING_SNAP_TO_GRID))
        {
          logic_attempt_move(b->thing_indices[k], facing);

          if (t2->state != STATE_STARTED_MOVING)
            blocked_check = 1;
        }
        else
          blocked_check = 1;
      }
      /* if this is not a spell, we are blocked */
      else if (THING_IS_NOT_A_SPELL(t2))
        blocked_check = 1;
    }
    /* jiangshis and wisps */
    else if ( (t1->type == THING_TYPE_WISP_RT)      || 
              (t1->type == THING_TYPE_WISP_LT)      || 
              (t1->type == THING_TYPE_JIANGSHI_RT)  || 
              (t1->type == THING_TYPE_JIANGSHI_LT))
    {
      /* if we encounter the player (or other moving denizen), stop! (or dissipate) */
      if (THING_IS_THE_PLAYER(t2) || 
          THING_IS_A_MOVING_DENIZEN(t2))
      {
        t1->state = STATE_READY_TO_CHANGE;
        return 0;
      }
      /* if this is not a spell, we are blocked */
      else if (THING_IS_NOT_A_SPELL(t2))
        blocked_check = 1;
    }
    /* pots & flasks */
    else if (THING_CAN_BE_FLOWN_OVER(t1))
    {
      if ((t2->type != THING_TYPE_BAT)      && 
          THING_IS_NOT_A_FLYING_DENIZEN(t2) && 
          THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
    /* other things (denizens, other pushable objects) */
    else
    {
      if (THING_IS_NOT_A_SPELL(t2))
      {
        blocked_check = 1;
      }
    }
  }

  /* if we were blocked by a thing, return */
  if (blocked_check == 1)
    return 0;

  /* otherwise, start the move! */
  t1->state = STATE_STARTED_MOVING;
  t1->timer_offset = 240 - G_timer_count;

  if (t1->facing == THING_FACING_RIGHT)
  {
    t1->vel_x = THING_DENIZEN_VEL_WALK;
    t1->vel_y = 0;
  }
  else if (t1->facing == THING_FACING_UP)
  {
    t1->vel_x = 0;
    t1->vel_y = -THING_DENIZEN_VEL_WALK;
  }
  else if (t1->facing == THING_FACING_LEFT)
  {
    t1->vel_x = -THING_DENIZEN_VEL_WALK;
    t1->vel_y = 0;
  }
  else if (t1->facing == THING_FACING_DOWN)
  {
    t1->vel_x = 0;
    t1->vel_y = THING_DENIZEN_VEL_WALK;
  }

  /* perform the 1st frame of movement now (this allows */
  /* the grid space being moved into to be "reserved")  */
  if (t1->state == STATE_STARTED_MOVING)
  {
    if (t1->vel_x != 0)
      grid_move_thing(index, GRID_MOVE_THING_MODE_HORIZONTAL, t1->vel_x);
    else if (t1->vel_y != 0)
      grid_move_thing(index, GRID_MOVE_THING_MODE_VERTICAL, t1->vel_y);
  }

  return 0;
}

/*******************************************************************************
** logic_attempt_transform()
*******************************************************************************/
short int logic_attempt_transform(int index)
{
  thing* t1;

  grid_box* b;

  int box_x;
  int box_y;

  /* make sure index is valid */
  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  /* obtain thing pointer */
  t1 = &G_world_all_things[index];

  /* make sure this thing is the player or a double */
  if ((t1->type != THING_TYPE_VAMPIRE)                && 
      (t1->type != THING_TYPE_BAT)                    && 
      (t1->type != THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  && 
      (t1->type != THING_TYPE_DOUBLE_NORMAL_BAT)      && 
      (t1->type != THING_TYPE_DOUBLE_REVERSE_VAMPIRE) && 
      (t1->type != THING_TYPE_DOUBLE_REVERSE_BAT))
  {
    return 0;
  }

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_TURNING)                  || 
      (t1->state == STATE_BLOCKED_IN)               || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* determine grid box for space we are currently in */
  box_x = t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  box_y = t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);

  /* make sure the box index is valid */
  if ((box_x < 0) || (box_x >= GRID_WIDTH))
    return 0;

  if ((box_y < 0) || (box_y >= GRID_HEIGHT))
    return 0;

  b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

  /* if we are currently in vampire form, we  */
  /* should always be able to turn into a bat */
  if (t1->type == THING_TYPE_VAMPIRE)
    t1->type = THING_TYPE_BAT;
  else if (t1->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)
    t1->type = THING_TYPE_DOUBLE_NORMAL_BAT;
  else if (t1->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)
    t1->type = THING_TYPE_DOUBLE_REVERSE_BAT;
  else
  {
    /* otherwise, check to see if we can land here */
    if ((b->object == GRID_OBJECT_WATER) || 
        (b->object == GRID_OBJECT_FLOOR_SPIKES_UP))
    {
      return 0;
    }

    /* if there is something else here (like a pot), return */
    if (b->num_things > 1)
      return 0;

    /* if the space is clear, change back! */
    if (t1->type == THING_TYPE_BAT)
      t1->type = THING_TYPE_VAMPIRE;
    else if (t1->type == THING_TYPE_DOUBLE_NORMAL_BAT)
      t1->type = THING_TYPE_DOUBLE_NORMAL_VAMPIRE;
    else if (t1->type == THING_TYPE_DOUBLE_REVERSE_BAT)
      t1->type = THING_TYPE_DOUBLE_REVERSE_VAMPIRE;

    /* trigger bridges & floor spikes if necessary */
    if ((b->object == GRID_OBJECT_BRIDGE_VERT) || 
        (b->object == GRID_OBJECT_BRIDGE_HORI) || 
        (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN))
    {
      b->state = STATE_READY_TO_CHANGE;
    }
  }

  return 0;
}

/*******************************************************************************
** logic_attempt_spell_cast()
*******************************************************************************/
short int logic_attempt_spell_cast(int index)
{
  thing* t1;

  int spell_type;

  int spell_pos_x;
  int spell_pos_y;

  int spell_base_vel;

  int spell_vel_x;
  int spell_vel_y;

  /* make sure index is valid */
  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  /* obtain thing pointer */
  t1 = &G_world_all_things[index];

  /* make sure this thing is the player, a double, or a spellcaster */
  if ((t1->type != THING_TYPE_VAMPIRE)                && 
      (t1->type != THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  && 
      (t1->type != THING_TYPE_DOUBLE_REVERSE_VAMPIRE) && 
      THING_IS_NOT_A_SPELLCASTING_DENIZEN(t1))
  {
    return 0;
  }

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_TURNING)                  || 
      (t1->state == STATE_BLOCKED_IN)               || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* determine spell type */
  if ((t1->type == THING_TYPE_VAMPIRE)                || 
      (t1->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)  || 
      (t1->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
      (t1->type == THING_TYPE_SHADE))
  {
    spell_type = THING_TYPE_ICE_SPELL;
  }
  else if ( (t1->type == THING_TYPE_PUMPKIN) || 
            (t1->type == THING_TYPE_GHOST_AWAKE))
  {
    spell_type = THING_TYPE_EVIL_EYE_SPELL;
  }
  else if (t1->type == THING_TYPE_SNAKE_AWAKE)
    spell_type = THING_TYPE_FIRE_SPELL;
  else if (t1->type == THING_TYPE_WITCH_AWAKE)
    spell_type = THING_TYPE_RING_SPELL;
  else if (t1->type == THING_TYPE_SORCERESS)
    spell_type = THING_TYPE_TWIN_SPELL;
  else
    return 0;

  /* determine base spell velocity */
  if ((spell_type == THING_TYPE_RING_SPELL) || 
      (spell_type == THING_TYPE_TWIN_SPELL))
  {
    spell_base_vel = THING_SPELL_VEL_SLOW;
  }
  else if ( (spell_type == THING_TYPE_FIRE_SPELL) || 
            (spell_type == THING_TYPE_ICE_SPELL))
  {
    spell_base_vel = THING_SPELL_VEL_FAST;
  }
  else if (spell_type == THING_TYPE_EVIL_EYE_SPELL)
    spell_base_vel = THING_SPELL_VEL_ULTRA;
  else
    return 0;

  /* determine spell position and velocity */

  /* note that we shift back the starting position by 1 frame of movement   */
  /* (by adding/subtracting the base velocity) so that on the next update   */
  /* tick the spell will check only the grid box in front of the caster     */
  /* for collision detection (instead of also looking at the one after it). */
  /* this is done to fix a bug where you could freeze 2 denizens with 1 ice */
  /* spell by hitting both one in right front of you, and one behind it.    */
  if (t1->facing == THING_FACING_RIGHT)
  {
    spell_pos_x = t1->pos_x + (GRID_BOX_SIZE * THING_NUM_SUBPIXELS) - spell_base_vel;
    spell_pos_y = t1->pos_y;

    spell_vel_x = spell_base_vel;
    spell_vel_y = 0;
  }
  else if (t1->facing == THING_FACING_UP)
  {
    spell_pos_x = t1->pos_x;
    spell_pos_y = t1->pos_y - (GRID_BOX_SIZE * THING_NUM_SUBPIXELS) + spell_base_vel;

    spell_vel_x = 0;
    spell_vel_y = -spell_base_vel;
  }
  else if (t1->facing == THING_FACING_LEFT)
  {
    spell_pos_x = t1->pos_x - (GRID_BOX_SIZE * THING_NUM_SUBPIXELS) + spell_base_vel;
    spell_pos_y = t1->pos_y;

    spell_vel_x = -spell_base_vel;
    spell_vel_y = 0;
  }
  else if (t1->facing == THING_FACING_DOWN)
  {
    spell_pos_x = t1->pos_x;
    spell_pos_y = t1->pos_y + (GRID_BOX_SIZE * THING_NUM_SUBPIXELS) - spell_base_vel;

    spell_vel_x = 0;
    spell_vel_y = spell_base_vel;
  }
  else
    return 0;

  /* spawn the spell */
  world_spawn_thing(spell_type, t1->facing, 
                    spell_pos_x, spell_pos_y, spell_vel_x, spell_vel_y, 0);

  /* set thing's state to casting */
  t1->state = STATE_CASTING;
  t1->timer_offset = 240 - G_timer_count;

  /* play sfx */
  if (spell_type == THING_TYPE_ICE_SPELL)
    doremi_play_sfx(SFX_INDEX_CAST_ICE_SPELL);
  else if (spell_type == THING_TYPE_EVIL_EYE_SPELL)
    doremi_play_sfx(SFX_INDEX_CAST_EVIL_EYE);
  else if (spell_type == THING_TYPE_FIRE_SPELL)
    doremi_play_sfx(SFX_INDEX_CAST_FIRE_SPELL);
  else if (spell_type == THING_TYPE_RING_SPELL)
    doremi_play_sfx(SFX_INDEX_CAST_RING_SPELL);
  else if (spell_type == THING_TYPE_TWIN_SPELL)
    doremi_play_sfx(SFX_INDEX_CAST_TWIN_SPELL);

  return 0;
}

/*******************************************************************************
** logic_complete_grid_object_lifetimes()
*******************************************************************************/
short int logic_complete_grid_object_lifetimes()
{
  int m;
  int n;

  grid_box* b;

  unsigned int adjusted_timer_count;

  /* check for completed lifetimes */
  for (n = 0; n < GRID_HEIGHT; n++)
  {
    for (m = 0; m < GRID_WIDTH; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH) + m];

      /* determine adjusted timer count */
      adjusted_timer_count = (G_timer_count + b->timer_offset) % 240;

      /* clear collapsed bridge if necessary */
      if ((b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING) || 
          (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING))
      {
        if (adjusted_timer_count >= LIFETIME_BRIDGE_COLLAPSING)
        {
          b->object = GRID_OBJECT_WATER;
          b->state = STATE_NONE;
          b->timer_offset = 0;
        }
      }
      /* clear opened padlock if necessary */
      else if (b->object == GRID_OBJECT_PADLOCK_OPENING)
      {
        if (adjusted_timer_count >= LIFETIME_PADLOCK_OPENING)
        {
          b->object = GRID_OBJECT_NONE;
          b->state = STATE_NONE;
          b->timer_offset = 0;
        }
      }
      /* reset changing floor spikes state if necessary */
      else if ( (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN) && 
                (b->state == STATE_CHANGING))
      {
        if (adjusted_timer_count >= LIFETIME_FLOOR_SPIKES_CHANGING)
        {
          b->state = STATE_NONE;
          b->timer_offset = 0;
        }
      }
      /* reset changing floor spikes state if necessary */
      else if ( (b->object == GRID_OBJECT_FLOOR_SPIKES_UP) && 
                (b->state == STATE_CHANGING))
      {
        if (adjusted_timer_count >= LIFETIME_FLOOR_SPIKES_CHANGING)
        {
          b->state = STATE_NONE;
          b->timer_offset = 0;
        }
      }
      /* reset portcullis state if necessary */
      else if ( (b->object == GRID_OBJECT_PORTCULLIS_OPEN) && 
                (b->state == STATE_CHANGING))
      {
        if (adjusted_timer_count >= LIFETIME_PORTCULLIS_OPENING)
        {
          b->state = STATE_NONE;
          b->timer_offset = 0;
        }
      }
      /* start thawing submerged ice cube if necessary */
      else if (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE)
      {
        if (adjusted_timer_count >= LIFETIME_ICE_CUBE)
        {
          b->object = GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING;
          b->state = STATE_NONE;
          b->timer_offset = 240 - G_timer_count;
        }
      }
      /* complete thawing submerged ice cube if necessary */
      else if ( (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING) && 
                (b->state != STATE_READY_TO_CHANGE))
      {
        if (adjusted_timer_count >= LIFETIME_ICE_CUBE)
        {
          b->state = STATE_READY_TO_CHANGE;
          b->timer_offset = 0;
        }
      }
      /* complete audrey / pickup spawner charge-up */
      else if ( (b->object == GRID_OBJECT_AUDREY_SPAWNER)             || 
                (b->object == GRID_OBJECT_SPIKE_DROPPER_SPAWNER)      || 
                (b->object == GRID_OBJECT_SPIKE_RAISER_SPAWNER)       || 
                (b->object == GRID_OBJECT_ARROW_SPINNER_CCW_SPAWNER)  || 
                (b->object == GRID_OBJECT_ARROW_SPINNER_CW_SPAWNER))
      {
        if ((b->state == STATE_SPAWNER_CHARGING_UP) && 
            (adjusted_timer_count >= LIFETIME_SPAWNER_CHARGING_UP))
        {
          b->state = STATE_SPAWNER_FULLY_CHARGED;
          b->timer_offset = 0;
        }
      }
    }
  }

  return 0;
}

/*******************************************************************************
** logic_grid_objects_activate()
*******************************************************************************/
short int logic_grid_objects_activate()
{
  int m;
  int n;

  int k;

  grid_box* b;

  thing* t;

  int change_check;

  /* check for grid objects that are ready to change */
  for (n = 0; n < GRID_HEIGHT; n++)
  {
    for (m = 0; m < GRID_WIDTH; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH) + m];

      if ((b->object == GRID_OBJECT_BRIDGE_VERT) && 
          (b->state == STATE_READY_TO_CHANGE))
      {
        /* check if there are any things on this bridge */
        change_check = 1;

        for (k = 0; k < b->num_things; k++)
        {
          t = &G_world_all_things[b->thing_indices[k]];

          /* if this is not a bat or a spell, we can't collapse */
          if ((t->type != THING_TYPE_BAT)       && 
              THING_IS_NOT_A_FLYING_DENIZEN(t)  && 
              THING_IS_NOT_A_SPELL(t))
          {
            change_check = 0;
          }
        }

        if (change_check == 1)
        {
          b->object = GRID_OBJECT_BRIDGE_VERT_COLLAPSING;
          b->state = STATE_NONE;
          b->timer_offset = 240 - G_timer_count;

          doremi_play_sfx(SFX_INDEX_BRIDGE_COLLAPSE);
        }
      }
      else if ( (b->object == GRID_OBJECT_BRIDGE_HORI) && 
                (b->state == STATE_READY_TO_CHANGE))
      {
        /* check if there are any things on this bridge */
        change_check = 1;

        for (k = 0; k < b->num_things; k++)
        {
          t = &G_world_all_things[b->thing_indices[k]];

          /* if this is not a bat or a spell, we can't collapse */
          if ((t->type != THING_TYPE_BAT)       && 
              THING_IS_NOT_A_FLYING_DENIZEN(t)  && 
              THING_IS_NOT_A_SPELL(t))
          {
            change_check = 0;
          }
        }

        if (change_check == 1)
        {
          b->object = GRID_OBJECT_BRIDGE_HORI_COLLAPSING;
          b->state = STATE_NONE;
          b->timer_offset = 240 - G_timer_count;

          doremi_play_sfx(SFX_INDEX_BRIDGE_COLLAPSE);
        }
      }
      else if ( (b->object == GRID_OBJECT_PADLOCK) && 
                (b->state == STATE_READY_TO_CHANGE))
      {
        b->object = GRID_OBJECT_PADLOCK_OPENING;
        b->state = STATE_NONE;
        b->timer_offset = 240 - G_timer_count;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_RIGHT) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CCW))
      {
        b->object = GRID_OBJECT_ARROWS_UP;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_RIGHT) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CW))
      {
        b->object = GRID_OBJECT_ARROWS_DOWN;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_UP) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CCW))
      {
        b->object = GRID_OBJECT_ARROWS_LEFT;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_UP) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CW))
      {
        b->object = GRID_OBJECT_ARROWS_RIGHT;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_LEFT) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CCW))
      {
        b->object = GRID_OBJECT_ARROWS_DOWN;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_LEFT) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CW))
      {
        b->object = GRID_OBJECT_ARROWS_UP;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_DOWN) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CCW))
      {
        b->object = GRID_OBJECT_ARROWS_RIGHT;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_DOWN) && 
                (b->state == STATE_ARROWS_READY_TO_SPIN_CW))
      {
        b->object = GRID_OBJECT_ARROWS_LEFT;
        b->state = STATE_NONE;
      }
      else if ( (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN) && 
                (b->state == STATE_READY_TO_CHANGE))
      {
        /* check if there are any things above these floor spikes */
        change_check = 1;

        for (k = 0; k < b->num_things; k++)
        {
          t = &G_world_all_things[b->thing_indices[k]];

          /* floor spikes are destroyed by metal boxes & pots */
          if ((t->type == THING_TYPE_BOX_METAL) || 
              (t->type == THING_TYPE_POT_METAL))
          {
            change_check = 0;

            b->object = GRID_OBJECT_NONE;
            b->state = STATE_NONE;
            b->timer_offset = 0;

            G_flag_redraw_grid_objects = 1;

            break;
          }
          /* floor spikes mutually destroy flasks */
          else if ( (t->type == THING_TYPE_ACID_FLASK) || 
                    (t->type == THING_TYPE_STONE_FLASK))
          {
            change_check = 0;

            b->object = GRID_OBJECT_NONE;
            b->state = STATE_NONE;
            b->timer_offset = 0;

            G_flag_redraw_grid_objects = 1;

            t->type = THING_TYPE_PUFF_GREY;
            t->state = STATE_NONE;
            t->timer_offset = 240 - G_timer_count;

            break;
          }
          /* otherwise, if this is not a bat or a spell, we can't rise */
          else if ( (t->type != THING_TYPE_BAT)       && 
                    THING_IS_NOT_A_FLYING_DENIZEN(t)  && 
                    THING_IS_NOT_A_SPELL(t))
          {
            change_check = 0;
          }
        }

        if (change_check == 1)
        {
          b->object = GRID_OBJECT_FLOOR_SPIKES_UP;
          b->state = STATE_CHANGING;
          b->timer_offset = 240 - G_timer_count;

          doremi_play_sfx(SFX_INDEX_SPIKE_RAISE);
        }
      }
      else if ( (b->object == GRID_OBJECT_FLOOR_SPIKES_UP) && 
                (b->state == STATE_READY_TO_CHANGE))
      {
        /* nothing prevents the floor spikes from dropping! */
        b->object = GRID_OBJECT_FLOOR_SPIKES_DOWN;
        b->state = STATE_CHANGING;
        b->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_SPIKE_LOWER);
      }
      else if ( (b->object == GRID_OBJECT_PORTCULLIS_CLOSED) && 
                (b->state == STATE_READY_TO_CHANGE))
      {
        b->object = GRID_OBJECT_PORTCULLIS_OPEN;
        b->state = STATE_CHANGING;
        b->timer_offset = 240 - G_timer_count;
      }
      else if ( (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING) && 
                (b->state == STATE_READY_TO_CHANGE))
      {
        /* check if there are any things on this ice cube */
        change_check = 1;

        for (k = 0; k < b->num_things; k++)
        {
          t = &G_world_all_things[b->thing_indices[k]];

          /* if this is not a bat or a spell, we can't collapse */
          if ((t->type != THING_TYPE_BAT)       && 
              THING_IS_NOT_A_FLYING_DENIZEN(t)  && 
              THING_IS_NOT_A_SPELL(t))
          {
            change_check = 0;
          }
        }

        if (change_check == 1)
        {
          b->object = GRID_OBJECT_WATER;
          b->state = STATE_NONE;
          b->timer_offset = 0;

          G_flag_redraw_grid_objects = 1;
        }
      }
    }
  }

  return 0;
}

/*******************************************************************************
** logic_start_audrey_respawns()
*******************************************************************************/
short int logic_start_audrey_respawns()
{
  int k;

  int m;
  int n;

  grid_box* b;

  thing* t;

  int available_check;

  /* begin charging available audrey spawners */
  for (n = 0; n < GRID_HEIGHT; n++)
  {
    for (m = 0; m < GRID_WIDTH; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH) + m];

      if (b->object == GRID_OBJECT_NONE)
        continue;

      /* if no audreys to respawn, return */
      if (G_num_audrey_respawns == 0)
        return 0;

      /* check for available audrey spawners */
      if ((b->object == GRID_OBJECT_AUDREY_SPAWNER) && 
          (b->state == STATE_NONE))
      {
        /* make sure this spawn does not already have an audrey above it */
        available_check = 1;

        for (k = 0; k < b->num_things; k++)
        {
          t = &G_world_all_things[b->thing_indices[k]];

          if (t->type == THING_TYPE_AUDREY)
          {
            available_check = 0;
            break;
          }

          if ((t->type == THING_TYPE_ICE_CUBE) || 
              (t->type == THING_TYPE_ICE_CUBE_THAWING))
          {
            if (t->freeze_type == THING_TYPE_AUDREY)
            {
              available_check = 0;
              break;
            }
          }
        }

        /* if this spawner is available, start charging it */
        if (available_check == 1)
        {
          if (G_num_audrey_respawns > 0)
            G_num_audrey_respawns -= 1;

          b->state = STATE_SPAWNER_CHARGING_UP;
          b->timer_offset = 240 - G_timer_count;
        }
      }
    }
  }

  return 0;
}

/*******************************************************************************
** logic_spawners_update()
*******************************************************************************/
short int logic_spawners_update()
{
  int m;
  int n;

  grid_box* b;

  int pos_x;
  int pos_y;

  unsigned int pickup_timer_offset;

  /* update audrey and pickup spawners */
  for (n = 0; n < GRID_HEIGHT; n++)
  {
    for (m = 0; m < GRID_WIDTH; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH) + m];

      if (b->object == GRID_OBJECT_NONE)
        continue;

      /* check for charged spawners */
      if ((b->object == GRID_OBJECT_AUDREY_SPAWNER) && 
          (b->state == STATE_SPAWNER_FULLY_CHARGED) && 
          (b->num_things == 0))
      {
        b->state = STATE_NONE;

        pos_x = (GRID_BOX_SIZE * m + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
        pos_y = (GRID_BOX_SIZE * n + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

        world_spawn_thing(THING_TYPE_AUDREY, THING_FACING_RIGHT, 
                          pos_x, pos_y, 0, 0, 0);

        doremi_play_sfx(SFX_INDEX_AUDREY_RESPAWN);
      }
      else if ( (b->object == GRID_OBJECT_SPIKE_DROPPER_SPAWNER)  && 
                (b->state == STATE_SPAWNER_FULLY_CHARGED)         && 
                (b->num_things == 0))
      {
        b->state = STATE_NONE;

        pos_x = (GRID_BOX_SIZE * m + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
        pos_y = (GRID_BOX_SIZE * n + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

        pickup_timer_offset = 240 - (((n + m) % ANIM_PICKUP_NUM_FRAMES) * ANIM_PICKUP_FRAME_LENGTH);

        world_spawn_thing(THING_TYPE_SPIKE_DROPPER, THING_FACING_RIGHT, 
                          pos_x, pos_y, 0, 0, pickup_timer_offset);
      }
      else if ( (b->object == GRID_OBJECT_SPIKE_RAISER_SPAWNER) && 
                (b->state == STATE_SPAWNER_FULLY_CHARGED)       && 
                (b->num_things == 0))
      {
        b->state = STATE_NONE;

        pos_x = (GRID_BOX_SIZE * m + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
        pos_y = (GRID_BOX_SIZE * n + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

        pickup_timer_offset = 240 - (((n + m) % ANIM_PICKUP_NUM_FRAMES) * ANIM_PICKUP_FRAME_LENGTH);

        world_spawn_thing(THING_TYPE_SPIKE_RAISER, THING_FACING_RIGHT, 
                          pos_x, pos_y, 0, 0, pickup_timer_offset);
      }
      else if ( (b->object == GRID_OBJECT_ARROW_SPINNER_CCW_SPAWNER)  && 
                (b->state == STATE_SPAWNER_FULLY_CHARGED)             && 
                (b->num_things == 0))
      {
        b->state = STATE_NONE;

        pos_x = (GRID_BOX_SIZE * m + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
        pos_y = (GRID_BOX_SIZE * n + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

        pickup_timer_offset = 240 - (((n + m) % ANIM_PICKUP_NUM_FRAMES) * ANIM_PICKUP_FRAME_LENGTH);

        world_spawn_thing(THING_TYPE_ARROW_SPINNER_CCW, THING_FACING_RIGHT, 
                          pos_x, pos_y, 0, 0, pickup_timer_offset);
      }
      else if ( (b->object == GRID_OBJECT_ARROW_SPINNER_CW_SPAWNER) && 
                (b->state == STATE_SPAWNER_FULLY_CHARGED)           && 
                (b->num_things == 0))
      {
        b->state = STATE_NONE;

        pos_x = (GRID_BOX_SIZE * m + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
        pos_y = (GRID_BOX_SIZE * n + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

        pickup_timer_offset = 240 - (((n + m) % ANIM_PICKUP_NUM_FRAMES) * ANIM_PICKUP_FRAME_LENGTH);

        world_spawn_thing(THING_TYPE_ARROW_SPINNER_CW, THING_FACING_RIGHT, 
                          pos_x, pos_y, 0, 0, pickup_timer_offset);
      }
    }
  }

  return 0;
}

/*******************************************************************************
** logic_freeze_thing()
*******************************************************************************/
short int logic_freeze_thing(int index)
{
  thing* t;

  grid_box* b;

  int box_x;
  int box_y;

  int cx;
  int cy;

  int amount_x;
  int amount_y;

  /* make sure index is valid */
  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  /* obtain thing pointer */
  t = &G_world_all_things[index];

  /* make sure this thing can be frozen */
  if (THING_IS_NOT_THE_PLAYER(t) && 
      THING_IS_NOT_A_DENIZEN(t))
  {
    return 0;
  }

  /* shades, knights, wisps, and collapsing mummies can't be frozen! */
  if ((t->type == THING_TYPE_SHADE)               || 
      (t->type == THING_TYPE_WISP_RT)             || 
      (t->type == THING_TYPE_WISP_LT)             || 
      (t->type == THING_TYPE_KNIGHT_RT)           || 
      (t->type == THING_TYPE_KNIGHT_LT)           || 
      (t->type == THING_TYPE_MUMMY_RT_COLLAPSING) || 
      (t->type == THING_TYPE_MUMMY_LT_COLLAPSING))
  {
    return 0;
  }

  /* determine grid box for the space where the ice cube will     */
  /* appear. we look at the front edge in the direction this      */
  /* thing is facing (so, if the thing is moving, it should snap  */
  /* to the grid on the box at the end of its current movement).  */
  if (t->facing == THING_FACING_RIGHT)
  {
    box_x = (t->pos_x + t->hx - 1) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = t->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else if (t->facing == THING_FACING_UP)
  {
    box_x = t->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = (t->pos_y - t->hy) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else if (t->facing == THING_FACING_LEFT)
  {
    box_x = (t->pos_x - t->hx) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = t->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else if (t->facing == THING_FACING_DOWN)
  {
    box_x = t->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = (t->pos_y + t->hy - 1) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else
    return 0;

  /* make sure the box index is valid */
  if ((box_x < 0) || (box_x >= GRID_WIDTH))
    return 0;

  if ((box_y < 0) || (box_y >= GRID_HEIGHT))
    return 0;

  b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

  /* compute box center position (in subpixels) */
  cx = ((box_x * GRID_BOX_SIZE) + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
  cy = ((box_y * GRID_BOX_SIZE) + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

  /* compute distance from center of grid box */
  amount_x = cx - t->pos_x;
  amount_y = cy - t->pos_y;

  /* snap thing to center of current grid box */

  /* horizontal move */
  if (amount_x != 0)
  {
    grid_move_thing(index, GRID_MOVE_THING_MODE_HORIZONTAL, amount_x);

    /* check if there was a collision */
    if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      t->state = STATE_AWAITING_SNAP_AND_FREEZE;
      return 0;
    }
  }

  /* vertical move */
  if (amount_y != 0)
  {
    grid_move_thing(index, GRID_MOVE_THING_MODE_VERTICAL, amount_y);

    /* check if there was a collision */
    if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      t->state = STATE_AWAITING_SNAP_AND_FREEZE;
      return 0;
    }
  }

  /* if this thing was frozen over water... splash! */
  if ((b->object == GRID_OBJECT_WATER)                  || 
      (b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING) || 
      (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING))
  {
    b->object = GRID_OBJECT_SUBMERGED_ICE_CUBE;
    b->state = STATE_NONE;
    b->timer_offset = 240 - G_timer_count;

    G_flag_redraw_grid_objects = 1;

    if (t->type == THING_TYPE_AUDREY)
      G_num_audrey_respawns += 1;

    if (THING_IS_THE_PLAYER(t))
      doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);

    t->type = THING_TYPE_PUFF_CYAN;
    t->state = STATE_NONE;
    t->timer_offset = 240 - G_timer_count;

    doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
  }
  /* otherwise, change this thing into an ice cube! */
  else
  {
    t->freeze_type = t->type;
    t->freeze_facing = t->facing;
    t->freeze_timer_offset = 240 - G_timer_count;

    t->type = THING_TYPE_ICE_CUBE;
    t->state = STATE_NONE;
    t->timer_offset = 0;
  }

  return 0;
}

/*******************************************************************************
** logic_thaw_thing()
*******************************************************************************/
short int logic_thaw_thing(int index)
{
  thing* t;

  grid_box* b;

  int box_x;
  int box_y;

  int cx;
  int cy;

  int amount_x;
  int amount_y;

  /* make sure index is valid */
  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  /* obtain thing pointer */
  t = &G_world_all_things[index];

  /* make sure this thing is an ice cube */
  if ((t->type != THING_TYPE_ICE_CUBE) && 
      (t->type != THING_TYPE_ICE_CUBE_THAWING))
  {
    return 0;
  }

  /* determine grid box for the space where the ice cube will     */
  /* thaw. we look at the front edge in the direction this        */
  /* thing is facing (so, if the thing is moving, it should snap  */
  /* to the grid on the box at the end of its current movement).  */
  if (t->facing == THING_FACING_RIGHT)
  {
    box_x = (t->pos_x + t->hx - 1) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = t->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else if (t->facing == THING_FACING_UP)
  {
    box_x = t->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = (t->pos_y - t->hy) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else if (t->facing == THING_FACING_LEFT)
  {
    box_x = (t->pos_x - t->hx) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = t->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else if (t->facing == THING_FACING_DOWN)
  {
    box_x = t->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
    box_y = (t->pos_y + t->hy - 1) / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  }
  else
    return 0;

  /* make sure the box index is valid */
  if ((box_x < 0) || (box_x >= GRID_WIDTH))
    return 0;

  if ((box_y < 0) || (box_y >= GRID_HEIGHT))
    return 0;

  b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

  /* compute box center position (in subpixels) */
  cx = ((box_x * GRID_BOX_SIZE) + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
  cy = ((box_y * GRID_BOX_SIZE) + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

  /* compute distance from center of grid box */
  amount_x = cx - t->pos_x;
  amount_y = cy - t->pos_y;

  /* snap thing to center of current grid box */

  /* horizontal move */
  if (amount_x != 0)
  {
    grid_move_thing(index, GRID_MOVE_THING_MODE_HORIZONTAL, amount_x);

    /* check if there was a collision */
    if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      t->state = STATE_AWAITING_SNAP_AND_THAW;
      return 0;
    }
  }

  /* vertical move */
  if (amount_y != 0)
  {
    grid_move_thing(index, GRID_MOVE_THING_MODE_VERTICAL, amount_y);

    /* check if there was a collision */
    if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      t->state = STATE_AWAITING_SNAP_AND_THAW;
      return 0;
    }
  }

  /* change this thing back into its previous form! */
  t->type = t->freeze_type;
  t->facing = t->freeze_facing;

  t->freeze_type = THING_TYPE_NONE;
  t->freeze_facing = THING_FACING_RIGHT;
  t->freeze_timer_offset = 0;

  t->state = STATE_NONE;
  t->timer_offset = 0;

  /* if this thing thawed out over water, and it is not a bat, despawn it */
  if ((b->object == GRID_OBJECT_WATER)                  || 
      (b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING) || 
      (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING))
  {
    if ((t->type != THING_TYPE_BAT) && 
        THING_IS_NOT_A_FLYING_DENIZEN(t))
    {
      if (t->type == THING_TYPE_AUDREY)
        G_num_audrey_respawns += 1;

      t->type = THING_TYPE_PUFF_CYAN;
      t->state = STATE_NONE;
      t->timer_offset = 240 - G_timer_count;

      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
  }

  /* if this thing is a mummy, set it to collapse */
  if (t->type == THING_TYPE_MUMMY_RT)
  {
    t->type = THING_TYPE_MUMMY_RT_COLLAPSING;
    t->state = STATE_NONE;
    t->timer_offset = 240 - G_timer_count;

    doremi_play_sfx(SFX_INDEX_MUMMY_COLLAPSE);
  }
  else if (t->type == THING_TYPE_MUMMY_LT)
  {
    t->type = THING_TYPE_MUMMY_LT_COLLAPSING;
    t->state = STATE_NONE;
    t->timer_offset = 240 - G_timer_count;

    doremi_play_sfx(SFX_INDEX_MUMMY_COLLAPSE);
  }

  return 0;
}

/*******************************************************************************
** logic_denizens_select_next_move()
*******************************************************************************/
short int logic_denizens_select_next_move()
{
  int k;

  thing* t;
  thing* p;

  unsigned int adjusted_timer_count;

  int completed_turn;

  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t = &G_world_all_things[k];

    /* check that there is a thing here */
    if (t->type == THING_TYPE_NONE)
      continue;

    /* make sure this thing is not already moving, casting, etc */
    if ((t->state == STATE_STARTED_MOVING)            || 
        (t->state == STATE_MOVING_ONE_SPACE)          || 
        (t->state == STATE_CASTING)                   || 
        (t->state == STATE_AWAITING_SNAP_TO_GRID)     || 
        (t->state == STATE_AWAITING_SNAP_AND_FREEZE)  || 
        (t->state == STATE_AWAITING_SNAP_AND_THAW))
    {
      continue;
    }

    /* determine adjusted timer count */
    adjusted_timer_count = (G_timer_count + t->timer_offset) % 240;

    /* initialize completed turn flag */
    completed_turn = 0;

    /* check if this denizen is completing a turn */
    if (t->state == STATE_TURNING)
    {
      if (adjusted_timer_count >= THING_DENIZEN_TURN_TIME)
      {
        /* back and forth */
        if ((t->type == THING_TYPE_IMP_AWAKE) || 
            (t->type == THING_TYPE_LIL_BOT))
        {
          if (t->facing == THING_FACING_RIGHT)
            t->facing = THING_FACING_LEFT;
          else if (t->facing == THING_FACING_UP)
            t->facing = THING_FACING_DOWN;
          else if (t->facing == THING_FACING_LEFT)
            t->facing = THING_FACING_RIGHT;
          else if (t->facing == THING_FACING_DOWN)
            t->facing = THING_FACING_UP;
        }
        /* right turn */
        else if ( (t->type == THING_TYPE_WISP_RT)   || 
                  (t->type == THING_TYPE_KNIGHT_RT) || 
                  (t->type == THING_TYPE_MUMMY_RT)  || 
                  (t->type == THING_TYPE_JIANGSHI_RT))
        {
          if (t->facing == THING_FACING_RIGHT)
            t->facing = THING_FACING_DOWN;
          else if (t->facing == THING_FACING_UP)
            t->facing = THING_FACING_RIGHT;
          else if (t->facing == THING_FACING_LEFT)
            t->facing = THING_FACING_UP;
          else if (t->facing == THING_FACING_DOWN)
            t->facing = THING_FACING_LEFT;
        }
        /* left turn */
        else if ( (t->type == THING_TYPE_WISP_LT)   || 
                  (t->type == THING_TYPE_KNIGHT_LT) || 
                  (t->type == THING_TYPE_MUMMY_LT)  || 
                  (t->type == THING_TYPE_JIANGSHI_LT))
        {
          if (t->facing == THING_FACING_RIGHT)
            t->facing = THING_FACING_UP;
          else if (t->facing == THING_FACING_UP)
            t->facing = THING_FACING_LEFT;
          else if (t->facing == THING_FACING_LEFT)
            t->facing = THING_FACING_DOWN;
          else if (t->facing == THING_FACING_DOWN)
            t->facing = THING_FACING_RIGHT;
        }

        t->state = STATE_NONE;
        t->timer_offset = 0;

        completed_turn = 1;
      }
      else
        continue;
    }
    /* if we were blocked in, just try to move    */
    /* again (this is for the lil bots and imps). */
    else if (t->state == STATE_BLOCKED_IN)
    {
      if (adjusted_timer_count >= THING_DENIZEN_TURN_TIME)
      {
        t->state = STATE_NONE;
        t->timer_offset = 0;
      }
      else
        continue;
    }

    /* imps, lil bots, wisps, knights, mummies, jiangshis */
    if ((t->type == THING_TYPE_IMP_AWAKE)   || 
        (t->type == THING_TYPE_LIL_BOT)     || 
        (t->type == THING_TYPE_WISP_RT)     || 
        (t->type == THING_TYPE_WISP_LT)     || 
        (t->type == THING_TYPE_KNIGHT_RT)   || 
        (t->type == THING_TYPE_KNIGHT_LT)   || 
        (t->type == THING_TYPE_MUMMY_RT)    || 
        (t->type == THING_TYPE_MUMMY_LT)    || 
        (t->type == THING_TYPE_JIANGSHI_RT) || 
        (t->type == THING_TYPE_JIANGSHI_LT))
    {
      logic_attempt_move(k, t->facing);

      /* jiangshis stop if the run into something */
      if ((t->type == THING_TYPE_JIANGSHI_RT) && 
          (t->state == STATE_READY_TO_CHANGE))
      {
        t->type = THING_TYPE_JIANGSHI_RT_STOPPED;
        t->state = STATE_CHANGING;
        t->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_JIANGSHI_STOP);
      }
      else if ( (t->type == THING_TYPE_JIANGSHI_LT) && 
                (t->state == STATE_READY_TO_CHANGE))
      {
        t->type = THING_TYPE_JIANGSHI_LT_STOPPED;
        t->state = STATE_CHANGING;
        t->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_JIANGSHI_STOP);
      }
      /* wisps dissipate if they run into something */
      else if ( (t->type == THING_TYPE_WISP_RT) && 
                (t->state == STATE_READY_TO_CHANGE))
      {
        t->type = THING_TYPE_PUFF_GREY;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
      }
      else if ( (t->type == THING_TYPE_WISP_LT) && 
                (t->state == STATE_READY_TO_CHANGE))
      {
        t->type = THING_TYPE_PUFF_GREY;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
      }
      /* otherwise... */
      else if (t->state != STATE_STARTED_MOVING)
      {
        t->state = STATE_TURNING;
        t->timer_offset = 240 - G_timer_count;

        /* if this is an imp or a lil bot, and we just turned */
        /* this frame, reset the facing to cancel the turn.   */

        /* (this just makes things look better visually, as flipping  */
        /* back and forth between the front and back frames can look  */
        /* awkward, seeing as they have the same silhouette).         */
        if ((t->type == THING_TYPE_IMP_AWAKE) || 
            (t->type == THING_TYPE_LIL_BOT))
        {
          if (completed_turn == 1)
          {
            if (t->facing == THING_FACING_RIGHT)
              t->facing = THING_FACING_LEFT;
            else if (t->facing == THING_FACING_UP)
              t->facing = THING_FACING_DOWN;
            else if (t->facing == THING_FACING_LEFT)
              t->facing = THING_FACING_RIGHT;
            else if (t->facing == THING_FACING_DOWN)
              t->facing = THING_FACING_UP;

            t->state = STATE_BLOCKED_IN;
          }
        }
      }
    }
    else if (t->type == THING_TYPE_FAERIE_HORIZONTAL)
    {
      /* determine player position */
      p = &G_world_all_things[WORLD_VAMPIRE_INDEX];

      if (THING_IS_NOT_THE_PLAYER(p))
        continue;

      /* move left/right to block the player */
      if (p->pos_x < t->pos_x - t->hx)
      {
        logic_attempt_move(k, THING_FACING_LEFT);

        if (t->state != STATE_STARTED_MOVING)
          t->facing = THING_FACING_DOWN;
      }
      else if (p->pos_x > t->pos_x + t->hx - 1)
      {
        logic_attempt_move(k, THING_FACING_RIGHT);

        if (t->state != STATE_STARTED_MOVING)
          t->facing = THING_FACING_DOWN;
      }
      else if (t->state != STATE_STARTED_MOVING)
        t->facing = THING_FACING_DOWN;
    }
    else if (t->type == THING_TYPE_FAERIE_VERTICAL)
    {
      /* determine player position */
      p = &G_world_all_things[WORLD_VAMPIRE_INDEX];

      if (THING_IS_NOT_THE_PLAYER(p))
        continue;

      /* move up/down to block the player */
      if (p->pos_y < t->pos_y - t->hy)
      {
        logic_attempt_move(k, THING_FACING_UP);

        if (t->state != STATE_STARTED_MOVING)
          t->facing = THING_FACING_DOWN;
      }
      else if (p->pos_y > t->pos_y + t->hy - 1)
      {
        logic_attempt_move(k, THING_FACING_DOWN);

        if (t->state != STATE_STARTED_MOVING)
          t->facing = THING_FACING_DOWN;
      }
      else if (t->state != STATE_STARTED_MOVING)
        t->facing = THING_FACING_DOWN;
    }
  }

  return 0;
}

/*******************************************************************************
** logic_complete_move()
*******************************************************************************/
short int logic_complete_move(int index)
{
  thing* t;

  grid_box* b;

  int box_x;
  int box_y;

  int cx;
  int cy;

  int amount_x;
  int amount_y;

  /* make sure index is valid */
  if ((index < 0) || (index >= WORLD_MAX_THINGS))
    return 0;

  /* obtain thing pointer */
  t = &G_world_all_things[index];

  /* make sure this thing is has stopped moving,  */
  /* or is awaiting a snap to the grid.           */
  if (t->state != STATE_STOPPED_MOVING)
  {
    return 0;
  }

  /* determine grid box for space we are currently in */
  box_x = t->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
  box_y = t->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);

  /* make sure the box index is valid */
  if ((box_x < 0) || (box_x >= GRID_WIDTH))
    return 0;

  if ((box_y < 0) || (box_y >= GRID_HEIGHT))
    return 0;

  b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

  /* compute box center position (in subpixels) */
  cx = ((box_x * GRID_BOX_SIZE) + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;
  cy = ((box_y * GRID_BOX_SIZE) + GRID_BOX_SIZE_HALF) * THING_NUM_SUBPIXELS;

  /* compute distance from center of grid box */
  amount_x = cx - t->pos_x;
  amount_y = cy - t->pos_y;

  /* snap thing to center of current grid box */

  /* horizontal move */
  if (amount_x != 0)
  {
    grid_move_thing(index, GRID_MOVE_THING_MODE_HORIZONTAL, amount_x);

    /* check if there was a collision */
    if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      t->state = STATE_AWAITING_SNAP_TO_GRID;
      return 0;
    }
  }

  /* vertical move */
  if (amount_y != 0)
  {
    grid_move_thing(index, GRID_MOVE_THING_MODE_VERTICAL, amount_y);

    /* check if there was a collision */
    if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      t->state = STATE_AWAITING_SNAP_TO_GRID;
      return 0;
    }
  }

  /* check out the grid box where we stopped! */

  /* water, collapsing bridges */
  if ((b->object == GRID_OBJECT_WATER)                  || 
      (b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING) || 
      (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING))
  {
    if (t->type == THING_TYPE_ICE_CUBE)
    {
      b->object = GRID_OBJECT_SUBMERGED_ICE_CUBE;
      b->state = STATE_NONE;
      b->timer_offset = t->freeze_timer_offset;

      G_flag_redraw_grid_objects = 1;

      if (t->freeze_type == THING_TYPE_AUDREY)
        G_num_audrey_respawns += 1;

      if ((t->freeze_type == THING_TYPE_VAMPIRE) || 
          (t->freeze_type == THING_TYPE_BAT))
      {
        doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);

        t->type = THING_TYPE_PUFF_CYAN;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;
      }
      else
        world_despawn_thing(index);

      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
    else if (t->type == THING_TYPE_ICE_CUBE_THAWING)
    {
      b->object = GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING;
      b->state = STATE_NONE;
      b->timer_offset = t->freeze_timer_offset;

      G_flag_redraw_grid_objects = 1;

      if (t->freeze_type == THING_TYPE_AUDREY)
        G_num_audrey_respawns += 1;

      if ((t->freeze_type == THING_TYPE_VAMPIRE) || 
          (t->freeze_type == THING_TYPE_BAT))
      {
        doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);

        t->type = THING_TYPE_PUFF_CYAN;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;
      }
      else
        world_despawn_thing(index);

      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
    else if (t->type == THING_TYPE_BOX_WOOD)
    {
      b->object = GRID_OBJECT_SUBMERGED_BOX_WOOD;
      b->state = STATE_NONE;
      b->timer_offset = 0;

      G_flag_redraw_grid_objects = 1;

      world_despawn_thing(index);
      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
    else if (t->type == THING_TYPE_BOX_METAL)
    {
      b->object = GRID_OBJECT_SUBMERGED_BOX_METAL;
      b->state = STATE_NONE;
      b->timer_offset = 0;

      G_flag_redraw_grid_objects = 1;

      world_despawn_thing(index);
      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
    else if (t->type == THING_TYPE_BOX_STONE)
    {
      b->object = GRID_OBJECT_SUBMERGED_BOX_STONE;
      b->state = STATE_NONE;
      b->timer_offset = 0;

      G_flag_redraw_grid_objects = 1;

      world_despawn_thing(index);
      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
    else if ( (t->type == THING_TYPE_POT_WOOD)    || 
              (t->type == THING_TYPE_POT_METAL)   || 
              (t->type == THING_TYPE_POT_STONE)   || 
              (t->type == THING_TYPE_ACID_FLASK)  || 
              (t->type == THING_TYPE_STONE_FLASK))
    {
      world_despawn_thing(index);
      doremi_play_sfx(SFX_INDEX_WATER_SPLASH);
    }
  }
  /* bridges */
  else if ( (b->object == GRID_OBJECT_BRIDGE_VERT) || 
            (b->object == GRID_OBJECT_BRIDGE_HORI))
  {
    if ((t->type == THING_TYPE_VAMPIRE) || 
        THING_IS_A_WALKING_DENIZEN(t))
    {
      b->state = STATE_READY_TO_CHANGE;
    }
  }
  /* lowered floor spikes */
  else if (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN)
  {
    if ((t->type == THING_TYPE_VAMPIRE) || 
        THING_IS_A_WALKING_DENIZEN(t))
    {
      b->state = STATE_READY_TO_CHANGE;
    }
  }
  /* raised floor spikes */
  else if (b->object == GRID_OBJECT_FLOOR_SPIKES_UP)
  {
    if ((t->type == THING_TYPE_BOX_METAL) || 
        (t->type == THING_TYPE_POT_METAL))
    {
      b->object = GRID_OBJECT_NONE;
      b->state = STATE_NONE;
      b->timer_offset = 0;

      G_flag_redraw_grid_objects = 1;

      doremi_play_sfx(SFX_INDEX_SPIKE_BREAK);
    }
    else if ( (t->type == THING_TYPE_ACID_FLASK) || 
              (t->type == THING_TYPE_STONE_FLASK))
    {
      b->object = GRID_OBJECT_NONE;
      b->state = STATE_NONE;
      b->timer_offset = 0;

      G_flag_redraw_grid_objects = 1;

      t->type = THING_TYPE_PUFF_GREY;
      t->state = STATE_NONE;
      t->timer_offset = 240 - G_timer_count;

      doremi_play_sfx(SFX_INDEX_SPIKE_BREAK);
    }
  }

  return 0;
}

/*******************************************************************************
** logic_move_things()
*******************************************************************************/
short int logic_move_things()
{
  int k;

  thing* t;

  unsigned int adjusted_timer_count;

  /* move the pushable objects, denizens, and player! */

  for (k = 0; k < WORLD_MAX_THINGS; k++)
  {
    t = &G_world_all_things[k];

    /* check that there is a thing here */
    if (t->type == THING_TYPE_NONE)
      continue;

    /* if this is a pickup or spell, ignore */
    if (THING_IS_A_PICKUP(t) || 
        THING_IS_A_SPELL(t))
      continue;

    /* for all other things, we want to move  */
    /* in increments of one whole grid space  */

    /* determine adjusted timer count */
    adjusted_timer_count = (G_timer_count + t->timer_offset) % 240;

    /* complete move start, move stop, or move thing if necessary */
    if (t->state == STATE_STARTED_MOVING)
    {
      t->state = STATE_MOVING_ONE_SPACE;
    }
    else if (t->state == STATE_MOVING_ONE_SPACE)
    {
      if (t->vel_x != 0)
        grid_move_thing(k, GRID_MOVE_THING_MODE_HORIZONTAL, t->vel_x);
      else if (t->vel_y != 0)
        grid_move_thing(k, GRID_MOVE_THING_MODE_VERTICAL, t->vel_y);

      if (adjusted_timer_count >= THING_DENIZEN_MOVE_TIME - 1)
      {
        t->state = STATE_STOPPED_MOVING;

        t->vel_x = 0;
        t->vel_y = 0;
        t->timer_offset = 0;

        logic_complete_move(k);
      }
    }
    else if (t->state == STATE_AWAITING_SNAP_TO_GRID)
    {
      logic_complete_move(k);
    }
    else if (t->state == STATE_AWAITING_SNAP_AND_FREEZE)
    {
      logic_freeze_thing(k);
    }
    else if (t->state == STATE_AWAITING_SNAP_AND_THAW)
    {
      logic_thaw_thing(k);
    }
    else if (t->state == STATE_STOPPED_MOVING)
    {
      t->state = STATE_NONE;
    }
  }

  /* move the spells! */

  /* we do these afterward, so that the correct collision flags are */
  /* set on the other things (denizens, etc). otherwise, the flags  */
  /* are always cleared at the start of their grid_move_thing().    */
  for (k = WORLD_SPELLS_START_INDEX; k < WORLD_SPELLS_END_INDEX; k++)
  {
    t = &G_world_all_things[k];

    /* check that there is a thing here */
    if (t->type == THING_TYPE_NONE)
      continue;

    /* if this is a spell, just apply the move each frame */
    if (THING_IS_A_SPELL(t))
    {
      if (t->vel_x != 0)
        grid_move_thing(k, GRID_MOVE_THING_MODE_HORIZONTAL, t->vel_x);
      else if (t->vel_y != 0)
        grid_move_thing(k, GRID_MOVE_THING_MODE_VERTICAL, t->vel_y);
    }
  }

  return 0;
}

/*******************************************************************************
** logic_complete_spell_casts()
*******************************************************************************/
short int logic_complete_spell_casts()
{
  int k;

  thing* t;

  unsigned int adjusted_timer_count;

  /* check for completed spell casts amongst the denizens */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t = &G_world_all_things[k];

    /* determine adjusted timer count */
    adjusted_timer_count = (G_timer_count + t->timer_offset) % 240;

    if (THING_IS_A_SPELLCASTING_DENIZEN(t)            || 
        (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
        (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE))
    {
      if ((t->state == STATE_CASTING) &&
          (adjusted_timer_count >= THING_DENIZEN_CAST_TIME))
      {
        t->state = STATE_NONE;
        t->timer_offset = 0;
      }
      else if ( (t->state == STATE_WAKEUP) &&
                (adjusted_timer_count >= THING_DENIZEN_WAKEUP_TIME))
      {
        t->state = STATE_NONE;
        t->timer_offset = 0;
      }
    }
  }

  /* check for completed spell casts with the player */
  t = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* determine adjusted timer count */
  adjusted_timer_count = (G_timer_count + t->timer_offset) % 240;

  if (t->type == THING_TYPE_VAMPIRE)
  {
    if ((t->state == STATE_CASTING) &&
        (adjusted_timer_count >= THING_DENIZEN_CAST_TIME))
    {
      t->state = STATE_NONE;
      t->timer_offset = 0;
    }
  }

  return 0;
}

/*******************************************************************************
** logic_complete_thing_lifetimes()
*******************************************************************************/
short int logic_complete_thing_lifetimes()
{
  int k;

  thing* t;

  unsigned int adjusted_timer_count;
  unsigned int freeze_timer_count;

  /* check for completed thing lifetimes */
  for (k = 0; k < WORLD_MAX_THINGS; k++)
  {
    t = &G_world_all_things[k];

    /* determine adjusted timer counts */
    adjusted_timer_count = (G_timer_count + t->timer_offset) % 240;
    freeze_timer_count = (G_timer_count + t->freeze_timer_offset) % 240;

    /* complete mummy collapse if necessary */
    if ((t->type == THING_TYPE_MUMMY_RT_COLLAPSING) || 
        (t->type == THING_TYPE_MUMMY_LT_COLLAPSING))
    {
      if (adjusted_timer_count >= LIFETIME_MUMMY_COLLAPSING)
      {
        world_despawn_thing(k);
        doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
      }
    }
    /* complete jiangshi stop if necessary */
    else if ( (t->type == THING_TYPE_JIANGSHI_RT_STOPPED) && 
              (t->state == STATE_CHANGING))
    {
      if (adjusted_timer_count >= LIFETIME_JIANGSHI_STOPPING)
      {
        t->state = STATE_NONE;
        t->timer_offset = 0;
      }
    }
    else if ( (t->type == THING_TYPE_JIANGSHI_LT_STOPPED) && 
              (t->state == STATE_CHANGING))
    {
      if (adjusted_timer_count >= LIFETIME_JIANGSHI_STOPPING)
      {
        t->state = STATE_NONE;
        t->timer_offset = 0;
      }
    }
    /* start ice cube thaw if necessary */
    else if (t->type == THING_TYPE_ICE_CUBE)
    {
      if (freeze_timer_count >= LIFETIME_ICE_CUBE)
      {
        t->type = THING_TYPE_ICE_CUBE_THAWING;
        t->freeze_timer_offset = 240 - G_timer_count;
      }
    }
    else if (t->type == THING_TYPE_ICE_CUBE_THAWING)
    {
      if (freeze_timer_count >= LIFETIME_ICE_CUBE)
      {
        logic_thaw_thing(k);
      }
    }
    /* complete puff if necessary */
    else if (THING_IS_A_PUFF(t))
    {
      if (adjusted_timer_count >= LIFETIME_PUFF)
      {
        world_despawn_thing(k);
      }
    }
  }

  return 0;
}

/*******************************************************************************
** logic_process_spell_collisions()
*******************************************************************************/
short int logic_process_spell_collisions()
{
  int k;

  thing* t;

  for (k = 0; k < WORLD_MAX_THINGS; k++)
  {
    t = &G_world_all_things[k];

    /* check that there is a thing here */
    if (t->type == THING_TYPE_NONE)
      continue;

    /* if this is a pickup, ignore */
    if (THING_IS_A_PICKUP(t))
      continue;

    /* evil eye spells */
    if (t->flag == THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL)
    {
      if (THING_IS_THE_PLAYER(t))
      {
        t->type = THING_TYPE_PUFF_GREY;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);
      }
    }
    /* fire spells */
    else if (t->flag == THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL)
    {
      if (THING_CAN_BE_BURNED(t))
      {
        if (THING_IS_THE_PLAYER(t))
          doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);
        else if (THING_IS_A_DENIZEN(t))
          doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
        else
          doremi_play_sfx(SFX_INDEX_THING_SAYS_BYE_BYE);

        if (t->type == THING_TYPE_AUDREY)
          G_num_audrey_respawns += 1;

        t->type = THING_TYPE_PUFF_ORANGE;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;
      }
      else if ( (t->type == THING_TYPE_ICE_CUBE) || 
                (t->type == THING_TYPE_ICE_CUBE_THAWING))
      {
        logic_thaw_thing(k);
      }
    }
    /* ice spells */
    else if (t->flag == THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL)
    {
      if (THING_CAN_BE_FROZEN(t))
      {
        logic_freeze_thing(k);
        doremi_play_sfx(SFX_INDEX_FREEZING);
      }
      else if ( (t->type == THING_TYPE_WISP_RT) || 
                (t->type == THING_TYPE_WISP_LT))
      {
        t->type = THING_TYPE_PUFF_CYAN;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
      }
      else if ( (t->type == THING_TYPE_ICE_CUBE) || 
                (t->type == THING_TYPE_ICE_CUBE_THAWING))
      {
        t->type = THING_TYPE_ICE_CUBE;
        t->freeze_timer_offset = 240 - G_timer_count;

        doremi_play_sfx(SFX_INDEX_FREEZING);
      }
    }
    /* ring spells */
    else if (t->flag == THING_COLLISION_FLAG_HIT_WITH_RING_SPELL)
    {
      if (THING_CAN_BE_DAMAGED(t))
      {
        if (THING_IS_THE_PLAYER(t))
          doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);
        else if (THING_IS_A_DENIZEN(t))
          doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
        else
          doremi_play_sfx(SFX_INDEX_THING_SAYS_BYE_BYE);

        if (t->type == THING_TYPE_AUDREY)
          G_num_audrey_respawns += 1;

        t->type = THING_TYPE_PUFF_PURPLE;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;
      }
    }
    /* twin spells */
    else if (t->flag == THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL)
    {
      if (THING_CAN_BE_DAMAGED(t))
      {
        if (THING_IS_THE_PLAYER(t))
          doremi_play_sfx(SFX_INDEX_PLAYER_SAYS_BYE_BYE);
        else if (THING_IS_A_DENIZEN(t))
          doremi_play_sfx(SFX_INDEX_DENIZEN_SAYS_BYE_BYE);
        else
          doremi_play_sfx(SFX_INDEX_THING_SAYS_BYE_BYE);

        if (t->type == THING_TYPE_AUDREY)
          G_num_audrey_respawns += 1;

        t->type = THING_TYPE_PUFF_GREEN;
        t->state = STATE_NONE;
        t->timer_offset = 240 - G_timer_count;
      }
    }
    /* general collisions */
    else if (t->flag == THING_COLLISION_FLAG_WE_HIT_SOMETHING)
    {
      if (THING_IS_A_SPELL(t))
        world_despawn_thing(k);
    }

    t->flag = THING_COLLISION_FLAG_NONE;
  }

  return 0;
}

/*******************************************************************************
** logic_spellcasters_update()
*******************************************************************************/
short int logic_spellcasters_update()
{
  int m;
  int n;

  int k;

  thing* t1;
  thing* t2;
  thing* t3;

  grid_box* b;

  int box_caster_x;
  int box_caster_y;

  int box_player_x;
  int box_player_y;

  int box_indices[16];
  int num_boxes;

  int cast_check;
  int path_clear;

  /* obtain player */
  t2 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  if (THING_IS_NOT_THE_PLAYER(t2))
    return 0;

  /* check the denizens for spellcasters */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t1 = &G_world_all_things[k];

    /* check that there is a thing here */
    if (t1->type == THING_TYPE_NONE)
      continue;

    if (THING_IS_A_SPELLCASTING_DENIZEN(t1))
    {
      /* if already casting, skip this denizen */
      if (t1->state == STATE_CASTING)
        continue;

      /* if still waking up, skip this denizen */
      if (t1->state == STATE_WAKEUP)
        continue;

      /* initialize casting check variable */
      cast_check = 0;

      /* for pumpkins, they check in four directions */
      if (t1->type == THING_TYPE_PUMPKIN)
      {
        if ((t2->pos_x >= t1->pos_x - t1->hx) && 
            (t2->pos_x < t1->pos_x + t1->hx))
        {
          if (t2->pos_y < t1->pos_y)
            t1->facing = THING_FACING_UP;
          else
            t1->facing = THING_FACING_DOWN;

          cast_check = 1;
        }
        else if ( (t2->pos_y >= t1->pos_y - t1->hy) && 
                  (t2->pos_y < t1->pos_y + t1->hy))
        {
          if (t2->pos_x < t1->pos_x)
            t1->facing = THING_FACING_LEFT;
          else
            t1->facing = THING_FACING_RIGHT;

          cast_check = 1;
        }
      }
      /* for other spellcasters, they check in the direction they're facing */
      else if ( (t1->facing == THING_FACING_RIGHT)  && 
                (t2->pos_y >= t1->pos_y - t1->hy)   && 
                (t2->pos_y < t1->pos_y + t1->hy)    && 
                (t2->pos_x > t1->pos_x))
      {
        cast_check = 1;
      }
      else if ( (t1->facing == THING_FACING_UP)     && 
                (t2->pos_x >= t1->pos_x - t1->hx)   && 
                (t2->pos_x < t1->pos_x + t1->hx)    && 
                (t2->pos_y < t1->pos_y))
      {
        cast_check = 1;
      }
      else if ( (t1->facing == THING_FACING_LEFT)   && 
                (t2->pos_y >= t1->pos_y - t1->hy)   && 
                (t2->pos_y < t1->pos_y + t1->hy)    && 
                (t2->pos_x < t1->pos_x))
      {
        cast_check = 1;
      }
      else if ( (t1->facing == THING_FACING_DOWN)   && 
                (t2->pos_x >= t1->pos_x - t1->hx)   && 
                (t2->pos_x < t1->pos_x + t1->hx)    && 
                (t2->pos_y > t1->pos_y))
      {
        cast_check = 1;
      }

      /* if the casting check passed, see if the path is clear */
      if (cast_check == 1)
      {
        /* determine grid box for the caster */
        box_caster_x = t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
        box_caster_y = t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);

        /* determine grid box for the player */
        box_player_x = t2->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);
        box_player_y = t2->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE);

        /* make sure the box indices valid */
        if ((box_caster_x < 0) || (box_caster_x >= GRID_WIDTH))
          return 0;

        if ((box_caster_y < 0) || (box_caster_y >= GRID_HEIGHT))
          return 0;

        if ((box_player_x < 0) || (box_player_x >= GRID_WIDTH))
          return 0;

        if ((box_player_y < 0) || (box_player_y >= GRID_HEIGHT))
          return 0;

        /* setup box indices */
        if (t1->facing == THING_FACING_RIGHT)
        {
          num_boxes = box_player_x - box_caster_x - 1;

          if ((num_boxes < 0) || (num_boxes > 16))
            return 0;

          for (m = 0; m < num_boxes; m++)
            box_indices[m] = (box_caster_y * GRID_WIDTH) + box_caster_x + 1 + m;
        }
        else if (t1->facing == THING_FACING_UP)
        {
          num_boxes = box_caster_y - box_player_y - 1;

          if ((num_boxes < 0) || (num_boxes > 16))
            return 0;

          for (m = 0; m < num_boxes; m++)
            box_indices[m] = ((box_caster_y - 1 - m) * GRID_WIDTH) + box_caster_x;
        }
        else if (t1->facing == THING_FACING_LEFT)
        {
          num_boxes = box_caster_x - box_player_x - 1;

          if ((num_boxes < 0) || (num_boxes > 16))
            return 0;

          for (m = 0; m < num_boxes; m++)
            box_indices[m] = (box_caster_y * GRID_WIDTH) + box_caster_x - 1 - m;
        }
        else if (t1->facing == THING_FACING_DOWN)
        {
          num_boxes = box_player_y - box_caster_y - 1;

          if ((num_boxes < 0) || (num_boxes > 16))
            return 0;

          for (m = 0; m < num_boxes; m++)
            box_indices[m] = ((box_caster_y + 1 + m) * GRID_WIDTH) + box_caster_x;
        }
        else
          return 0;

        /* initialize path clear check */
        path_clear = 1;

        /* check through the boxes between the caster and the player */
        for (m = 0; m < num_boxes; m++)
        {
          b = &G_collision_grid[box_indices[m]];

          /* see if there is a grid object that will block this spell */
          if ((t1->type == THING_TYPE_PUMPKIN)      || 
              (t1->type == THING_TYPE_GHOST_AWAKE)  || 
              (t1->type == THING_TYPE_SORCERESS))
          {
            if (GRID_OBJECT_WILL_BLOCK_EVIL_EYE_SPELLS(b))
              path_clear = 0;
          }
          else
          {
            if (GRID_OBJECT_WILL_BLOCK_OTHER_SPELLS(b))
              path_clear = 0;
          }

          /* see if there is a thing that will block this spell */
          for (n = 0; n < b->num_things; n++)
          {
            t3 = &G_world_all_things[b->thing_indices[n]];

            if ((t1->type == THING_TYPE_PUMPKIN) || 
                (t1->type == THING_TYPE_GHOST_AWAKE))
            {
              if ((t3->type == THING_TYPE_ICE_CUBE)         || 
                  (t3->type == THING_TYPE_ICE_CUBE_THAWING) || 
                  (t3->type == THING_TYPE_BOX_WOOD)         || 
                  (t3->type == THING_TYPE_BOX_METAL)        || 
                  (t3->type == THING_TYPE_BOX_STONE)        || 
                  THING_IS_A_DENIZEN(t3)                    || 
                  THING_IS_A_SPELL(t3))
              {
                path_clear = 0;
              }
            }
            else if (t1->type == THING_TYPE_SNAKE_AWAKE)
            {
              if ((t3->type == THING_TYPE_BOX_METAL)    || 
                  (t3->type == THING_TYPE_BOX_STONE)    || 
                  (t3->type == THING_TYPE_POT_METAL)    || 
                  (t3->type == THING_TYPE_POT_STONE)    || 
                  (t3->type == THING_TYPE_STONE_FLASK)  || 
                  THING_IS_A_PICKUP(t3)                 || 
                  THING_IS_A_SPELL(t3))
              {
                path_clear = 0;
              }
            }
            else if (t1->type == THING_TYPE_SHADE)
            {
              if (THING_IS_A_PUSHABLE_OBJECT(t3)  || 
                  THING_IS_A_PICKUP(t3)           || 
                  THING_IS_A_SPELL(t3))
              {
                path_clear = 0;
              }
            }
            else if (t1->type == THING_TYPE_WITCH_AWAKE)
            {
              if ((t3->type == THING_TYPE_ICE_CUBE)         || 
                  (t3->type == THING_TYPE_ICE_CUBE_THAWING) || 
                  (t3->type == THING_TYPE_BOX_METAL)        || 
                  (t3->type == THING_TYPE_BOX_STONE)        || 
                  (t3->type == THING_TYPE_POT_METAL)        || 
                  (t3->type == THING_TYPE_POT_STONE)        || 
                  (t3->type == THING_TYPE_STONE_FLASK)      || 
                  THING_IS_A_PICKUP(t3)                     || 
                  THING_IS_A_SPELL(t3))
              {
                path_clear = 0;
              }
            }
            else if (t1->type == THING_TYPE_SORCERESS)
            {
              if ((t3->type == THING_TYPE_ICE_CUBE)         || 
                  (t3->type == THING_TYPE_ICE_CUBE_THAWING) || 
                  (t3->type == THING_TYPE_BOX_METAL)        || 
                  (t3->type == THING_TYPE_BOX_STONE)        || 
                  THING_IS_A_SPELL(t3))
              {
                path_clear = 0;
              }
            }
          }
        }

        /* if the path is clear, cast the spell! */
        if (path_clear == 1)
          logic_attempt_spell_cast(k);
      }
    }
  }

  return 0;
}

/*******************************************************************************
** logic_all_vials_collected()
*******************************************************************************/
short int logic_all_vials_collected()
{
  grid_box* b;

  thing*    t1;

  int       m;
  int       n;

  int       k;

  /* open the portcullis */
  for (n = 0; n < GRID_HEIGHT; n++)
  {
    for (m = 0; m < GRID_WIDTH; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH) + m];

      if (b->object == GRID_OBJECT_PORTCULLIS_CLOSED)
      {
        b->state = STATE_READY_TO_CHANGE;
      }
    }
  }

  /* awake all denizens */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t1 = &G_world_all_things[k];

    if (t1->type == THING_TYPE_GHOST_ASLEEP)
      t1->type = THING_TYPE_GHOST_AWAKE;
    else if (t1->type == THING_TYPE_SNAKE_ASLEEP)
      t1->type = THING_TYPE_SNAKE_AWAKE;
    else if (t1->type == THING_TYPE_WITCH_ASLEEP)
      t1->type = THING_TYPE_WITCH_AWAKE;
    else if (t1->type == THING_TYPE_IMP_ASLEEP)
      t1->type = THING_TYPE_IMP_AWAKE;
    else if ( (t1->type == THING_TYPE_ICE_CUBE) || 
              (t1->type == THING_TYPE_ICE_CUBE_THAWING))
    {
      if (t1->freeze_type == THING_TYPE_GHOST_ASLEEP)
        t1->freeze_type = THING_TYPE_GHOST_AWAKE;
      else if (t1->freeze_type == THING_TYPE_SNAKE_ASLEEP)
        t1->freeze_type = THING_TYPE_SNAKE_AWAKE;
      else if (t1->freeze_type == THING_TYPE_WITCH_ASLEEP)
        t1->freeze_type = THING_TYPE_WITCH_AWAKE;
      else if (t1->freeze_type == THING_TYPE_IMP_ASLEEP)
        t1->freeze_type = THING_TYPE_IMP_AWAKE;
    }

    /* set initial state for awakened spellcasters */
    if ((t1->type == THING_TYPE_GHOST_AWAKE) || 
        (t1->type == THING_TYPE_SNAKE_AWAKE) || 
        (t1->type == THING_TYPE_WITCH_AWAKE))
    {
      t1->state = STATE_WAKEUP;
      t1->timer_offset = 240 - G_timer_count;
    }
  }

  return 0;
}

/*******************************************************************************
** logic_collect_pickups()
*******************************************************************************/
short int logic_collect_pickups()
{
  grid_box* b;

  thing*    t1;

  int       m;
  int       n;

  int       k;

  int       box_x;
  int       box_y;

  int       spike_sfx_check;

  /* cycle through all pickups */
  for ( k = WORLD_PICKUPS_START_INDEX; 
        k < WORLD_PICKUPS_END_INDEX; 
        k++)
  {
    t1 = &G_world_all_things[k];

    if (t1->type == THING_TYPE_NONE)
      continue;

    /* check if this pickup was collided with by the player */
    if (t1->flag == THING_COLLISION_FLAG_PICKUP_COLLECTED)
    {
      if (t1->type == THING_TYPE_BLOOD_VIAL)
      {
        G_vials_remaining -= 1;

        /* play vial pickup sound */
        if (G_vials_remaining > 0)
          doremi_play_sfx(SFX_INDEX_PICKUP_VIAL);
        else
        {
          doremi_play_sfx(SFX_INDEX_PORTCULLIS_OPENING);
          logic_all_vials_collected();
        }
      }
      else if (t1->type == THING_TYPE_BAT_VIAL)
      {
        G_vials_remaining -= 1;
        G_num_bat_vials += 1;

        /* play vial pickup sound */
        if (G_vials_remaining > 0)
          doremi_play_sfx(SFX_INDEX_PICKUP_VIAL);
        else
        {
          doremi_play_sfx(SFX_INDEX_PORTCULLIS_OPENING);
          logic_all_vials_collected();
        }
      }
      else if (t1->type == THING_TYPE_MANA_PRISM)
      {
        G_num_mana_prisms += 1;

        doremi_play_sfx(SFX_INDEX_PICKUP_VIAL);
      }
      else if (t1->type == THING_TYPE_SKELETON_KEY)
      {
        /* mark all padlocks as ready to open */
        for (n = 0; n < GRID_HEIGHT; n++)
        {
          for (m = 0; m < GRID_WIDTH; m++)
          {
            b = &G_collision_grid[(n * GRID_WIDTH) + m];

            if (b->object == GRID_OBJECT_PADLOCK)
            {
              b->state = STATE_READY_TO_CHANGE;
            }
          }
        }

        /* play key pickup sound */
        doremi_play_sfx(SFX_INDEX_PICKUP_KEY);
      }
      else if (t1->type == THING_TYPE_SPIKE_DROPPER)
      {
        spike_sfx_check = 1;

        /* mark all raised floor spikes as ready to change */
        for (n = 0; n < GRID_HEIGHT; n++)
        {
          for (m = 0; m < GRID_WIDTH; m++)
          {
            b = &G_collision_grid[(n * GRID_WIDTH) + m];

            if (b->object == GRID_OBJECT_FLOOR_SPIKES_UP)
            {
              b->state = STATE_READY_TO_CHANGE;
              spike_sfx_check = 0;
            }
          }
        }

        /* determine grid box that contains this pickup */
        box_x = (t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_WIDTH;
        box_y = (t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_HEIGHT;

        b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

        /* set spawner timer */
        if (b->object == GRID_OBJECT_SPIKE_DROPPER_SPAWNER)
        {
          b->state = STATE_SPAWNER_CHARGING_UP;
          b->timer_offset = 240 - G_timer_count;
        }

        if (spike_sfx_check == 1)
          doremi_play_sfx(SFX_INDEX_MENU_CANCEL);
      }
      else if (t1->type == THING_TYPE_SPIKE_RAISER)
      {
        spike_sfx_check = 1;

        /* mark all lowered floor spikes as ready to change */
        for (n = 0; n < GRID_HEIGHT; n++)
        {
          for (m = 0; m < GRID_WIDTH; m++)
          {
            b = &G_collision_grid[(n * GRID_WIDTH) + m];

            if (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN)
            {
              b->state = STATE_READY_TO_CHANGE;
              spike_sfx_check = 0;
            }
          }
        }

        /* determine grid box that contains this pickup */
        box_x = (t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_WIDTH;
        box_y = (t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_HEIGHT;

        b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

        /* set spawner timer */
        if (b->object == GRID_OBJECT_SPIKE_RAISER_SPAWNER)
        {
          b->state = STATE_SPAWNER_CHARGING_UP;
          b->timer_offset = 240 - G_timer_count;
        }

        if (spike_sfx_check == 1)
          doremi_play_sfx(SFX_INDEX_MENU_CANCEL);
      }
      else if (t1->type == THING_TYPE_ARROW_SPINNER_CCW)
      {
        /* mark all arrows as ready to spin */
        for (n = 0; n < GRID_HEIGHT; n++)
        {
          for (m = 0; m < GRID_WIDTH; m++)
          {
            b = &G_collision_grid[(n * GRID_WIDTH) + m];

            if ((b->object == GRID_OBJECT_ARROWS_RIGHT) || 
                (b->object == GRID_OBJECT_ARROWS_UP)    || 
                (b->object == GRID_OBJECT_ARROWS_LEFT)  || 
                (b->object == GRID_OBJECT_ARROWS_DOWN))
            {
              b->state = STATE_ARROWS_READY_TO_SPIN_CCW;
            }
          }
        }

        /* determine grid box that contains this pickup */
        box_x = (t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_WIDTH;
        box_y = (t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_HEIGHT;

        b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

        /* set spawner timer */
        if (b->object == GRID_OBJECT_ARROW_SPINNER_CCW_SPAWNER)
        {
          b->state = STATE_SPAWNER_CHARGING_UP;
          b->timer_offset = 240 - G_timer_count;
        }

        doremi_play_sfx(SFX_INDEX_TOGGLE_DOWN);
      }
      else if (t1->type == THING_TYPE_ARROW_SPINNER_CW)
      {
        /* mark all arrows as ready to spin */
        for (n = 0; n < GRID_HEIGHT; n++)
        {
          for (m = 0; m < GRID_WIDTH; m++)
          {
            b = &G_collision_grid[(n * GRID_WIDTH) + m];

            if ((b->object == GRID_OBJECT_ARROWS_RIGHT) || 
                (b->object == GRID_OBJECT_ARROWS_UP)    || 
                (b->object == GRID_OBJECT_ARROWS_LEFT)  || 
                (b->object == GRID_OBJECT_ARROWS_DOWN))
            {
              b->state = STATE_ARROWS_READY_TO_SPIN_CW;
            }
          }
        }

        /* determine grid box that contains this pickup */
        box_x = (t1->pos_x / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_WIDTH;
        box_y = (t1->pos_y / (THING_NUM_SUBPIXELS * GRID_BOX_SIZE)) % GRID_HEIGHT;

        b = &G_collision_grid[(box_y * GRID_WIDTH) + box_x];

        /* set spawner timer */
        if (b->object == GRID_OBJECT_ARROW_SPINNER_CW_SPAWNER)
        {
          b->state = STATE_SPAWNER_CHARGING_UP;
          b->timer_offset = 240 - G_timer_count;
        }

        doremi_play_sfx(SFX_INDEX_TOGGLE_UP);
      }

      /* despawn this pickup */
      world_despawn_thing(k);
    }
  }

  return 0;
}

/*******************************************************************************
** logic_check_if_player_said_bye_bye()
*******************************************************************************/
short int logic_check_if_player_said_bye_bye()
{
  thing* t;

  t = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  if (t->type == THING_TYPE_NONE)
    G_player_said_bye_bye = 1;

  return 0;
}

/*******************************************************************************
** logic_update_frame()
*******************************************************************************/
short int logic_update_frame()
{
  /* update denizen movements */
  logic_denizens_select_next_move();

  /* update grid */
  if (G_timer_count % 8 == 0)
  {
    logic_complete_grid_object_lifetimes();
    logic_grid_objects_activate();

    logic_start_audrey_respawns();
    logic_spawners_update();
  }

  /* update world */
  logic_move_things();
  logic_complete_spell_casts();
  logic_complete_thing_lifetimes();

  logic_process_spell_collisions();

  if (G_timer_count % 4 == 0)
    logic_spellcasters_update();

  logic_collect_pickups();

  /* check if we need to try again! */
  logic_check_if_player_said_bye_bye();

  return 0;
}

