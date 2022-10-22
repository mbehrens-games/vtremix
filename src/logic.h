/*******************************************************************************
** logic.h (grid and world update functions)
*******************************************************************************/

#ifndef LOGIC_H
#define LOGIC_H

/* function declarations */
short int logic_attempt_move(int index, int facing);  /* called in vampire.c */
short int logic_attempt_transform(int index);         /* called in vampire.c */
short int logic_attempt_spell_cast(int index);        /* called in vampire.c */

short int logic_update_frame();

#endif
