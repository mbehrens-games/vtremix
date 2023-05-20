/*******************************************************************************
** level.h (level)
*******************************************************************************/

#ifndef LEVEL_H
#define LEVEL_H

/* sky tiles */
#define LEVEL_SKY_WIDTH     10
#define LEVEL_SKY_HEIGHT    7
#define LEVEL_SKY_NUM_TILES (LEVEL_SKY_WIDTH * LEVEL_SKY_HEIGHT)

#define LEVEL_SKY_TILE_SIZE       32
#define LEVEL_SKY_TILE_SIZE_HALF  (LEVEL_SKY_TILE_SIZE / 2)

/* room tiles */
#define LEVEL_ROOM_TILE_BLANK               0x00
#define LEVEL_ROOM_TILE_PLAYER_START        0x01
#define LEVEL_ROOM_TILE_BLOCK               0x02
#define LEVEL_ROOM_TILE_PILLAR              0x03
#define LEVEL_ROOM_TILE_WATER               0x04
#define LEVEL_ROOM_TILE_BRIDGE_VERT         0x05
#define LEVEL_ROOM_TILE_BRIDGE_HORI         0x06
#define LEVEL_ROOM_TILE_PADLOCK             0x07

#define LEVEL_ROOM_TILE_BLOOD_VIAL          0x08
#define LEVEL_ROOM_TILE_BAT_VIAL            0x09
#define LEVEL_ROOM_TILE_MANA_PRISM          0x0A
#define LEVEL_ROOM_TILE_SKELETON_KEY        0x0B
#define LEVEL_ROOM_TILE_SPIKE_DROPPER       0x0C
#define LEVEL_ROOM_TILE_SPIKE_RAISER        0x0D
#define LEVEL_ROOM_TILE_ARROW_SPINNER_CCW   0x0E
#define LEVEL_ROOM_TILE_ARROW_SPINNER_CW    0x0F

#define LEVEL_ROOM_TILE_BOX_WOOD            0x10
#define LEVEL_ROOM_TILE_BOX_METAL           0x11
#define LEVEL_ROOM_TILE_BOX_STONE           0x12
#define LEVEL_ROOM_TILE_POT_WOOD            0x13
#define LEVEL_ROOM_TILE_POT_METAL           0x14
#define LEVEL_ROOM_TILE_POT_STONE           0x15
#define LEVEL_ROOM_TILE_ACID_FLASK          0x16
#define LEVEL_ROOM_TILE_STONE_FLASK         0x17

#define LEVEL_ROOM_TILE_ARROWS_RIGHT        0x18
#define LEVEL_ROOM_TILE_ARROWS_UP           0x19
#define LEVEL_ROOM_TILE_ARROWS_LEFT         0x1A
#define LEVEL_ROOM_TILE_ARROWS_DOWN         0x1B
#define LEVEL_ROOM_TILE_FLOOR_SPIKES_DOWN   0x1C
#define LEVEL_ROOM_TILE_FLOOR_SPIKES_UP     0x1D
#define LEVEL_ROOM_TILE_PUMPKIN             0x1E
#define LEVEL_ROOM_TILE_AUDREY              0x1F

#define LEVEL_ROOM_TILE_GHOST_RIGHT         0x20
#define LEVEL_ROOM_TILE_GHOST_UP            0x21
#define LEVEL_ROOM_TILE_GHOST_LEFT          0x22
#define LEVEL_ROOM_TILE_GHOST_DOWN          0x23
#define LEVEL_ROOM_TILE_SNAKE_RIGHT         0x24
#define LEVEL_ROOM_TILE_SNAKE_UP            0x25
#define LEVEL_ROOM_TILE_SNAKE_LEFT          0x26
#define LEVEL_ROOM_TILE_SNAKE_DOWN          0x27

#define LEVEL_ROOM_TILE_SHADE_RIGHT         0x28
#define LEVEL_ROOM_TILE_SHADE_UP            0x29
#define LEVEL_ROOM_TILE_SHADE_LEFT          0x2A
#define LEVEL_ROOM_TILE_SHADE_DOWN          0x2B
#define LEVEL_ROOM_TILE_WITCH_RIGHT         0x2C
#define LEVEL_ROOM_TILE_WITCH_UP            0x2D
#define LEVEL_ROOM_TILE_WITCH_LEFT          0x2E
#define LEVEL_ROOM_TILE_WITCH_DOWN          0x2F

#define LEVEL_ROOM_TILE_SORCERESS_RIGHT     0x30
#define LEVEL_ROOM_TILE_SORCERESS_UP        0x31
#define LEVEL_ROOM_TILE_SORCERESS_LEFT      0x32
#define LEVEL_ROOM_TILE_SORCERESS_DOWN      0x33
#define LEVEL_ROOM_TILE_IMP_RIGHT           0x34
#define LEVEL_ROOM_TILE_IMP_UP              0x35
#define LEVEL_ROOM_TILE_IMP_LEFT            0x36
#define LEVEL_ROOM_TILE_IMP_DOWN            0x37

