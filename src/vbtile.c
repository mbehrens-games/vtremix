/*******************************************************************************
** vbtile.c (vbo population functions - tiles)
*******************************************************************************/

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "graphics.h"
#include "level.h"
#include "vbtile.h"

#define VB_TILE_ADD_TO_VERTEX_BUFFER_16X16(pos_x, pos_y, z)                    \
  G_vertex_buffer_tiles[12 * G_num_tiles + 0]   = pos_x - 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 1]   = pos_y - 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 2]   = z;                           \
                                                                               \
  G_vertex_buffer_tiles[12 * G_num_tiles + 3]   = pos_x + 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 4]   = pos_y - 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 5]   = z;                           \
                                                                               \
  G_vertex_buffer_tiles[12 * G_num_tiles + 6]   = pos_x - 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 7]   = pos_y + 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 8]   = z;                           \
                                                                               \
  G_vertex_buffer_tiles[12 * G_num_tiles + 9]   = pos_x + 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 10]  = pos_y + 8;                   \
  G_vertex_buffer_tiles[12 * G_num_tiles + 11]  = z;

#define VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                   \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 0] = G_texture_coord_table[2 * ss_x];          \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 1] = G_texture_coord_table[2 * (32 - ss_y)];   \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 2] = G_texture_coord_table[2 * (ss_x + 1)];    \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 3] = G_texture_coord_table[2 * (32 - ss_y)];   \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 4] = G_texture_coord_table[2 * ss_x];          \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 5] = G_texture_coord_table[2 * (31 - ss_y)];   \
                                                                                                \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 6] = G_texture_coord_table[2 * (ss_x + 1)];    \
  G_texture_coord_buffer_tiles[8 * G_num_tiles + 7] = G_texture_coord_table[2 * (31 - ss_y)];

#define VB_TILE_ADD_TO_PALETTE_COORD_BUFFER(lighting, palette)                                              \
  G_palette_coord_buffer_tiles[4 * G_num_tiles + 0] = G_palette_coord_table[4 + (8 * palette) + lighting];  \
                                                                                                            \
  G_palette_coord_buffer_tiles[4 * G_num_tiles + 1] = G_palette_coord_table[4 + (8 * palette) + lighting];  \
                                                                                                            \
  G_palette_coord_buffer_tiles[4 * G_num_tiles + 2] = G_palette_coord_table[4 + (8 * palette) + lighting];  \
                                                                                                            \
  G_palette_coord_buffer_tiles[4 * G_num_tiles + 3] = G_palette_coord_table[4 + (8 * palette) + lighting];

#define VB_TILE_ADD_TO_ELEMENT_BUFFER()                                        \
  G_index_buffer_tiles[6 * G_num_tiles + 0] = 4 * G_num_tiles + 0;             \
  G_index_buffer_tiles[6 * G_num_tiles + 1] = 4 * G_num_tiles + 2;             \
  G_index_buffer_tiles[6 * G_num_tiles + 2] = 4 * G_num_tiles + 1;             \
                                                                               \
  G_index_buffer_tiles[6 * G_num_tiles + 3] = 4 * G_num_tiles + 1;             \
  G_index_buffer_tiles[6 * G_num_tiles + 4] = 4 * G_num_tiles + 2;             \
  G_index_buffer_tiles[6 * G_num_tiles + 5] = 4 * G_num_tiles + 3;

#define VB_TILE_ADD_BACKDROP_TILE_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette)  \
  if (G_num_tiles < GRAPHICS_MAX_TILES)                                                       \
  {                                                                                           \
    VB_TILE_ADD_TO_VERTEX_BUFFER_16X16(pos_x, pos_y, z)                                       \
    VB_TILE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                     \
    VB_TILE_ADD_TO_PALETTE_COORD_BUFFER(lighting, palette)                                    \
    VB_TILE_ADD_TO_ELEMENT_BUFFER()                                                           \
                                                                                              \
    G_num_tiles += 1;                                                                         \
  }

#define VB_TILE_UPDATE_STATIC_TILE_BUFFERS()                                   \
  glBindBuffer(GL_ARRAY_BUFFER, G_vertex_buffer_id_tiles);                     \
  glBufferSubData(GL_ARRAY_BUFFER, 0,                                          \
                  G_num_tiles * 12 * sizeof(GLfloat),                          \
                  G_vertex_buffer_tiles);                                      \
                                                                               \
  glBindBuffer(GL_ARRAY_BUFFER, G_texture_coord_buffer_id_tiles);              \
  glBufferSubData(GL_ARRAY_BUFFER, 0,                                          \
                  G_num_tiles * 8 * sizeof(GLfloat),                           \
                  G_texture_coord_buffer_tiles);                               \
                                                                               \
  glBindBuffer(GL_ARRAY_BUFFER, G_palette_coord_buffer_id_tiles);              \
  glBufferSubData(GL_ARRAY_BUFFER, 0,                                          \
                  G_num_tiles * 4 * sizeof(GLfloat),                           \
                  G_palette_coord_buffer_tiles);                               \
                                                                               \
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G_index_buffer_id_tiles);              \
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,                                  \
                  G_num_tiles * 6 * sizeof(unsigned short),                    \
                  G_index_buffer_tiles);

/*******************************************************************************
** vb_tile_load_background_tilemap()
*******************************************************************************/
short int vb_tile_load_background_tilemap()
{
  int m;
  int n;

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

  /* initialize tile count */
  G_num_tiles = 0;

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

  /* update vbo */
  VB_TILE_UPDATE_STATIC_TILE_BUFFERS()

  return 0;
}

