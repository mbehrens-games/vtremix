/*******************************************************************************
** thing.h (things)
*******************************************************************************/

#ifndef THING_H
#define THING_H

/* thing positions, velocities, and hitbox  */
/* sizes are stored in terms of subpixels   */
/* there are 16 subpixels per pixel         */
/* (in other words, these values are fixed  */
/* point with a 4 bit mantissa).            */
#define THING_NUM_SUBPIXELS       16
#define THING_NUM_SUBPIXELS_HALF  (THING_NUM_SUBPIXELS / 2)

enum
{
  THING_TYPE_NONE = 0,
  /* vampire */
  THING_TYPE_VAMPIRE,
  THING_TYPE_BAT,
  /* spells */
  THING_TYPE_EVIL_EYE_SPELL,
  THING_TYPE_FIRE_SPELL,
  THING_TYPE_ICE_SPELL,
  THING_TYPE_RING_SPELL,
  THING_TYPE_TWIN_SPELL,
  /* pickups */
  THING_TYPE_BLOOD_VIAL,
  THING_TYPE_BAT_VIAL,
  THING_TYPE_MANA_PRISM,
  THING_TYPE_SKELETON_KEY,
  THING_TYPE_SPIKE_DROPPER,
  THING_TYPE_SPIKE_RAISER,
  THING_TYPE_ARROW_SPINNER_CCW,
  THING_TYPE_ARROW_SPINNER_CW,
  /* denizens */
  THING_TYPE_PUMPKIN,
  THING_TYPE_AUDREY,
  THING_TYPE_GHOST_ASLEEP,
  THING_TYPE_GHOST_AWAKE,
  THING_TYPE_SNAKE_ASLEEP,
  THING_TYPE_SNAKE_AWAKE,
  THING_TYPE_SHADE,
  THING_TYPE_WITCH_ASLEEP,
  THING_TYPE_WITCH_AWAKE,
  THING_TYPE_SORCERESS,
  THING_TYPE_IMP_ASLEEP,
  THING_TYPE_IMP_AWAKE,
  THING_TYPE_LIL_BOT,
  THING_TYPE_FAERIE_HORIZONTAL,
  THING_TYPE_FAERIE_VERTICAL,
  THING_TYPE_DOUBLE_NORMAL_VAMPIRE,
  THING_TYPE_DOUBLE_NORMAL_BAT,
  THING_TYPE_DOUBLE_REVERSE_VAMPIRE,
  THING_TYPE_DOUBLE_REVERSE_BAT,
  THING_TYPE_WISP_RT,
  THING_TYPE_WISP_LT,
  THING_TYPE_KNIGHT_RT,
  THING_TYPE_KNIGHT_LT,
  THING_TYPE_MUMMY_RT,
  THING_TYPE_MUMMY_LT,
  THING_TYPE_MUMMY_RT_COLLAPSING,
  THING_TYPE_MUMMY_LT_COLLAPSING,
  THING_TYPE_JIANGSHI_RT,
  THING_TYPE_JIANGSHI_LT,
  THING_TYPE_JIANGSHI_RT_STOPPED,
  THING_TYPE_JIANGSHI_LT_STOPPED,
  /* ice cubes */
  THING_TYPE_ICE_CUBE,
  THING_TYPE_ICE_CUBE_THAWING,
  /* boxes, pots, flasks */
  THING_TYPE_BOX_WOOD,
  THING_TYPE_BOX_METAL,
  THING_TYPE_BOX_STONE,
  THING_TYPE_POT_WOOD,
  THING_TYPE_POT_METAL,
  THING_TYPE_POT_STONE,
  THING_TYPE_ACID_FLASK,
  THING_TYPE_STONE_FLASK,
  /* puffs */
  THING_TYPE_PUFF_GREY,
  THING_TYPE_PUFF_ORANGE,
  THING_TYPE_PUFF_CYAN,
  THING_TYPE_PUFF_PURPLE,
  THING_TYPE_PUFF_GREEN
};

#define THING_IS_THE_PLAYER(t)                                                 \
  ( (t->type == THING_TYPE_VAMPIRE) ||                                         \
    (t->type == THING_TYPE_BAT))

#define THING_IS_A_SPELL(t)                                                    \
  ( (t->type == THING_TYPE_EVIL_EYE_SPELL)  ||                                 \
    (t->type == THING_TYPE_FIRE_SPELL)      ||                                 \
    (t->type == THING_TYPE_ICE_SPELL)       ||                                 \
    (t->type == THING_TYPE_RING_SPELL)      ||                                 \
    (t->type == THING_TYPE_TWIN_SPELL))