#define LEVEL_ROOM_TILE_LIL_BOT_RIGHT       0x38
#define LEVEL_ROOM_TILE_LIL_BOT_UP          0x39
#define LEVEL_ROOM_TILE_LIL_BOT_LEFT        0x3A
#define LEVEL_ROOM_TILE_LIL_BOT_DOWN        0x3B
#define LEVEL_ROOM_TILE_FAERIE_HORIZONTAL   0x3C
#define LEVEL_ROOM_TILE_FAERIE_VERTICAL     0x3D
#define LEVEL_ROOM_TILE_DOUBLE_NORMAL       0x3E
#define LEVEL_ROOM_TILE_DOUBLE_REVERSE      0x3F

#define LEVEL_ROOM_TILE_WISP_RT_RIGHT       0x40
#define LEVEL_ROOM_TILE_WISP_RT_UP          0x41
#define LEVEL_ROOM_TILE_WISP_RT_LEFT        0x42
#define LEVEL_ROOM_TILE_WISP_RT_DOWN        0x43
#define LEVEL_ROOM_TILE_WISP_LT_RIGHT       0x44
#define LEVEL_ROOM_TILE_WISP_LT_UP          0x45
#define LEVEL_ROOM_TILE_WISP_LT_LEFT        0x46
#define LEVEL_ROOM_TILE_WISP_LT_DOWN        0x47

#define LEVEL_ROOM_TILE_KNIGHT_RT_RIGHT     0x48
#define LEVEL_ROOM_TILE_KNIGHT_RT_UP        0x49
#define LEVEL_ROOM_TILE_KNIGHT_RT_LEFT      0x4A
#define LEVEL_ROOM_TILE_KNIGHT_RT_DOWN      0x4B
#define LEVEL_ROOM_TILE_KNIGHT_LT_RIGHT     0x4C
#define LEVEL_ROOM_TILE_KNIGHT_LT_UP        0x4D
#define LEVEL_ROOM_TILE_KNIGHT_LT_LEFT      0x4E
#define LEVEL_ROOM_TILE_KNIGHT_LT_DOWN      0x4F

#define LEVEL_ROOM_TILE_MUMMY_RT_RIGHT      0x50
#define LEVEL_ROOM_TILE_MUMMY_RT_UP         0x51
#define LEVEL_ROOM_TILE_MUMMY_RT_LEFT       0x52
#define LEVEL_ROOM_TILE_MUMMY_RT_DOWN       0x53
#define LEVEL_ROOM_TILE_MUMMY_LT_RIGHT      0x54
#define LEVEL_ROOM_TILE_MUMMY_LT_UP         0x55
#define LEVEL_ROOM_TILE_MUMMY_LT_LEFT       0x56
#define LEVEL_ROOM_TILE_MUMMY_LT_DOWN       0x57

#define LEVEL_ROOM_TILE_JIANGSHI_RT_RIGHT   0x58
#define LEVEL_ROOM_TILE_JIANGSHI_RT_UP      0x59
#define LEVEL_ROOM_TILE_JIANGSHI_RT_LEFT    0x5A
#define LEVEL_ROOM_TILE_JIANGSHI_RT_DOWN    0x5B
#define LEVEL_ROOM_TILE_JIANGSHI_LT_RIGHT   0x5C
#define LEVEL_ROOM_TILE_JIANGSHI_LT_UP      0x5D
#define LEVEL_ROOM_TILE_JIANGSHI_LT_LEFT    0x5E
#define LEVEL_ROOM_TILE_JIANGSHI_LT_DOWN    0x5F

/* with 16x16 tiles, each screen consists of 11x11 tiles    */
/* note that one extra space is used for the door position  */
#define LEVEL_ROOM_WIDTH      11
#define LEVEL_ROOM_HEIGHT     11
#define LEVEL_ROOM_NUM_TILES  ((LEVEL_ROOM_WIDTH * LEVEL_ROOM_HEIGHT) + 1)

#define LEVEL_ROOM_TILE_SIZE      16
#define LEVEL_ROOM_TILE_SIZE_HALF (LEVEL_ROOM_TILE_SIZE / 2)

#define LEVEL_NUM_ROOMS 120

#define LEVEL_NUM_EPISODES        5
#define LEVEL_WORLDS_PER_EPISODE  3

#define LEVEL_NUM_WORLDS          15
#define LEVEL_ROOMS_PER_WORLD     8

#define LEVEL_EPISODE_NAME_SIZE   16
#define LEVEL_WORLD_NAME_SIZE     16
#define LEVEL_ROOM_NAME_SIZE      24

/* global variables */
extern unsigned char G_room_tilemaps[LEVEL_NUM_ROOMS][LEVEL_ROOM_NUM_TILES];

extern char G_episode_names[LEVEL_NUM_EPISODES][LEVEL_EPISODE_NAME_SIZE];
extern char G_world_names[LEVEL_NUM_WORLDS][LEVEL_WORLD_NAME_SIZE];
extern char G_room_names[LEVEL_NUM_ROOMS][LEVEL_ROOM_NAME_SIZE];

/* function declarations */
short int level_init_arrays();

short int level_load_all_from_file(char* filename);

#endif
