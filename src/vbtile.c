/*******************************************************************************
** vbtile.c (vbo population functions - tiles)
*******************************************************************************/

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "graphics.h"
#include "level.h"
#include "palette.h"
#include "texture.h"
#include "vbtile.h"
#include "world.h"

#define VB_TILE_ADD_TO_VERTEX_BUFFER_32X32(pos_x, pos_y, z)                    \
  G_vertex_buffer_tiles[12 * tile_index + 0]   = pos_x - 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 1]   = pos_y - 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 2]   = z;                            \
                                                                               \
  G_vertex_buffer_tiles[12 * tile_index + 3]   = pos_x + 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 4]   = pos_y - 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 5]   = z;                            \
                                                                               \
  G_vertex_buffer_tiles[12 * tile_index + 6]   = pos_x - 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 7]   = pos_y + 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 8]   = z;                            \
                                                                               \
  G_vertex_buffer_tiles[12 * tile_index + 9]   = pos_x + 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 10]  = pos_y + 16;                   \
  G_vertex_buffer_tiles[12 * tile_index + 11]  = z;

#define VB_TILE_ADD_TO_VERTEX_BUFFER_16X16(pos_x, pos_y, z)                    \
  G_vertex_buffer_tiles[12 * tile_index + 0]   = pos_x - 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 1]   = pos_y - 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 2]   = z;                            \
                                                                               \
  G_vertex_buffer_tiles[12 * tile_index + 3]   = pos_x + 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 4]   = pos_y - 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 5]   = z;                            \
                                                                               \
  G_vertex_buffer_tiles[12 * tile_index + 6]   = pos_x - 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 7]   = pos_y + 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 8]   = z;                            \
                                                                               \
  G_vertex_buffer_tiles[12 * tile_index + 9]   = pos_x + 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 10]  = pos_y + 8;                    \
  G_vertex_buffer_tiles[12 * tile_index + 11]  = z;

#define VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_32X32(ss_x, ss_y)                                   \
  G_texture_coord_buffer_tiles[8 * tile_index + 0] = G_texture_coord_table[4 * ss_x];           \
  G_texture_coord_buffer_tiles[8 * tile_index + 1] = G_texture_coord_table[4 * (16 - ss_y)];    \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * tile_index + 2] = G_texture_coord_table[4 * (ss_x + 1)];     \
  G_texture_coord_buffer_tiles[8 * tile_index + 3] = G_texture_coord_table[4 * (16 - ss_y)];    \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * tile_index + 4] = G_texture_coord_table[4 * ss_x];           \
  G_texture_coord_buffer_tiles[8 * tile_index + 5] = G_texture_coord_table[4 * (15 - ss_y)];    \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * tile_index + 6] = G_texture_coord_table[4 * (ss_x + 1)];     \
  G_texture_coord_buffer_tiles[8 * tile_index + 7] = G_texture_coord_table[4 * (15 - ss_y)];

#define VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                   \
  G_texture_coord_buffer_tiles[8 * tile_index + 0] = G_texture_coord_table[2 * ss_x];           \
  G_texture_coord_buffer_tiles[8 * tile_index + 1] = G_texture_coord_table[2 * (32 - ss_y)];    \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * tile_index + 2] = G_texture_coord_table[2 * (ss_x + 1)];     \
  G_texture_coord_buffer_tiles[8 * tile_index + 3] = G_texture_coord_table[2 * (32 - ss_y)];    \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * tile_index + 4] = G_texture_coord_table[2 * ss_x];           \
  G_texture_coord_buffer_tiles[8 * tile_index + 5] = G_texture_coord_table[2 * (31 - ss_y)];    \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * tile_index + 6] = G_texture_coord_table[2 * (ss_x + 1)];     \
  G_texture_coord_buffer_tiles[8 * tile_index + 7] = G_texture_coord_table[2 * (31 - ss_y)];

#define VB_TILE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                               \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 0] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 1] = G_palette_coord_table[palette];         \
                                                                                                    \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 2] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 3] = G_palette_coord_table[palette];         \
                                                                                                    \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 4] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 5] = G_palette_coord_table[palette];         \
                                                                                                    \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 6] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_tiles[8 * tile_index + 7] = G_palette_coord_table[palette];

