/*******************************************************************************
** global.h (global variables)
*******************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

extern int            G_game_screen;
extern int            G_last_screen;

extern int            G_screen_alternate;
extern int            G_screen_page;
extern int            G_screen_choice;

extern int            G_flag_world_complete;
extern int            G_flag_button_b_queued;
extern int            G_flag_button_a_queued;

extern int            G_flag_window_minimized;
extern int            G_flag_window_fullscreen;
extern int            G_flag_window_vsync;
extern int            G_flag_quit_game;

extern unsigned int   G_timer_count;
extern unsigned int   G_saved_timer_count;

extern int            G_current_room;
extern int            G_current_world;

extern int            G_vials_remaining;
extern int            G_portcullis_reached;
extern int            G_player_said_bye_bye;

extern int            G_num_bat_vials;
extern int            G_num_mana_prisms;

extern int            G_num_audrey_respawns;

/* function declarations */
short int globals_init_variables();

#endif
