/*******************************************************************************
** global.c (global variables)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "screen.h"

int           G_game_screen;
int           G_last_screen;

int           G_screen_alternate;
int           G_screen_page;
int           G_screen_choice;

int           G_flag_world_complete;
int           G_flag_button_b_queued;
int           G_flag_button_a_queued;

int           G_flag_window_minimized;
int           G_flag_window_fullscreen;
int           G_flag_window_vsync;
int           G_flag_quit_game;

unsigned int  G_timer_count;
unsigned int  G_saved_timer_count;

int           G_current_room;
int           G_current_world;

int           G_vials_remaining;
int           G_portcullis_reached;
int           G_player_said_bye_bye;

int           G_num_bat_vials;
int           G_num_mana_prisms;

int           G_num_audrey_respawns;

/*******************************************************************************
** globals_init_variables()
*******************************************************************************/
short int globals_init_variables()
{
  G_game_screen = GAME_SCREEN_TITLE;
  G_last_screen = GAME_SCREEN_TITLE;

  G_screen_alternate = 0;
  G_screen_page = 0;
  G_screen_choice = 0;

  G_flag_world_complete = 0;
  G_flag_button_b_queued = 0;
  G_flag_button_a_queued = 0;

  G_flag_window_minimized = 0;
  G_flag_window_fullscreen = 0;
  G_flag_window_vsync = 0;
  G_flag_quit_game = 0;

  G_timer_count = 0;
  G_saved_timer_count = 0;

  G_current_room = 0;
  G_current_world = 0;

  G_vials_remaining = 0;
  G_portcullis_reached = 0;
  G_player_said_bye_bye = 0;

  G_num_bat_vials = 0;
  G_num_mana_prisms = 0;

  G_num_audrey_respawns = 0;

  return 0;
}