#define VB_TILE_ADD_TO_ELEMENT_BUFFER()                                        \
  G_index_buffer_tiles[6 * tile_index + 0] = 4 * tile_index + 0;               \
  G_index_buffer_tiles[6 * tile_index + 1] = 4 * tile_index + 2;               \
  G_index_buffer_tiles[6 * tile_index + 2] = 4 * tile_index + 1;               \
                                                                               \
  G_index_buffer_tiles[6 * tile_index + 3] = 4 * tile_index + 1;               \
  G_index_buffer_tiles[6 * tile_index + 4] = 4 * tile_index + 2;               \
  G_index_buffer_tiles[6 * tile_index + 5] = 4 * tile_index + 3;

#define VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette)  \
  if (tile_index < GRAPHICS_BACKDROP_TILES_END_INDEX)                                         \
  {                                                                                           \
    VB_TILE_ADD_TO_VERTEX_BUFFER_16X16(pos_x, pos_y, z)                                       \
    VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                     \
    VB_TILE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                             \
    VB_TILE_ADD_TO_ELEMENT_BUFFER()                                                           \
                                                                                              \
    tile_index += 1;                                                                          \
  }

#define VB_TILE_ADD_SKY_TILE_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette) \
  if (tile_index < GRAPHICS_SKY_TILES_END_INDEX)                                        \
  {                                                                                     \
    VB_TILE_ADD_TO_VERTEX_BUFFER_32X32(pos_x, pos_y, z)                                 \
    VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_32X32(ss_x, ss_y)                               \
    VB_TILE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                       \
    VB_TILE_ADD_TO_ELEMENT_BUFFER()                                                     \
                                                                                        \
    tile_index += 1;                                                                    \
  }

#define VB_TILE_ADD_MENU_BACKGROUND_TILE_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette) \
  if (tile_index < GRAPHICS_SKY_TILES_END_INDEX)                                                    \
  {                                                                                                 \
    VB_TILE_ADD_TO_VERTEX_BUFFER_32X32(pos_x, pos_y, z)                                             \
    VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_32X32(ss_x, ss_y)                                           \
    VB_TILE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                   \
    VB_TILE_ADD_TO_ELEMENT_BUFFER()                                                                 \
                                                                                                    \
    tile_index += 1;                                                                                \
  }

#define VB_TILE_UPDATE_BACKDROP_TILES_IN_VBOS()                                                   \
  glBindBuffer(GL_ARRAY_BUFFER, G_vertex_buffer_id_tiles);                                        \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                \
                  GRAPHICS_BACKDROP_TILES_START_INDEX * 12 * sizeof(GLfloat),                     \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_BACKDROP] * 12 * sizeof(GLfloat),       \
                  &G_vertex_buffer_tiles[GRAPHICS_BACKDROP_TILES_START_INDEX * 12]);              \
                                                                                                  \
  glBindBuffer(GL_ARRAY_BUFFER, G_texture_coord_buffer_id_tiles);                                 \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                \
                  GRAPHICS_BACKDROP_TILES_START_INDEX * 8 * sizeof(GLfloat),                      \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_BACKDROP] * 8 * sizeof(GLfloat),        \
                  &G_texture_coord_buffer_tiles[GRAPHICS_BACKDROP_TILES_START_INDEX * 8]);        \
                                                                                                  \
  glBindBuffer(GL_ARRAY_BUFFER, G_lighting_and_palette_buffer_id_tiles);                          \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                \
                  GRAPHICS_BACKDROP_TILES_START_INDEX * 8 * sizeof(GLfloat),                      \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_BACKDROP] * 8 * sizeof(GLfloat),        \
                  &G_lighting_and_palette_buffer_tiles[GRAPHICS_BACKDROP_TILES_START_INDEX * 8]); \
                                                                                                  \
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G_index_buffer_id_tiles);                                 \
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,                                                        \
                  GRAPHICS_BACKDROP_TILES_START_INDEX * 6 * sizeof(unsigned short),               \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_BACKDROP] * 6 * sizeof(unsigned short), \
                  &G_index_buffer_tiles[GRAPHICS_BACKDROP_TILES_START_INDEX * 6]);

