/*******************************************************************************
** state.h (states for grid objects and things)
*******************************************************************************/

#ifndef STATE_H
#define STATE_H

enum
{
  STATE_NONE = 0,
  STATE_READY_TO_CHANGE,
  STATE_CHANGING,
  STATE_SPAWNER_CHARGING_UP,
  STATE_SPAWNER_FULLY_CHARGED,
  STATE_ARROWS_READY_TO_SPIN_CCW,
  STATE_ARROWS_READY_TO_SPIN_CW,
  STATE_STARTED_MOVING,
  STATE_MOVING_ONE_SPACE,
  STATE_STOPPED_MOVING,
  STATE_TURNING,
  STATE_BLOCKED_IN,
  STATE_CASTING,
  STATE_WAKEUP,
  STATE_AWAITING_SNAP_TO_GRID,
  STATE_AWAITING_SNAP_AND_FREEZE,
  STATE_AWAITING_SNAP_AND_THAW
};

#endif
