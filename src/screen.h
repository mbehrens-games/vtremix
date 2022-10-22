/*******************************************************************************
** screen.h (game screens)
*******************************************************************************/

#ifndef SCREEN_H
#define SCREEN_H

/* screens */
enum
{
  GAME_SCREEN_TITLE = 0,
  GAME_SCREEN_SAVE_GAME_SELECT,
  GAME_SCREEN_CONFIRM_CLEAR_SAVE_GAME,
  GAME_SCREEN_WORLD_SELECT,
  GAME_SCREEN_ROOM_SELECT,
  GAME_SCREEN_STORY_SELECT,
  GAME_SCREEN_STORY_SCENE,
  GAME_SCREEN_OPTIONS,
  GAME_SCREEN_HELP,
  GAME_SCREEN_CREDITS,
  GAME_SCREEN_PLAYING,
  GAME_SCREEN_ROOM_INTRO_PANEL,
  GAME_SCREEN_ROOM_COMPLETE_PANEL,
  GAME_SCREEN_PAUSE_PANEL,
  GAME_SCREEN_RETRY_PANEL,
  GAME_SCREEN_TRY_AGAIN_PANEL,
  GAME_SCREEN_QUIT,
  GAME_NUM_SCREENS
};

/* alternates */
enum
{
  SCREEN_OPTIONS_ALTERNATE_STANDARD = 0,
  SCREEN_OPTIONS_ALTERNATE_VIDEO,
  SCREEN_OPTIONS_NUM_ALTERNATES
};

enum
{
  SCREEN_PLAYING_ALTERNATE_STANDARD = 0,
  SCREEN_PLAYING_ALTERNATE_LOAD_ROOM,
  SCREEN_PLAYING_ALTERNATE_RESET_ROOM,
  SCREEN_PLAYING_ALTERNATE_BACK_TO_GAME,
  SCREEN_PLAYING_ALTERNATE_NEXT_ROOM,
  SCREEN_PLAYING_NUM_ALTERNATES
};

enum
{
  SCREEN_ROOM_COMPLETE_PANEL_ALTERNATE_STANDARD = 0,
  SCREEN_ROOM_COMPLETE_PANEL_ALTERNATE_CLOSING,
  SCREEN_ROOM_COMPLETE_PANEL_NUM_ALTERNATES
};

/* choices */
enum
{
  SCREEN_TITLE_CHOICE_PLAY_GAME = 0,
  SCREEN_TITLE_CHOICE_OPTIONS,
  SCREEN_TITLE_CHOICE_HELP,
  SCREEN_TITLE_CHOICE_CREDITS,
  SCREEN_TITLE_CHOICE_QUIT,
  SCREEN_TITLE_NUM_CHOICES
};

enum
{
  SCREEN_SAVE_GAME_SELECT_CHOICE_WORLD_SELECT = 0,
  SCREEN_SAVE_GAME_SELECT_CHOICE_STORY_SELECT,
  SCREEN_SAVE_GAME_SELECT_CHOICE_CLEAR_SAVE_GAME,
  SCREEN_SAVE_GAME_SELECT_NUM_CHOICES
};

enum
{
  SCREEN_CONFIRM_CLEAR_SAVE_GAME_CHOICE_YES = 0,
  SCREEN_CONFIRM_CLEAR_SAVE_GAME_CHOICE_NO,
  SCREEN_CONFIRM_CLEAR_SAVE_GAME_NUM_CHOICES
};

enum
{
  SCREEN_STANDARD_OPTIONS_CHOICE_WINDOW_SIZE = 0,
  SCREEN_STANDARD_OPTIONS_CHOICE_FULLSCREEN,
  SCREEN_STANDARD_OPTIONS_CHOICE_VSYNC,
  SCREEN_STANDARD_OPTIONS_CHOICE_VOLUME,
  SCREEN_STANDARD_OPTIONS_CHOICE_GAMEPAD_NUMBER,
  SCREEN_STANDARD_OPTIONS_CHOICE_BUTTON_LAYOUT,
  SCREEN_STANDARD_OPTIONS_CHOICE_VIDEO_OPTIONS,
  SCREEN_STANDARD_OPTIONS_NUM_CHOICES
};

enum
{
  SCREEN_VIDEO_OPTIONS_CHOICE_UPSCALING_MODE = 0,
  SCREEN_VIDEO_OPTIONS_CHOICE_BRIGHTNESS,
  SCREEN_VIDEO_OPTIONS_CHOICE_CONTRAST,
  SCREEN_VIDEO_OPTIONS_CHOICE_TINT,
  SCREEN_VIDEO_OPTIONS_CHOICE_SATURATION,
  SCREEN_VIDEO_OPTIONS_CHOICE_GAMMA,
  SCREEN_VIDEO_OPTIONS_CHOICE_BLUR_AMOUNT,
  SCREEN_VIDEO_OPTIONS_CHOICE_PHOSPHOR_MASK,
  SCREEN_VIDEO_OPTIONS_NUM_CHOICES
};

enum
{
  SCREEN_PAUSE_PANEL_CHOICE_BACK_TO_GAME = 0,
  SCREEN_PAUSE_PANEL_CHOICE_RETRY_ROOM,
  SCREEN_PAUSE_PANEL_CHOICE_OPTIONS,
  SCREEN_PAUSE_PANEL_CHOICE_HELP,
  SCREEN_PAUSE_PANEL_CHOICE_QUIT_TO_ROOM_SELECT,
  SCREEN_PAUSE_PANEL_NUM_CHOICES
};

enum
{
  SCREEN_RETRY_PANEL_CHOICE_YES = 0,
  SCREEN_RETRY_PANEL_CHOICE_NO,
  SCREEN_RETRY_PANEL_NUM_CHOICES
};

enum
{
  SCREEN_TRY_AGAIN_PANEL_CHOICE_YES = 0,
  SCREEN_TRY_AGAIN_PANEL_CHOICE_NO,
  SCREEN_TRY_AGAIN_PANEL_NUM_CHOICES
};

/* unnamed alternates / pages / choices */
#define SCREEN_SAVE_GAME_SELECT_NUM_PAGES   3

#define SCREEN_WORLD_SELECT_NUM_PAGES       5
#define SCREEN_WORLD_SELECT_NUM_CHOICES     3

#define SCREEN_ROOM_SELECT_NUM_ALTERNATES   15
#define SCREEN_ROOM_SELECT_NUM_CHOICES      8

#define SCREEN_STORY_SELECT_NUM_CHOICES     6

#define SCREEN_STORY_SCENE_NUM_ALTERNATES   6

#define SCREEN_HELP_NUM_PAGES               4

#endif