#define VB_TILE_UPDATE_SKY_TILES_IN_VBOS()                                                    \
  glBindBuffer(GL_ARRAY_BUFFER, G_vertex_buffer_id_tiles);                                    \
  glBufferSubData(GL_ARRAY_BUFFER,                                                            \
                  GRAPHICS_SKY_TILES_START_INDEX * 12 * sizeof(GLfloat),                      \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_SKY] * 12 * sizeof(GLfloat),        \
                  &G_vertex_buffer_tiles[GRAPHICS_SKY_TILES_START_INDEX * 12]);               \
                                                                                              \
  glBindBuffer(GL_ARRAY_BUFFER, G_texture_coord_buffer_id_tiles);                             \
  glBufferSubData(GL_ARRAY_BUFFER,                                                            \
                  GRAPHICS_SKY_TILES_START_INDEX * 8 * sizeof(GLfloat),                       \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_SKY] * 8 * sizeof(GLfloat),         \
                  &G_texture_coord_buffer_tiles[GRAPHICS_SKY_TILES_START_INDEX * 8]);         \
                                                                                              \
  glBindBuffer(GL_ARRAY_BUFFER, G_lighting_and_palette_buffer_id_tiles);                      \
  glBufferSubData(GL_ARRAY_BUFFER,                                                            \
                  GRAPHICS_SKY_TILES_START_INDEX * 8 * sizeof(GLfloat),                       \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_SKY] * 8 * sizeof(GLfloat),         \
                  &G_lighting_and_palette_buffer_tiles[GRAPHICS_SKY_TILES_START_INDEX * 8]);  \
                                                                                              \
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G_index_buffer_id_tiles);                             \
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,                                                    \
                  GRAPHICS_SKY_TILES_START_INDEX * 6 * sizeof(unsigned short),                \
                  G_tile_layer_counts[GRAPHICS_TILE_LAYER_SKY] * 6 * sizeof(unsigned short),  \
                  &G_index_buffer_tiles[GRAPHICS_SKY_TILES_START_INDEX * 6]);

