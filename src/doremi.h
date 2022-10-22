/*******************************************************************************
** doremi.h (sound and music)
*******************************************************************************/

#ifndef DOREMI_H
#define DOREMI_H

#ifdef VTREMIX_MAC_OSX
  #include <SDL2_mixer/SDL_mixer.h>
#else
  #include <SDL2/SDL_mixer.h>
#endif

enum
{
  SFX_INDEX_AUDREY_RESPAWN = 0,
  SFX_INDEX_BRIDGE_COLLAPSE,
  SFX_INDEX_CAST_EVIL_EYE,
  SFX_INDEX_CAST_FIRE_SPELL,
  SFX_INDEX_CAST_ICE_SPELL,
  SFX_INDEX_CAST_RING_SPELL,
  SFX_INDEX_CAST_TWIN_SPELL,
  SFX_INDEX_DENIZEN_SAYS_BYE_BYE,
  SFX_INDEX_FREEZING,
  SFX_INDEX_JIANGSHI_STOP,
  SFX_INDEX_KABOOM,
  SFX_INDEX_LEVEL_COMPLETE,
  SFX_INDEX_MENU_CANCEL,
  SFX_INDEX_MENU_SELECT,
  SFX_INDEX_MUMMY_COLLAPSE,
  SFX_INDEX_PAUSE,
  SFX_INDEX_PICKUP_KEY,
  SFX_INDEX_PICKUP_VIAL,
  SFX_INDEX_PLAYER_SAYS_BYE_BYE,
  SFX_INDEX_PORTCULLIS_OPENING,
  SFX_INDEX_SPIKE_BREAK,
  SFX_INDEX_SPIKE_LOWER,
  SFX_INDEX_SPIKE_RAISE,
  SFX_INDEX_THING_SAYS_BYE_BYE,
  SFX_INDEX_TOGGLE_DOWN,
  SFX_INDEX_TOGGLE_UP,
  SFX_INDEX_TRANSFORM_TO_BAT,
  SFX_INDEX_TRANSFORM_TO_VAMPIRE,
  SFX_INDEX_WATER_SPLASH,
  SFX_INDEX_WORLD_COMPLETE,
  SFX_NUM_INDICES
};

enum
{
  MUSIC_TRACK_MENU = 0,
  MUSIC_TRACK_STORY,
  MUSIC_TRACK_WORLD_01,
  MUSIC_TRACK_WORLD_02,
  MUSIC_TRACK_WORLD_03,
  MUSIC_TRACK_WORLD_04,
  MUSIC_TRACK_WORLD_05,
  MUSIC_TRACK_WORLD_06,
  MUSIC_TRACK_WORLD_07,
  MUSIC_TRACK_WORLD_08,
  MUSIC_TRACK_WORLD_09,
  MUSIC_TRACK_WORLD_10,
  MUSIC_TRACK_WORLD_11,
  MUSIC_TRACK_WORLD_12,
  MUSIC_TRACK_WORLD_13,
  MUSIC_TRACK_WORLD_14,
  MUSIC_TRACK_WORLD_15,
  MUSIC_TRACK_NONE
};

extern Mix_Chunk  G_sfx_all[SFX_NUM_INDICES];
extern Mix_Music  *G_music;

extern int        G_sound_volume;

/* function declarations */
short int doremi_init_all_sfx();
short int doremi_deinit_all_sfx();
short int doremi_load_all_sfx_from_file(char* filename);
short int doremi_play_sfx(int type);

short int doremi_init_music();
short int doremi_deinit_music();
short int doremi_play_music(int track);
short int doremi_pause_music();
short int doremi_unpause_music();

short int doremi_set_volume(int steps);
short int doremi_increase_volume();
short int doremi_decrease_volume();

#endif