#define THING_IS_A_PICKUP(t)                                                   \
  ( (t->type == THING_TYPE_BLOOD_VIAL)        ||                               \
    (t->type == THING_TYPE_BAT_VIAL)          ||                               \
    (t->type == THING_TYPE_MANA_PRISM)        ||                               \
    (t->type == THING_TYPE_SKELETON_KEY)      ||                               \
    (t->type == THING_TYPE_SPIKE_DROPPER)     ||                               \
    (t->type == THING_TYPE_SPIKE_RAISER)      ||                               \
    (t->type == THING_TYPE_ARROW_SPINNER_CCW) ||                               \
    (t->type == THING_TYPE_ARROW_SPINNER_CW))

#define THING_IS_A_DENIZEN(t)                                                  \
  ( (t->type == THING_TYPE_PUMPKIN)                 ||                         \
    (t->type == THING_TYPE_AUDREY)                  ||                         \
    (t->type == THING_TYPE_GHOST_ASLEEP)            ||                         \
    (t->type == THING_TYPE_GHOST_AWAKE)             ||                         \
    (t->type == THING_TYPE_SNAKE_ASLEEP)            ||                         \
    (t->type == THING_TYPE_SNAKE_AWAKE)             ||                         \
    (t->type == THING_TYPE_SHADE)                   ||                         \
    (t->type == THING_TYPE_WITCH_ASLEEP)            ||                         \
    (t->type == THING_TYPE_WITCH_AWAKE)             ||                         \
    (t->type == THING_TYPE_SORCERESS)               ||                         \
    (t->type == THING_TYPE_IMP_ASLEEP)              ||                         \
    (t->type == THING_TYPE_IMP_AWAKE)               ||                         \
    (t->type == THING_TYPE_LIL_BOT)                 ||                         \
    (t->type == THING_TYPE_FAERIE_HORIZONTAL)       ||                         \
    (t->type == THING_TYPE_FAERIE_VERTICAL)         ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)       ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_BAT)      ||                         \
    (t->type == THING_TYPE_WISP_RT)                 ||                         \
    (t->type == THING_TYPE_WISP_LT)                 ||                         \
    (t->type == THING_TYPE_KNIGHT_RT)               ||                         \
    (t->type == THING_TYPE_KNIGHT_LT)               ||                         \
    (t->type == THING_TYPE_MUMMY_RT)                ||                         \
    (t->type == THING_TYPE_MUMMY_LT)                ||                         \
    (t->type == THING_TYPE_MUMMY_RT_COLLAPSING)     ||                         \
    (t->type == THING_TYPE_MUMMY_LT_COLLAPSING)     ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT_STOPPED)     ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT_STOPPED))

#define THING_IS_A_FLYING_DENIZEN(t)                                           \
  ( (t->type == THING_TYPE_DOUBLE_NORMAL_BAT) ||                               \
    (t->type == THING_TYPE_DOUBLE_REVERSE_BAT))

#define THING_IS_A_WALKING_DENIZEN(t)                                          \
  ( (t->type == THING_TYPE_IMP_AWAKE)               ||                         \
    (t->type == THING_TYPE_LIL_BOT)                 ||                         \
    (t->type == THING_TYPE_FAERIE_HORIZONTAL)       ||                         \
    (t->type == THING_TYPE_FAERIE_VERTICAL)         ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  ||                         \
    (t->type == THING_TYPE_WISP_RT)                 ||                         \
    (t->type == THING_TYPE_WISP_LT)                 ||                         \
    (t->type == THING_TYPE_KNIGHT_RT)               ||                         \
    (t->type == THING_TYPE_KNIGHT_LT)               ||                         \
    (t->type == THING_TYPE_MUMMY_RT)                ||                         \
    (t->type == THING_TYPE_MUMMY_LT)                ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT))

#define THING_IS_A_MOVING_DENIZEN(t)                                           \
  ( (t->type == THING_TYPE_IMP_AWAKE)               ||                         \
    (t->type == THING_TYPE_LIL_BOT)                 ||                         \
    (t->type == THING_TYPE_FAERIE_HORIZONTAL)       ||                         \
    (t->type == THING_TYPE_FAERIE_VERTICAL)         ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)       ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_BAT)      ||                         \
    (t->type == THING_TYPE_WISP_RT)                 ||                         \
    (t->type == THING_TYPE_WISP_LT)                 ||                         \
    (t->type == THING_TYPE_KNIGHT_RT)               ||                         \
    (t->type == THING_TYPE_KNIGHT_LT)               ||                         \
    (t->type == THING_TYPE_MUMMY_RT)                ||                         \
    (t->type == THING_TYPE_MUMMY_LT)                ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT))