/*******************************************************************************
** vb_tile_load_background_tilemaps()
*******************************************************************************/
short int vb_tile_load_background_tilemaps()
{
  int k;

  int m;
  int n;

  int tile_index;

  int pos_x;
  int pos_y;

  int ss_x;
  int ss_y;

  int lighting;
  int palette;

  unsigned char* room_tilemap;

  /* make sure current room is valid */
  if ((G_current_room < 0) || (G_current_room >= LEVEL_NUM_ROOMS))
    return 1;

  /* set room tilemap */
  if ((G_current_room >= 0) && (G_current_room < LEVEL_NUM_ROOMS))
    room_tilemap = &G_room_tilemaps[G_current_room][0];
  else
    room_tilemap = &G_room_tilemaps[0][0];

  /* reset tile vbo counts */
  for (k = 0; k < GRAPHICS_NUM_TILE_LAYERS; k++)
    G_tile_layer_counts[k] = 0;

  /* load backdrop tilemap to vbo */
  tile_index = GRAPHICS_BACKDROP_TILES_START_INDEX;

  /* load outer walls */

  /* determine ss_y for all outer walls based on the current world */
  if ((G_current_world >= 0) && (G_current_world < 15))
    ss_y = G_current_world;
  else
    ss_y = 0;

  /* set lighting & palette for all outer walls */
  lighting = 0;
  palette = 0;

  /* top left corner */
  pos_x = LEVEL_ROOM_TILE_SIZE * 0 + LEVEL_ROOM_TILE_SIZE_HALF + 56;
  pos_y = LEVEL_ROOM_TILE_SIZE * 0 + LEVEL_ROOM_TILE_SIZE_HALF;

  ss_x = 0;

  VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                        ss_x, ss_y, lighting, palette)

  /* top right corner */
  pos_x = LEVEL_ROOM_TILE_SIZE * 12 + LEVEL_ROOM_TILE_SIZE_HALF + 56;
  pos_y = LEVEL_ROOM_TILE_SIZE * 0 + LEVEL_ROOM_TILE_SIZE_HALF;

  ss_x = 2;

  VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                        ss_x, ss_y, lighting, palette)

  /* bottom left corner */
  pos_x = LEVEL_ROOM_TILE_SIZE * 0 + LEVEL_ROOM_TILE_SIZE_HALF + 56;
  pos_y = LEVEL_ROOM_TILE_SIZE * 13 + LEVEL_ROOM_TILE_SIZE_HALF;

  ss_x = 3;

  VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                        ss_x, ss_y, lighting, palette)

  /* bottom right corner */
  pos_x = LEVEL_ROOM_TILE_SIZE * 12 + LEVEL_ROOM_TILE_SIZE_HALF + 56;
  pos_y = LEVEL_ROOM_TILE_SIZE * 13 + LEVEL_ROOM_TILE_SIZE_HALF;

  ss_x = 5;

  VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                        ss_x, ss_y, lighting, palette)

  /* top */
  for (m = 0; m < LEVEL_ROOM_WIDTH; m++)
  {
    pos_x = LEVEL_ROOM_TILE_SIZE * (m + 1) + LEVEL_ROOM_TILE_SIZE_HALF + 56;
    pos_y = LEVEL_ROOM_TILE_SIZE * 0 + LEVEL_ROOM_TILE_SIZE_HALF;

    ss_x = 1;

    VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                          ss_x, ss_y, lighting, palette)
  }

  /* bottom */
  for (m = 0; m < LEVEL_ROOM_WIDTH; m++)
  {
    pos_x = LEVEL_ROOM_TILE_SIZE * (m + 1) + LEVEL_ROOM_TILE_SIZE_HALF + 56;
    pos_y = LEVEL_ROOM_TILE_SIZE * 13 + LEVEL_ROOM_TILE_SIZE_HALF;

    ss_x = 4;

    VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                          ss_x, ss_y, lighting, palette)
  }

  /* left */
  for (n = 0; n < LEVEL_ROOM_HEIGHT + 1; n++)
  {
    pos_x = LEVEL_ROOM_TILE_SIZE * 0 + LEVEL_ROOM_TILE_SIZE_HALF + 56;
    pos_y = LEVEL_ROOM_TILE_SIZE * (n + 1) + LEVEL_ROOM_TILE_SIZE_HALF;

    ss_x = 6;

    VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                          ss_x, ss_y, lighting, palette)
  }

  /* right */
  for (n = 0; n < LEVEL_ROOM_HEIGHT + 1; n++)
  {
    pos_x = LEVEL_ROOM_TILE_SIZE * 12 + LEVEL_ROOM_TILE_SIZE_HALF + 56;
    pos_y = LEVEL_ROOM_TILE_SIZE * (n + 1) + LEVEL_ROOM_TILE_SIZE_HALF;

    ss_x = 7;

    VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                          ss_x, ss_y, lighting, palette)
  }

  /* extra top wall (where the portcullis is) */
  for (m = 0; m < LEVEL_ROOM_WIDTH; m++)
  {
    pos_x = LEVEL_ROOM_TILE_SIZE * (m + 1) + LEVEL_ROOM_TILE_SIZE_HALF + 56;
    pos_y = LEVEL_ROOM_TILE_SIZE * 1 + LEVEL_ROOM_TILE_SIZE_HALF;

    if (m == 0)
      ss_x = 8;
    else
      ss_x = 9;

    VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                          ss_x, ss_y, lighting, palette)
  }

  /* load room tilemap to vbo (i.e., the floor tiles) */
  for (n = 0; n < LEVEL_ROOM_HEIGHT; n++)
  {
    for (m = 0; m < LEVEL_ROOM_WIDTH; m++)
    {
      /* if there is water or a bridge here, continue */
      if ((room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_WATER)       || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BRIDGE_VERT) || 
          (room_tilemap[(n * LEVEL_ROOM_WIDTH) + m] == LEVEL_ROOM_TILE_BRIDGE_HORI))
      {
        continue;
      }

      /* determine position */
      pos_x = LEVEL_ROOM_TILE_SIZE * (m + 1) + LEVEL_ROOM_TILE_SIZE_HALF + 56;
      pos_y = LEVEL_ROOM_TILE_SIZE * (n + 2) + LEVEL_ROOM_TILE_SIZE_HALF;

      /* determine texture coordinates */
      /* if this tile is by the left wall, use the shadowed version */
      if (m == 0)
        ss_x = 10;
      else
        ss_x = 11;

      ss_y = 0;

      /* determine lighting and palette */
      lighting = 0;
      palette = 6;

      /* add this tile to the buffers */
      VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_BACKDROP, 
                                            ss_x, ss_y, lighting, palette)
    }
  }

  /* update backdrop tile layer count */
  G_tile_layer_counts[GRAPHICS_TILE_LAYER_BACKDROP] = 
    tile_index - GRAPHICS_BACKDROP_TILES_START_INDEX;

  /* load sky tilemap to vbo */
  tile_index = GRAPHICS_SKY_TILES_START_INDEX;

  for (n = 0; n < LEVEL_SKY_HEIGHT; n++)
  {
    for (m = 0; m < LEVEL_SKY_WIDTH; m++)
    {
      /* determine position */
      pos_x = LEVEL_SKY_TILE_SIZE * m + LEVEL_SKY_TILE_SIZE_HALF;
      pos_y = LEVEL_SKY_TILE_SIZE * n + LEVEL_SKY_TILE_SIZE_HALF;

      /* determine texture coordinates */
      ss_x = 1;
      ss_y = 9;

      /* determine lighting and palette */
      lighting = 0;
      palette = 0;

      /* add this tile to the buffers */
      VB_TILE_ADD_SKY_TILE_TO_BUFFERS(pos_x, pos_y, GRAPHICS_Z_LEVEL_SKY, 
                                      ss_x, ss_y, lighting, palette)
    }
  }

  /* update sky tile layer count */
  G_tile_layer_counts[GRAPHICS_TILE_LAYER_SKY] = 
    tile_index - GRAPHICS_SKY_TILES_START_INDEX;

  /* update vbos */
  VB_TILE_UPDATE_BACKDROP_TILES_IN_VBOS()
  VB_TILE_UPDATE_SKY_TILES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_tile_load_menu_background()
