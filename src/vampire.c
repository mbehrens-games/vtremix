/*******************************************************************************
** vampire.c (vampire update functions)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "doremi.h"
#include "global.h"
#include "grid.h"
#include "logic.h"
#include "state.h"
#include "thing.h"
#include "world.h"

/*******************************************************************************
** vampire_player_input_d_pad_left_on()
*******************************************************************************/
short int vampire_player_input_d_pad_left_on()
{
  int k;

  thing* t1;
  thing* t2;

  t1 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* make sure we are either a vampire or bat (i.e., not an ice cube) */
  if (THING_IS_NOT_THE_PLAYER(t1))
    return 0;

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* try to move the vampire */
  logic_attempt_move(WORLD_VAMPIRE_INDEX, THING_FACING_LEFT);

  /* update the doubles */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t2 = &G_world_all_things[k];

    if (t2->type == THING_TYPE_NONE)
      continue;

    if ((t2->state == STATE_STARTED_MOVING)           || 
        (t2->state == STATE_MOVING_ONE_SPACE)         || 
        (t2->state == STATE_CASTING)                  || 
        (t2->state == STATE_AWAITING_SNAP_TO_GRID)    || 
        (t2->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
        (t2->state == STATE_AWAITING_SNAP_AND_THAW))
    {
      continue;
    }

    /* if the player moved successfully, try to move the    */
    /* double as well. otherwise, just change their facing. */
    if ((t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
        (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_LEFT);
      else
        t2->facing = THING_FACING_LEFT;
    }
    else if ( (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
              (t2->type == THING_TYPE_DOUBLE_REVERSE_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_RIGHT);
      else
        t2->facing = THING_FACING_RIGHT;
    }
  }

#if 0
  /* if the move was successful, also try to move the doubles */
  if (t1->state == STATE_STARTED_MOVING)
  {
    for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
    {
      t2 = &G_world_all_things[k];

      if (t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)
        logic_attempt_move(k, THING_FACING_LEFT);
      else if (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)
        logic_attempt_move(k, THING_FACING_RIGHT);
    }
  }
#endif

  return 0;
}

/*******************************************************************************
** vampire_player_input_d_pad_right_on()
*******************************************************************************/
short int vampire_player_input_d_pad_right_on()
{
  int k;

  thing* t1;
  thing* t2;

  t1 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* make sure we are either a vampire or bat (i.e., not an ice cube) */
  if (THING_IS_NOT_THE_PLAYER(t1))
    return 0;

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* try to move the vampire */
  logic_attempt_move(WORLD_VAMPIRE_INDEX, THING_FACING_RIGHT);

  /* update the doubles */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t2 = &G_world_all_things[k];

    if (t2->type == THING_TYPE_NONE)
      continue;

    if ((t2->state == STATE_STARTED_MOVING)           || 
        (t2->state == STATE_MOVING_ONE_SPACE)         || 
        (t2->state == STATE_CASTING)                  || 
        (t2->state == STATE_AWAITING_SNAP_TO_GRID)    || 
        (t2->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
        (t2->state == STATE_AWAITING_SNAP_AND_THAW))
    {
      continue;
    }

    /* if the player moved successfully, try to move the    */
    /* double as well. otherwise, just change their facing. */
    if ((t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
        (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_RIGHT);
      else
        t2->facing = THING_FACING_RIGHT;
    }
    else if ( (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
              (t2->type == THING_TYPE_DOUBLE_REVERSE_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_LEFT);
      else
        t2->facing = THING_FACING_LEFT;
    }
  }

  return 0;
}

/*******************************************************************************
** vampire_player_input_d_pad_up_on()
*******************************************************************************/
short int vampire_player_input_d_pad_up_on()
{
  int k;

  thing* t1;
  thing* t2;

  t1 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* make sure we are either a vampire or bat (i.e., not an ice cube) */
  if (THING_IS_NOT_THE_PLAYER(t1))
    return 0;

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* try to move the vampire */
  logic_attempt_move(WORLD_VAMPIRE_INDEX, THING_FACING_UP);

  /* update the doubles */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t2 = &G_world_all_things[k];

    if (t2->type == THING_TYPE_NONE)
      continue;

    if ((t2->state == STATE_STARTED_MOVING)           || 
        (t2->state == STATE_MOVING_ONE_SPACE)         || 
        (t2->state == STATE_CASTING)                  || 
        (t2->state == STATE_AWAITING_SNAP_TO_GRID)    || 
        (t2->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
        (t2->state == STATE_AWAITING_SNAP_AND_THAW))
    {
      continue;
    }

    /* if the player moved successfully, try to move the    */
    /* double as well. otherwise, just change their facing. */
    if ((t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
        (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_UP);
      else
        t2->facing = THING_FACING_UP;
    }
    else if ( (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
              (t2->type == THING_TYPE_DOUBLE_REVERSE_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_DOWN);
      else
        t2->facing = THING_FACING_DOWN;
    }
  }

  return 0;
}

/*******************************************************************************
** vampire_player_input_d_pad_down_on()
*******************************************************************************/
short int vampire_player_input_d_pad_down_on()
{
  int k;

  thing* t1;
  thing* t2;

  t1 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* make sure we are either a vampire or bat (i.e., not an ice cube) */
  if (THING_IS_NOT_THE_PLAYER(t1))
    return 0;

  /* make sure this thing is not already moving, casting, etc */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_CASTING)                  || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    return 0;
  }

  /* try to move the vampire */
  logic_attempt_move(WORLD_VAMPIRE_INDEX, THING_FACING_DOWN);

  /* update the doubles */
  for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
  {
    t2 = &G_world_all_things[k];

    if (t2->type == THING_TYPE_NONE)
      continue;

    if ((t2->state == STATE_STARTED_MOVING)           || 
        (t2->state == STATE_MOVING_ONE_SPACE)         || 
        (t2->state == STATE_CASTING)                  || 
        (t2->state == STATE_AWAITING_SNAP_TO_GRID)    || 
        (t2->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
        (t2->state == STATE_AWAITING_SNAP_AND_THAW))
    {
      continue;
    }

    /* if the player moved successfully, try to move the    */
    /* double as well. otherwise, just change their facing. */
    if ((t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
        (t2->type == THING_TYPE_DOUBLE_NORMAL_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_DOWN);
      else
        t2->facing = THING_FACING_DOWN;
    }
    else if ( (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE) || 
              (t2->type == THING_TYPE_DOUBLE_REVERSE_BAT))
    {
      if (t1->state == STATE_STARTED_MOVING)
        logic_attempt_move(k, THING_FACING_UP);
      else
        t2->facing = THING_FACING_UP;
    }
  }

  return 0;
}

/*******************************************************************************
** vampire_player_input_button_b_pressed()
*******************************************************************************/
short int vampire_player_input_button_b_pressed()
{
  int k;

  thing* t1;
  thing* t2;

  t1 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* make sure we are either a vampire or bat (i.e., not an ice cube) */
  if (THING_IS_NOT_THE_PLAYER(t1))
    return 0;

  /* check if the vampire is currently moving */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    G_flag_button_b_queued = 1;

    return 0;
  }

  /* check if the vampire is currently casting */
  if (t1->state == STATE_CASTING)
    return 0;

  /* clear input queue flags */
  G_flag_button_b_queued = 0;
  G_flag_button_a_queued = 0;

  /* try to transform! */

  /* from vampire to bat */
  if (t1->type == THING_TYPE_VAMPIRE)
  {
    /* make sure we have a bat vial */
    if (G_num_bat_vials < 1)
      return 0;

    logic_attempt_transform(WORLD_VAMPIRE_INDEX);

    /* if the transform was successful, also try to transform the doubles */
    if (t1->type == THING_TYPE_BAT)
    {
      G_num_bat_vials -= 1;
      doremi_play_sfx(SFX_INDEX_TRANSFORM_TO_BAT);

      for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
      {
        t2 = &G_world_all_things[k];

        if ((t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
            (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE))
        {
          logic_attempt_transform(k);
        }
      }
    }
  }
  /* from bat to vampire */
  else if (t1->type == THING_TYPE_BAT)
  {
    logic_attempt_transform(WORLD_VAMPIRE_INDEX);

    /* if the transform was successful, also try to transform the doubles */
    if (t1->type == THING_TYPE_VAMPIRE)
    {
      doremi_play_sfx(SFX_INDEX_TRANSFORM_TO_VAMPIRE);

      for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
      {
        t2 = &G_world_all_things[k];

        if ((t2->type == THING_TYPE_DOUBLE_NORMAL_BAT) || 
            (t2->type == THING_TYPE_DOUBLE_REVERSE_BAT))
        {
          logic_attempt_transform(k);
        }
      }
    }
  }

  return 0;
}

/*******************************************************************************
** vampire_player_input_button_a_pressed()
*******************************************************************************/
short int vampire_player_input_button_a_pressed()
{
  int k;

  thing* t1;
  thing* t2;

  t1 = &G_world_all_things[WORLD_VAMPIRE_INDEX];

  /* make sure we are either a vampire or bat (i.e., not an ice cube) */
  if (THING_IS_NOT_THE_PLAYER(t1))
    return 0;

  /* check if the vampire is currently moving */
  if ((t1->state == STATE_STARTED_MOVING)           || 
      (t1->state == STATE_MOVING_ONE_SPACE)         || 
      (t1->state == STATE_AWAITING_SNAP_TO_GRID)    || 
      (t1->state == STATE_AWAITING_SNAP_AND_FREEZE) || 
      (t1->state == STATE_AWAITING_SNAP_AND_THAW))
  {
    G_flag_button_a_queued = 1;

    return 0;
  }

  /* check if the vampire is currently casting */
  if (t1->state == STATE_CASTING)
    return 0;

  /* clear input queue flags */
  G_flag_button_b_queued = 0;
  G_flag_button_a_queued = 0;

  /* if we are a vampire, try to cast the ice spell! */
  if (t1->type == THING_TYPE_VAMPIRE)
  {
    /* make sure we have a mana prism */
    if (G_num_mana_prisms < 1)
      return 0;

    G_num_mana_prisms -= 1;

    logic_attempt_spell_cast(WORLD_VAMPIRE_INDEX);

    /* also try to have the doubles cast the ice spell */
    for (k = WORLD_DENIZENS_START_INDEX; k < WORLD_DENIZENS_END_INDEX; k++)
    {
      t2 = &G_world_all_things[k];

      if ((t2->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
          (t2->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE))
      {
        logic_attempt_spell_cast(k);
      }
    }
  }

  return 0;
}