#define THING_IS_A_SPELLCASTING_DENIZEN(t)                                     \
  ( (t->type == THING_TYPE_PUMPKIN)     ||                                     \
    (t->type == THING_TYPE_GHOST_AWAKE) ||                                     \
    (t->type == THING_TYPE_SNAKE_AWAKE) ||                                     \
    (t->type == THING_TYPE_SHADE)       ||                                     \
    (t->type == THING_TYPE_WITCH_AWAKE) ||                                     \
    (t->type == THING_TYPE_SORCERESS))

#define THING_IS_A_PUSHABLE_OBJECT(t)                                          \
  ( (t->type == THING_TYPE_ICE_CUBE)          ||                               \
    (t->type == THING_TYPE_ICE_CUBE_THAWING)  ||                               \
    (t->type == THING_TYPE_BOX_WOOD)          ||                               \
    (t->type == THING_TYPE_BOX_METAL)         ||                               \
    (t->type == THING_TYPE_BOX_STONE)         ||                               \
    (t->type == THING_TYPE_POT_WOOD)          ||                               \
    (t->type == THING_TYPE_POT_METAL)         ||                               \
    (t->type == THING_TYPE_POT_STONE)         ||                               \
    (t->type == THING_TYPE_ACID_FLASK)        ||                               \
    (t->type == THING_TYPE_STONE_FLASK))

#define THING_IS_A_SPIKE_BREAKING_PUSHABLE_OBJECT(t)                           \
  ( (t->type == THING_TYPE_BOX_METAL)   ||                                     \
    (t->type == THING_TYPE_POT_METAL)   ||                                     \
    (t->type == THING_TYPE_ACID_FLASK)  ||                                     \
    (t->type == THING_TYPE_STONE_FLASK))

#define THING_IS_A_NON_SPIKE_BREAKING_PUSHABLE_OBJECT(t)                       \
  ( (t->type == THING_TYPE_ICE_CUBE)          ||                               \
    (t->type == THING_TYPE_ICE_CUBE_THAWING)  ||                               \
    (t->type == THING_TYPE_BOX_WOOD)          ||                               \
    (t->type == THING_TYPE_BOX_STONE)         ||                               \
    (t->type == THING_TYPE_POT_WOOD)          ||                               \
    (t->type == THING_TYPE_POT_STONE))

#define THING_IS_A_PUFF(t)                                                     \
  ( (t->type == THING_TYPE_PUFF_GREY)   ||                                     \
    (t->type == THING_TYPE_PUFF_ORANGE) ||                                     \
    (t->type == THING_TYPE_PUFF_CYAN)   ||                                     \
    (t->type == THING_TYPE_PUFF_PURPLE) ||                                     \
    (t->type == THING_TYPE_PUFF_GREEN))

#define THING_CAN_BE_FLOWN_OVER(t)                                             \
  ( (t->type == THING_TYPE_POT_WOOD)    ||                                     \
    (t->type == THING_TYPE_POT_METAL)   ||                                     \
    (t->type == THING_TYPE_POT_STONE)   ||                                     \
    (t->type == THING_TYPE_ACID_FLASK)  ||                                     \
    (t->type == THING_TYPE_STONE_FLASK))

#define THING_CAN_BE_BURNED(t)                                                 \
  ( (t->type == THING_TYPE_VAMPIRE)                 ||                         \
    (t->type == THING_TYPE_BAT)                     ||                         \
    (t->type == THING_TYPE_PUMPKIN)                 ||                         \
    (t->type == THING_TYPE_AUDREY)                  ||                         \
    (t->type == THING_TYPE_GHOST_ASLEEP)            ||                         \
    (t->type == THING_TYPE_GHOST_AWAKE)             ||                         \
    (t->type == THING_TYPE_SNAKE_ASLEEP)            ||                         \
    (t->type == THING_TYPE_SNAKE_AWAKE)             ||                         \
    (t->type == THING_TYPE_SHADE)                   ||                         \
    (t->type == THING_TYPE_WITCH_ASLEEP)            ||                         \
    (t->type == THING_TYPE_WITCH_AWAKE)             ||                         \
    (t->type == THING_TYPE_SORCERESS)               ||                         \
    (t->type == THING_TYPE_IMP_ASLEEP)              ||                         \
    (t->type == THING_TYPE_IMP_AWAKE)               ||                         \
    (t->type == THING_TYPE_LIL_BOT)                 ||                         \
    (t->type == THING_TYPE_FAERIE_HORIZONTAL)       ||                         \
    (t->type == THING_TYPE_FAERIE_VERTICAL)         ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)       ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_BAT)      ||                         \
    (t->type == THING_TYPE_WISP_RT)                 ||                         \
    (t->type == THING_TYPE_WISP_LT)                 ||                         \
    (t->type == THING_TYPE_MUMMY_RT)                ||                         \
    (t->type == THING_TYPE_MUMMY_LT)                ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT_STOPPED)     ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT_STOPPED)     ||                         \
    (t->type == THING_TYPE_BOX_WOOD)                ||                         \
    (t->type == THING_TYPE_POT_WOOD)                ||                         \
    (t->type == THING_TYPE_ACID_FLASK))