*******************************************************************************/
short int vb_tile_load_menu_background()
{
  int k;

  int m;
  int n;

  int tile_index;

  int pos_x;
  int pos_y;

  int ss_x;
  int ss_y;

  int lighting;
  int palette;

  /* reset tile vbo counts */
  for (k = 0; k < GRAPHICS_NUM_TILE_LAYERS; k++)
    G_tile_layer_counts[k] = 0;

  /* draw the background */
  tile_index = GRAPHICS_SKY_TILES_START_INDEX;

  for (n = 0; n < LEVEL_SKY_HEIGHT; n++)
  {
    for (m = 0; m < LEVEL_SKY_WIDTH; m++)
    {
      /* determine position */
      pos_x = LEVEL_SKY_TILE_SIZE * m + LEVEL_SKY_TILE_SIZE_HALF;
      pos_y = LEVEL_SKY_TILE_SIZE * n + LEVEL_SKY_TILE_SIZE_HALF;

      /* determine texture coordinates */
      ss_x = 0;
      ss_y = 9;

      /* determine lighting and palette */
      lighting = 0;
      palette = 0;

      /* add this tile to the buffers */
      VB_TILE_ADD_MENU_BACKGROUND_TILE_TO_BUFFERS(pos_x, pos_y, GRAPHICS_Z_LEVEL_SKY, 
                                      ss_x, ss_y, lighting, palette)
    }
  }

  /* update sky tile layer count */
  G_tile_layer_counts[GRAPHICS_TILE_LAYER_SKY] = 
    tile_index - GRAPHICS_SKY_TILES_START_INDEX;

  /* update vbos */
  VB_TILE_UPDATE_SKY_TILES_IN_VBOS()

  return 0;
}

