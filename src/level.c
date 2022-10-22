/*******************************************************************************
** level.c (level)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "level.h"

/* global variables */
unsigned char G_room_tilemaps[LEVEL_NUM_ROOMS][LEVEL_ROOM_NUM_TILES];

char G_episode_names[LEVEL_NUM_EPISODES][LEVEL_EPISODE_NAME_SIZE];
char G_world_names[LEVEL_NUM_WORLDS][LEVEL_WORLD_NAME_SIZE];
char G_room_names[LEVEL_NUM_ROOMS][LEVEL_ROOM_NAME_SIZE];

/*******************************************************************************
** level_init_arrays()
*******************************************************************************/
short int level_init_arrays()
{
  int m;
  int n;

  /* initialize room tilemaps */
  for (m = 0; m < LEVEL_NUM_ROOMS; m++)
  {
    for (n = 0; n < LEVEL_ROOM_NUM_TILES; n++)
    {
      G_room_tilemaps[m][n] = LEVEL_ROOM_TILE_BLANK;
    }
  }

  /* initialize name arrays */
  for (m = 0; m < LEVEL_NUM_EPISODES; m++)
  {
    for (n = 0; n < LEVEL_EPISODE_NAME_SIZE; n++)
    {
      G_episode_names[m][n] = '\0';
    }
  }

  for (m = 0; m < LEVEL_NUM_WORLDS; m++)
  {
    for (n = 0; n < LEVEL_WORLD_NAME_SIZE; n++)
    {
      G_world_names[m][n] = '\0';
    }
  }

  for (m = 0; m < LEVEL_NUM_ROOMS; m++)
  {
    for (n = 0; n < LEVEL_ROOM_NAME_SIZE; n++)
    {
      G_room_names[m][n] = '\0';
    }
  }

  return 0;
}

/*******************************************************************************
** level_load_all_from_file()
*******************************************************************************/
short int level_load_all_from_file(char* filename)
{
  FILE* fp;

  int   i;
  int   j;

  char  signature[4];
  char  type[4];

  if (filename == NULL)
    return 1;

  /* initialize data */
  level_init_arrays();

  /* open file */
  fp = fopen(filename, "rb");

  /* if file did not open, return error */
  if (fp == NULL)
    return 1;

  /* read signature */
  if (fread(signature, 1, 4, fp) < 4)
  {
    fclose(fp);
    return 1;
  }

  if ((signature[0] != 'V') || 
      (signature[1] != 'T') || 
      (signature[2] != 'R') || 
      (signature[3] != 'X'))
  {
    fclose(fp);
    return 1;
  }

  /* read type */
  if (fread(type, 1, 4, fp) < 4)
  {
    fclose(fp);
    return 1;
  }

  if ((type[0] != 'L') || 
      (type[1] != 'V') || 
      (type[2] != 'L') || 
      (type[3] != 'S'))
  {
    fclose(fp);
    return 1;
  }

  /* read rooms */
  for (i = 0; i < LEVEL_NUM_ROOMS; i++)
  {
    /* read this room from the file */
    if (fread(&G_room_tilemaps[i][0], 1, LEVEL_ROOM_NUM_TILES, fp) == 0)
    {
      fclose(fp);
      return 1;
    }

    /* convert any invalid room tile values to "blank" */
    for (j = 0; j < LEVEL_ROOM_HEIGHT * LEVEL_ROOM_WIDTH; j++)
    {
      if ((G_room_tilemaps[i][j] < LEVEL_ROOM_TILE_BLANK) ||
          (G_room_tilemaps[i][j] > LEVEL_ROOM_TILE_JIANGSHI_LT_DOWN))
      {
        G_room_tilemaps[i][j] = LEVEL_ROOM_TILE_BLANK;
      }
    }
  }

  /* read episode names */
  for (i = 0; i < LEVEL_NUM_EPISODES; i++)
  {
    /* read this name from the file */
    if (fread(&G_episode_names[i][0], 1, LEVEL_EPISODE_NAME_SIZE, fp) == 0)
    {
      fclose(fp);
      return 1;
    }
  }

  /* read world names */
  for (i = 0; i < LEVEL_NUM_WORLDS; i++)
  {
    /* read this name from the file */
    if (fread(&G_world_names[i][0], 1, LEVEL_WORLD_NAME_SIZE, fp) == 0)
    {
      fclose(fp);
      return 1;
    }
  }

  /* read room names */
  for (i = 0; i < LEVEL_NUM_ROOMS; i++)
  {
    /* read this name from the file */
    if (fread(&G_room_names[i][0], 1, LEVEL_ROOM_NAME_SIZE, fp) == 0)
    {
      fclose(fp);
      return 1;
    }
  }

  /* decode text data, and convert any      */
  /* invalid characters to null terminators */

  /* the ascii range used is 32 to 127 */
  /* the coded values are from 1 to 96 */
  for (i = 0; i < LEVEL_NUM_EPISODES; i++)
  {
    for (j = 0; j < LEVEL_EPISODE_NAME_SIZE; j++)
    {
      if ((G_episode_names[i][j] >= 1) && (G_episode_names[i][j] <= 96))
        G_episode_names[i][j] = G_episode_names[i][j] + 31;
      else
        G_episode_names[i][j] = '\0';
    }
  }

  for (i = 0; i < LEVEL_NUM_WORLDS; i++)
  {
    for (j = 0; j < LEVEL_WORLD_NAME_SIZE; j++)
    {
      if ((G_world_names[i][j] >= 1) && (G_world_names[i][j] <= 96))
        G_world_names[i][j] = G_world_names[i][j] + 31;
      else
        G_world_names[i][j] = '\0';
    }
  }

  for (i = 0; i < LEVEL_NUM_ROOMS; i++)
  {
    for (j = 0; j < LEVEL_ROOM_NAME_SIZE; j++)
    {
      if ((G_room_names[i][j] >= 1) && (G_room_names[i][j] <= 96))
        G_room_names[i][j] = G_room_names[i][j] + 31;
      else
        G_room_names[i][j] = '\0';
    }
  }

  /* close file */
  fclose(fp);

  return 0;
}