#define THING_CAN_BE_FROZEN(t)                                                 \
  ( (t->type == THING_TYPE_VAMPIRE)                 ||                         \
    (t->type == THING_TYPE_BAT)                     ||                         \
    (t->type == THING_TYPE_PUMPKIN)                 ||                         \
    (t->type == THING_TYPE_AUDREY)                  ||                         \
    (t->type == THING_TYPE_GHOST_ASLEEP)            ||                         \
    (t->type == THING_TYPE_GHOST_AWAKE)             ||                         \
    (t->type == THING_TYPE_SNAKE_ASLEEP)            ||                         \
    (t->type == THING_TYPE_SNAKE_AWAKE)             ||                         \
    (t->type == THING_TYPE_WITCH_ASLEEP)            ||                         \
    (t->type == THING_TYPE_WITCH_AWAKE)             ||                         \
    (t->type == THING_TYPE_SORCERESS)               ||                         \
    (t->type == THING_TYPE_IMP_ASLEEP)              ||                         \
    (t->type == THING_TYPE_IMP_AWAKE)               ||                         \
    (t->type == THING_TYPE_LIL_BOT)                 ||                         \
    (t->type == THING_TYPE_FAERIE_HORIZONTAL)       ||                         \
    (t->type == THING_TYPE_FAERIE_VERTICAL)         ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)       ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_BAT)      ||                         \
    (t->type == THING_TYPE_MUMMY_RT)                ||                         \
    (t->type == THING_TYPE_MUMMY_LT)                ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT_STOPPED)     ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT_STOPPED))

#define THING_CAN_BE_DAMAGED(t)                                                \
  ( (t->type == THING_TYPE_VAMPIRE)                 ||                         \
    (t->type == THING_TYPE_BAT)                     ||                         \
    (t->type == THING_TYPE_PUMPKIN)                 ||                         \
    (t->type == THING_TYPE_AUDREY)                  ||                         \
    (t->type == THING_TYPE_GHOST_ASLEEP)            ||                         \
    (t->type == THING_TYPE_GHOST_AWAKE)             ||                         \
    (t->type == THING_TYPE_SNAKE_ASLEEP)            ||                         \
    (t->type == THING_TYPE_SNAKE_AWAKE)             ||                         \
    (t->type == THING_TYPE_SHADE)                   ||                         \
    (t->type == THING_TYPE_WITCH_ASLEEP)            ||                         \
    (t->type == THING_TYPE_WITCH_AWAKE)             ||                         \
    (t->type == THING_TYPE_SORCERESS)               ||                         \
    (t->type == THING_TYPE_IMP_ASLEEP)              ||                         \
    (t->type == THING_TYPE_IMP_AWAKE)               ||                         \
    (t->type == THING_TYPE_LIL_BOT)                 ||                         \
    (t->type == THING_TYPE_FAERIE_HORIZONTAL)       ||                         \
    (t->type == THING_TYPE_FAERIE_VERTICAL)         ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   ||                         \
    (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)       ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  ||                         \
    (t->type == THING_TYPE_DOUBLE_REVERSE_BAT)      ||                         \
    (t->type == THING_TYPE_WISP_RT)                 ||                         \
    (t->type == THING_TYPE_WISP_LT)                 ||                         \
    (t->type == THING_TYPE_MUMMY_RT)                ||                         \
    (t->type == THING_TYPE_MUMMY_LT)                ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT)             ||                         \
    (t->type == THING_TYPE_JIANGSHI_RT_STOPPED)     ||                         \
    (t->type == THING_TYPE_JIANGSHI_LT_STOPPED)     ||                         \
    (t->type == THING_TYPE_BOX_WOOD)                ||                         \
    (t->type == THING_TYPE_POT_WOOD)                ||                         \
    (t->type == THING_TYPE_ACID_FLASK))

#define THING_IS_NOT_THE_PLAYER(t)                                             \
  (!THING_IS_THE_PLAYER(t))

#define THING_IS_NOT_A_SPELL(t)                                                \
  (!THING_IS_A_SPELL(t))

#define THING_IS_NOT_A_PICKUP(t)                                               \
  (!THING_IS_A_PICKUP(t))

#define THING_IS_NOT_A_DENIZEN(t)                                              \
  (!THING_IS_A_DENIZEN(t))

#define THING_IS_NOT_A_FLYING_DENIZEN(t)                                       \
  (!THING_IS_A_FLYING_DENIZEN(t))

#define THING_IS_NOT_A_WALKING_DENIZEN(t)                                      \
  (!THING_IS_A_WALKING_DENIZEN(t))

#define THING_IS_NOT_A_MOVING_DENIZEN(t)                                       \
  (!THING_IS_A_MOVING_DENIZEN(t))

#define THING_IS_NOT_A_SPELLCASTING_DENIZEN(t)                                 \
  (!THING_IS_A_SPELLCASTING_DENIZEN(t))

#define THING_IS_NOT_A_PUSHABLE_OBJECT(t)                                      \
  (!THING_IS_A_PUSHABLE_OBJECT(t))

#define THING_IS_NOT_A_SPIKE_BREAKING_PUSHABLE_OBJECT(t)                       \
  (!THING_IS_A_SPIKE_BREAKING_PUSHABLE_OBJECT(t))

#define THING_IS_NOT_A_NON_SPIKE_BREAKING_PUSHABLE_OBJECT(t)                   \
  (!THING_IS_A_NON_SPIKE_BREAKING_PUSHABLE_OBJECT(t))

#define THING_IS_NOT_A_PUFF(t)                                                 \
  (!THING_IS_A_PUFF(t))

#define THING_CANNOT_BE_FLOWN_OVER(t)                                          \
  (!THING_CAN_BE_FLOWN_OVER(t))

#define THING_CANNOT_BE_BURNED(t)                                              \
  (!THING_CAN_BE_BURNED(t))

#define THING_CANNOT_BE_FROZEN(t)                                              \
  (!THING_CAN_BE_FROZEN(t))

#define THING_CANNOT_BE_DAMAGED(t)                                             \
  (!THING_CAN_BE_DAMAGED(t))

enum
{
  THING_FACING_RIGHT = 0,
  THING_FACING_UP,
  THING_FACING_LEFT,
  THING_FACING_DOWN
};

enum
{
  THING_COLLISION_FLAG_NONE = 0,
  THING_COLLISION_FLAG_WE_HIT_SOMETHING,
  THING_COLLISION_FLAG_PICKUP_COLLECTED,
  THING_COLLISION_FLAG_HIT_WITH_EVIL_EYE_SPELL,
  THING_COLLISION_FLAG_HIT_WITH_FIRE_SPELL,
  THING_COLLISION_FLAG_HIT_WITH_ICE_SPELL,
  THING_COLLISION_FLAG_HIT_WITH_RING_SPELL,
  THING_COLLISION_FLAG_HIT_WITH_TWIN_SPELL,
  THING_COLLISION_FLAG_NEGATE_SPELLS
};

#define THING_DENIZEN_VEL_WALK    16

#define THING_SPELL_VEL_SLOW      16
#define THING_SPELL_VEL_FAST      32
#define THING_SPELL_VEL_ULTRA     48

#define THING_DENIZEN_MOVE_TIME   16
#define THING_DENIZEN_CAST_TIME   30
#define THING_DENIZEN_TURN_TIME   12

#define THING_DENIZEN_WAKEUP_TIME 24

typedef struct thing
{
  /* thing type, state, facing */
  int type;
  int state;
  int facing;

  /* original type, facing, freeze timer offset (used for turning into an ice cube) */
  int freeze_type;
  int freeze_facing;
  int freeze_timer_offset;

  /* thing collision flag */
  int flag;

  /* position (in subpixels) */
  int pos_x;
  int pos_y;

  /* velocity (in subpixels / frame) */
  int vel_x;
  int vel_y;

  /* hitbox half-width and half-height (in subpixels) */
  int hx;
  int hy;

  /* timer offset */
  unsigned int timer_offset;
} thing;

#endif
