/*******************************************************************************
** vbsprite.c (vbo population functions - sprites)
*******************************************************************************/

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "animate.h"
#include "global.h"
#include "graphics.h"
#include "grid.h"
#include "palette.h"
#include "state.h"
#include "subpixel.h"
#include "texture.h"
#include "thing.h"
#include "vbsprite.h"
#include "world.h"

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_NORMAL(pos_x, pos_y, z)           \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_ROTATE_90(pos_x, pos_y, z)        \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_ROTATE_180(pos_x, pos_y, z)       \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_ROTATE_270(pos_x, pos_y, z)       \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_FLIP_HORI(pos_x, pos_y, z)        \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_FLIP_VERT(pos_x, pos_y, z)        \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_FLIP_AND_ROTATE_90(pos_x, pos_y, z)  \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                           \
                                                                                  \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                           \
                                                                                  \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                           \
                                                                                  \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_FLIP_AND_ROTATE_270(pos_x, pos_y, z) \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                           \
                                                                                  \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                           \
                                                                                  \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                           \
                                                                                  \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y - 8;                   \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

/* for the bunny sprites (16x24), the position is lower   */
/* than the center when the sprite is right side up, and  */
/* higher than the center when the sprite is upside down  */
#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X24_NORMAL(pos_x, pos_y, z)           \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X24_FLIP_HORI(pos_x, pos_y, z)        \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X24_FLIP_VERT(pos_x, pos_y, z)        \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y + 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y + 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X24_FLIP_BOTH(pos_x, pos_y, z)        \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y + 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y + 16;               \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y - 8;                \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_SPRITE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                     \
  G_texture_coord_buffer_sprites[8 * sprite_index + 0] = G_texture_coord_table[2 * ss_x];           \
  G_texture_coord_buffer_sprites[8 * sprite_index + 1] = G_texture_coord_table[2 * (32 - ss_y)];    \
                                                                                                    \
  G_texture_coord_buffer_sprites[8 * sprite_index + 2] = G_texture_coord_table[2 * (ss_x + 1)];     \
  G_texture_coord_buffer_sprites[8 * sprite_index + 3] = G_texture_coord_table[2 * (32 - ss_y)];    \
                                                                                                    \
  G_texture_coord_buffer_sprites[8 * sprite_index + 4] = G_texture_coord_table[2 * ss_x];           \
  G_texture_coord_buffer_sprites[8 * sprite_index + 5] = G_texture_coord_table[2 * (31 - ss_y)];    \
                                                                                                    \
  G_texture_coord_buffer_sprites[8 * sprite_index + 6] = G_texture_coord_table[2 * (ss_x + 1)];     \
  G_texture_coord_buffer_sprites[8 * sprite_index + 7] = G_texture_coord_table[2 * (31 - ss_y)];

/* the cell is the top-left 8x8 cell on the 16x24 sprite */
#define VB_SPRITE_ADD_TO_TEXTURE_COORD_BUFFER_16X24(cell_x, cell_y)                             \
  G_texture_coord_buffer_sprites[8 * sprite_index + 0] = G_texture_coord_table[cell_x];         \
  G_texture_coord_buffer_sprites[8 * sprite_index + 1] = G_texture_coord_table[64 - cell_y];    \
                                                                                                \
  G_texture_coord_buffer_sprites[8 * sprite_index + 2] = G_texture_coord_table[cell_x + 2];     \
  G_texture_coord_buffer_sprites[8 * sprite_index + 3] = G_texture_coord_table[64 - cell_y];    \
                                                                                                \
  G_texture_coord_buffer_sprites[8 * sprite_index + 4] = G_texture_coord_table[cell_x];         \
  G_texture_coord_buffer_sprites[8 * sprite_index + 5] = G_texture_coord_table[61 - cell_y];    \
                                                                                                \
  G_texture_coord_buffer_sprites[8 * sprite_index + 6] = G_texture_coord_table[cell_x + 2];     \
  G_texture_coord_buffer_sprites[8 * sprite_index + 7] = G_texture_coord_table[61 - cell_y];

#define VB_SPRITE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                 \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 0] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 1] = G_palette_coord_table[palette];         \
                                                                                                        \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 2] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 3] = G_palette_coord_table[palette];         \
                                                                                                        \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 4] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 5] = G_palette_coord_table[palette];         \
                                                                                                        \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 6] = G_lighting_coord_table[8 + lighting];   \
  G_lighting_and_palette_buffer_sprites[8 * sprite_index + 7] = G_palette_coord_table[palette];

#define VB_SPRITE_ADD_TO_ELEMENT_BUFFER()                                      \
  G_index_buffer_sprites[6 * sprite_index + 0] = 4 * sprite_index + 0;         \
  G_index_buffer_sprites[6 * sprite_index + 1] = 4 * sprite_index + 2;         \
  G_index_buffer_sprites[6 * sprite_index + 2] = 4 * sprite_index + 1;         \
                                                                               \
  G_index_buffer_sprites[6 * sprite_index + 3] = 4 * sprite_index + 1;         \
  G_index_buffer_sprites[6 * sprite_index + 4] = 4 * sprite_index + 2;         \
  G_index_buffer_sprites[6 * sprite_index + 5] = 4 * sprite_index + 3;

#define VB_SPRITE_ADD_GRID_OBJECT_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, mode)  \
  if (sprite_index < GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_END_INDEX)                            \
  {                                                                                                 \
    VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_##mode(pos_x, pos_y, z)                                    \
    VB_SPRITE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                         \
    VB_SPRITE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                 \
    VB_SPRITE_ADD_TO_ELEMENT_BUFFER()                                                               \
                                                                                                    \
    sprite_index += 1;                                                                              \
  }

#define VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, mode)  \
  if (sprite_index < GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_END_INDEX)                      \
  {                                                                                           \
    VB_SPRITE_ADD_TO_VERTEX_BUFFER_16X16_##mode(pos_x, pos_y, z)                              \
    VB_SPRITE_ADD_TO_TEXTURE_COORD_BUFFER_16X16(ss_x, ss_y)                                   \
    VB_SPRITE_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                           \
    VB_SPRITE_ADD_TO_ELEMENT_BUFFER()                                                         \
                                                                                              \
    sprite_index += 1;                                                                        \
  }

#define VB_SPRITE_UPDATE_GRID_OBJECTS_AND_THINGS_SPRITES_IN_VBOS()                                                    \
  glBindBuffer(GL_ARRAY_BUFFER, G_vertex_buffer_id_sprites);                                                          \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                                    \
                  GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 12 * sizeof(GLfloat),                        \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] * 12 * sizeof(GLfloat),        \
                  &G_vertex_buffer_sprites[GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 12]);               \
                                                                                                                      \
  glBindBuffer(GL_ARRAY_BUFFER, G_texture_coord_buffer_id_sprites);                                                   \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                                    \
                  GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 8 * sizeof(GLfloat),                         \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] * 8 * sizeof(GLfloat),         \
                  &G_texture_coord_buffer_sprites[GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 8]);         \
                                                                                                                      \
  glBindBuffer(GL_ARRAY_BUFFER, G_lighting_and_palette_buffer_id_sprites);                                            \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                                    \
                  GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 8 * sizeof(GLfloat),                         \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] * 8 * sizeof(GLfloat),         \
                  &G_lighting_and_palette_buffer_sprites[GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 8]);  \
                                                                                                                      \
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G_index_buffer_id_sprites);                                                   \
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,                                                                            \
                  GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 6 * sizeof(unsigned short),                  \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] * 6 * sizeof(unsigned short),  \
                  &G_index_buffer_sprites[GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX * 6]);

/*******************************************************************************
** vb_sprite_load_grid_objects_and_things()
*******************************************************************************/
short int vb_sprite_load_grid_objects_and_things()
{
  int k;

  int m;
  int n;

  grid_box* b;

  thing* t;

  int sprite_index;

  int pos_x;
  int pos_y;

  int ss_x;
  int ss_y;

  int lighting;
  int palette;

  float z;

  unsigned int adjusted_timer_count;
  unsigned int freeze_timer_count;

  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] = 0;

  /* load water */
  sprite_index = GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX;

  for (n = 1; n < GRID_HEIGHT_IN_BOXES - 1; n++)
  {
    for (m = 1; m < GRID_WIDTH_IN_BOXES - 1; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH_IN_BOXES) + m];

      if (b->object == GRID_OBJECT_NONE)
        continue;

      /* determine position */
      pos_x = GRID_BOX_SIZE_IN_PIXELS * m + GRID_BOX_SIZE_IN_PIXELS_HALF + 56;
      pos_y = GRID_BOX_SIZE_IN_PIXELS * n + GRID_BOX_SIZE_IN_PIXELS_HALF;

      /* determine adjusted timer count */
      adjusted_timer_count = (G_timer_count + G_collision_grid[(n * GRID_WIDTH_IN_BOXES) + m].timer_offset) % 240;

      /* determine texture coordinates, lighting, and palette */
      if ((b->object == GRID_OBJECT_WATER)                  || 
          (b->object == GRID_OBJECT_BRIDGE_VERT)            || 
          (b->object == GRID_OBJECT_BRIDGE_HORI)            || 
          (b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING) || 
          (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING) || 
          (b->object == GRID_OBJECT_SUBMERGED_BOX_WOOD)     || 
          (b->object == GRID_OBJECT_SUBMERGED_BOX_METAL)    || 
          (b->object == GRID_OBJECT_SUBMERGED_BOX_STONE)    || 
          (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE)     || 
          (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING))
      {
        /* if this tile is by the left wall, use the shadowed version */
        if (m == 1)
          ss_x = 10;
        else
          ss_x = 11;

        if (ANIM_GLOBAL_FRAME(WATER) == 0)
          ss_y = 2;
        else if (ANIM_GLOBAL_FRAME(WATER) == 1)
          ss_y = 3;
        else if (ANIM_GLOBAL_FRAME(WATER) == 2)
          ss_y = 4;
        else if (ANIM_GLOBAL_FRAME(WATER) == 3)
          ss_y = 5;
        else if (ANIM_GLOBAL_FRAME(WATER) == 4)
          ss_y = 4;
        else
          ss_y = 3;

        lighting = 0;
        palette = 0;
      }
      else
        continue;

      /* add this sprite to the buffers */
      VB_SPRITE_ADD_GRID_OBJECT_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_GRID_BACK_OBJECTS, 
                                            ss_x, ss_y, lighting, palette, NORMAL)
    }
  }

  /* load objects */
  for (n = 0; n < GRID_HEIGHT_IN_BOXES; n++)
  {
    for (m = 0; m < GRID_WIDTH_IN_BOXES; m++)
    {
      b = &G_collision_grid[(n * GRID_WIDTH_IN_BOXES) + m];

      if (b->object == GRID_OBJECT_NONE)
        continue;

      /* determine position */
      pos_x = GRID_BOX_SIZE_IN_PIXELS * m + GRID_BOX_SIZE_IN_PIXELS_HALF + 56;
      pos_y = GRID_BOX_SIZE_IN_PIXELS * n + GRID_BOX_SIZE_IN_PIXELS_HALF;

      /* determine adjusted timer count */
      adjusted_timer_count = (G_timer_count + G_collision_grid[(n * GRID_WIDTH_IN_BOXES) + m].timer_offset) % 240;

      /* determine texture coordinates, lighting, and palette */
      if (b->object == GRID_OBJECT_BLOCK)
      {
        ss_x = 10;
        ss_y = 11;

        /* shift to alternate colors if necessary */
        if ((G_current_world == 1) || 
            (G_current_world == 3))
        {
          ss_x += 1;
        }

        lighting = 0;

        if (G_current_world == 0)
          palette = 6;
        else if (G_current_world == 1)
          palette = 5;
        else if (G_current_world == 2)
          palette = 4;
        else if (G_current_world == 3)
          palette = 5;
        else if (G_current_world == 4)
          palette = 0;
        else if (G_current_world == 5)
          palette = 1;
        else if (G_current_world == 6)
          palette = 4;
        else if (G_current_world == 7)
          palette = 1;
        else if (G_current_world == 8)
          palette = 2;
        else if (G_current_world == 9)
          palette = 3;
        else if (G_current_world == 10)
          palette = 5;
        else if (G_current_world == 11)
          palette = 1;
        else if (G_current_world == 12)
          palette = 5;
        else if (G_current_world == 13)
          palette = 0;
        else if (G_current_world == 14)
          palette = 6;
        else
          palette = 6;
      }
      else if (b->object == GRID_OBJECT_PILLAR)
      {
        ss_x = 10;
        ss_y = 12;

        /* shift to alternate colors if necessary */
        if ((G_current_world == 1) || 
            (G_current_world == 3))
        {
          ss_x += 1;
        }

        lighting = 0;

        if (G_current_world == 0)
          palette = 6;
        else if (G_current_world == 1)
          palette = 5;
        else if (G_current_world == 2)
          palette = 4;
        else if (G_current_world == 3)
          palette = 5;
        else if (G_current_world == 4)
          palette = 0;
        else if (G_current_world == 5)
          palette = 1;
        else if (G_current_world == 6)
          palette = 4;
        else if (G_current_world == 7)
          palette = 1;
        else if (G_current_world == 8)
          palette = 2;
        else if (G_current_world == 9)
          palette = 3;
        else if (G_current_world == 10)
          palette = 5;
        else if (G_current_world == 11)
          palette = 1;
        else if (G_current_world == 12)
          palette = 5;
        else if (G_current_world == 13)
          palette = 0;
        else if (G_current_world == 14)
          palette = 6;
        else
          palette = 6;
      }
      else if (b->object == GRID_OBJECT_BRIDGE_VERT)
      {
        ss_x = 29;
        ss_y = 14;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_BRIDGE_HORI)
      {
        ss_x = 29;
        ss_y = 15;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_BRIDGE_VERT_COLLAPSING)
      {
        if (ANIM_CURRENT_FRAME(BRIDGE_COLLAPSING) == 0)
          ss_x = 30;
        else
          ss_x = 31;

        ss_y = 14;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_BRIDGE_HORI_COLLAPSING)
      {
        if (ANIM_CURRENT_FRAME(BRIDGE_COLLAPSING) == 0)
          ss_x = 30;
        else
          ss_x = 31;

        ss_y = 15;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_PADLOCK)
      {
        ss_x = 15;
        ss_y = 13;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_PADLOCK_OPENING)
      {
        if (ANIM_CURRENT_FRAME(PADLOCK_OPENING) == 0)
          ss_x = 16;
        else if (ANIM_CURRENT_FRAME(PADLOCK_OPENING) == 1)
          ss_x = 17;
        else if (ANIM_CURRENT_FRAME(PADLOCK_OPENING) == 2)
          ss_x = 18;
        else
          ss_x = 19;

        ss_y = 13;

        lighting = 0;
        palette = 0;
      }
      else if ( (b->object == GRID_OBJECT_ARROWS_RIGHT) || 
                (b->object == GRID_OBJECT_ARROWS_UP)    || 
                (b->object == GRID_OBJECT_ARROWS_LEFT)  || 
                (b->object == GRID_OBJECT_ARROWS_DOWN))
      {
        if (ANIM_GLOBAL_FRAME(ARROWS) == 0)
          ss_x = 25;
        else if (ANIM_GLOBAL_FRAME(ARROWS) == 1)
          ss_x = 26;
        else if (ANIM_GLOBAL_FRAME(ARROWS) == 2)
          ss_x = 27;
        else if (ANIM_GLOBAL_FRAME(ARROWS) == 3)
          ss_x = 27;
        else if (ANIM_GLOBAL_FRAME(ARROWS) == 4)
          ss_x = 28;
        else
          ss_x = 25;

        ss_y = 14;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_FLOOR_SPIKES_DOWN)
      {
        if (b->state == STATE_CHANGING)
        {
          if (ANIM_CURRENT_FRAME(SPIKES) == 0)
            ss_x = 27;
          else
            ss_x = 26;
        }
        else
          ss_x = 25;

        ss_y = 15;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_FLOOR_SPIKES_UP)
      {
        if (b->state == STATE_CHANGING)
        {
          if (ANIM_CURRENT_FRAME(SPIKES) == 0)
            ss_x = 26;
          else
            ss_x = 27;
        }
        else
          ss_x = 28;

        ss_y = 15;

        lighting = 0;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_AUDREY_SPAWNER)
      {
        if (ANIM_GLOBAL_FRAME(AUDREY_SPAWNER) == 0)
          ss_x = 12;
        else if (ANIM_GLOBAL_FRAME(AUDREY_SPAWNER) == 1)
          ss_x = 13;
        else if (ANIM_GLOBAL_FRAME(AUDREY_SPAWNER) == 2)
          ss_x = 14;
        else if (ANIM_GLOBAL_FRAME(AUDREY_SPAWNER) == 3)
          ss_x = 15;
        else if (ANIM_GLOBAL_FRAME(AUDREY_SPAWNER) == 4)
          ss_x = 14;
        else
          ss_x = 13;

        ss_y = 8;

        lighting = 0;
        palette = 0;
      }
      else if ( (b->object == GRID_OBJECT_PORTCULLIS_CLOSED) || 
                (b->object == GRID_OBJECT_PORTCULLIS_OPEN))
      {
        if (b->object == GRID_OBJECT_PORTCULLIS_CLOSED)
        {
          ss_x = 10;
          ss_y = 7;
        }
        else if (b->state == STATE_CHANGING)
        {
          if (ANIM_CURRENT_FRAME(PORTCULLIS_OPENING) == 0)
          {
            ss_x = 11;
            ss_y = 7;
          }
          else
          {
            ss_x = 10;
            ss_y = 8;
          }
        }
        else
        {
          ss_x = 11;
          ss_y = 8;
        }

        /* shift to alternate colors if necessary */
        if (G_current_world == 3)
        {
          ss_y += 2;
        }

        lighting = 0;

        if (G_current_world == 0)
          palette = 1;
        else if (G_current_world == 1)
          palette = 1;
        else if (G_current_world == 2)
          palette = 4;
        else if (G_current_world == 3)
          palette = 5;
        else if (G_current_world == 4)
          palette = 0;
        else if (G_current_world == 5)
          palette = 1;
        else if (G_current_world == 6)
          palette = 4;
        else if (G_current_world == 7)
          palette = 1;
        else if (G_current_world == 8)
          palette = 0;
        else if (G_current_world == 9)
          palette = 3;
        else if (G_current_world == 10)
          palette = 5;
        else if (G_current_world == 11)
          palette = 1;
        else if (G_current_world == 12)
          palette = 5;
        else if (G_current_world == 13)
          palette = 0;
        else if (G_current_world == 14)
          palette = 1;
        else
          palette = 6;
      }
      else if ( (b->object == GRID_OBJECT_SUBMERGED_BOX_WOOD)  || 
                (b->object == GRID_OBJECT_SUBMERGED_BOX_METAL) || 
                (b->object == GRID_OBJECT_SUBMERGED_BOX_STONE))
      {
        ss_x = 20;
        ss_y = 13;

        lighting = -2;

        if (b->object == GRID_OBJECT_SUBMERGED_BOX_WOOD)
          palette = 0;
        else if (b->object == GRID_OBJECT_SUBMERGED_BOX_METAL)
          palette = 3;
        else if (b->object == GRID_OBJECT_SUBMERGED_BOX_STONE)
          palette = 2;
        else
          palette = 0;
      }
      else if (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE)
      {
        if (ANIM_CURRENT_FRAME(ICE_CUBE) == 0)
          ss_x = 26;
        else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 1)
          ss_x = 27;
        else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 2)
          ss_x = 28;
        else
          ss_x = 25;

        ss_y = 13;

        lighting = -2;
        palette = 0;
      }
      else if (b->object == GRID_OBJECT_SUBMERGED_ICE_CUBE_THAWING)
      {
        ss_x = 25;
        ss_y = 13;

        if (b->state == STATE_READY_TO_CHANGE)
          lighting = 0;
        else
        {
          if (ANIM_CURRENT_FRAME(ICE_CUBE) == 0)
            lighting = -2;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 1)
            lighting = -2;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 2)
            lighting = -2;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 3)
            lighting = -2;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 4)
            lighting = -2;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 5)
            lighting = -1;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 6)
            lighting = 0;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 7)
            lighting = 1;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 8)
            lighting = 0;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 9)
            lighting = -1;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 10)
            lighting = 0;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 11)
            lighting = 1;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 12)
            lighting = 0;
          else if (ANIM_CURRENT_FRAME(ICE_CUBE) == 13)
            lighting = -1;
          else
            lighting = 0;
        }

        palette = 0;
      }
      else
        continue;

      /* add this sprite to the buffers */
      if (b->object == GRID_OBJECT_ARROWS_UP)
      {
        VB_SPRITE_ADD_GRID_OBJECT_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_GRID_FRONT_OBJECTS, 
                                              ss_x, ss_y, lighting, palette, ROTATE_90)
      }
      else if (b->object == GRID_OBJECT_ARROWS_LEFT)
      {
        VB_SPRITE_ADD_GRID_OBJECT_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_GRID_FRONT_OBJECTS, 
                                              ss_x, ss_y, lighting, palette, FLIP_HORI)
      }
      else if (b->object == GRID_OBJECT_ARROWS_DOWN)
      {
        VB_SPRITE_ADD_GRID_OBJECT_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_GRID_FRONT_OBJECTS, 
                                              ss_x, ss_y, lighting, palette, FLIP_AND_ROTATE_90)
      }
      else
      {
        VB_SPRITE_ADD_GRID_OBJECT_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_GRID_FRONT_OBJECTS, 
                                              ss_x, ss_y, lighting, palette, NORMAL)
      }
    }
  }

  /* load things */
  for (k = 0; k < WORLD_MAX_THINGS; k++)
  {
    t = &G_world_all_things[k];

    if (t->type == THING_TYPE_NONE)
      continue;

    /* determine position */
    pos_x = (t->pos_x + SUBPIXEL_MANTISSA_HALF) / SUBPIXEL_MANTISSA_FULL;
    pos_y = (t->pos_y + SUBPIXEL_MANTISSA_HALF) / SUBPIXEL_MANTISSA_FULL;

    pos_x += 56;

    /* determine adjusted timer count */
    adjusted_timer_count = (G_timer_count + t->timer_offset) % 240;
    freeze_timer_count = (G_timer_count + t->freeze_timer_offset) % 240;

    /* determine texture coordinates, lighting, and palette */
    /* vampire & bat */
    if ((t->type == THING_TYPE_VAMPIRE)               || 
        (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE) || 
        (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE))
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(VAMPIRE) == 0)
          ss_x = 24;
        else if (ANIM_GLOBAL_FRAME(VAMPIRE) == 1)
          ss_x = 25;
        else if (ANIM_GLOBAL_FRAME(VAMPIRE) == 2)
          ss_x = 24;
        else
          ss_x = 26;
      }
      else if (t->state == STATE_CASTING)
        ss_x = 27;
      else
        ss_x = 24;

      if (t->facing == THING_FACING_RIGHT)
        ss_y = 12;
      else if (t->facing == THING_FACING_UP)
        ss_y = 11;
      else if (t->facing == THING_FACING_LEFT)
        ss_y = 12;
      else if (t->facing == THING_FACING_DOWN)
        ss_y = 10;

      lighting = 0;

      if (t->type == THING_TYPE_VAMPIRE)
        palette = 0;
      else if (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)
        palette = 2;
      else if (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)
        palette = 3;
      else
        palette = 0;
    }
    else if ( (t->type == THING_TYPE_BAT)               || 
              (t->type == THING_TYPE_DOUBLE_NORMAL_BAT) || 
              (t->type == THING_TYPE_DOUBLE_REVERSE_BAT))
    {
      if (ANIM_GLOBAL_FRAME(BAT) == 0)
        ss_x = 16;
      else if (ANIM_GLOBAL_FRAME(BAT) == 1)
        ss_x = 17;
      else if (ANIM_GLOBAL_FRAME(BAT) == 2)
        ss_x = 16;
      else
        ss_x = 18;

      ss_y = 8;

      if (t->facing == THING_FACING_RIGHT)
        ss_x += 6;
      else if (t->facing == THING_FACING_UP)
        ss_x += 3;
      else if (t->facing == THING_FACING_LEFT)
        ss_x += 6;
      else if (t->facing == THING_FACING_DOWN)
        ss_x += 0;

      lighting = 0;

      if (t->type == THING_TYPE_BAT)
        palette = 0;
      else if (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)
        palette = 2;
      else if (t->type == THING_TYPE_DOUBLE_REVERSE_BAT)
        palette = 3;
      else
        palette = 0;
    }
    /* spells */
    else if (t->type == THING_TYPE_EVIL_EYE_SPELL)
    {
      if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 0)
        ss_x = 10;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 1)
        ss_x = 11;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 2)
        ss_x = 12;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 3)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 4)
        ss_x = 14;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 5)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 6)
        ss_x = 12;
      else
        ss_x = 11;

      ss_y = 13;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_FIRE_SPELL)
    {
      if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 0)
        ss_x = 10;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 1)
        ss_x = 11;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 2)
        ss_x = 12;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 3)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 4)
        ss_x = 14;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 5)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 6)
        ss_x = 12;
      else
        ss_x = 11;

      ss_y = 15;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_ICE_SPELL)
    {
      if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 0)
        ss_x = 15;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 1)
        ss_x = 16;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 2)
        ss_x = 17;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 3)
        ss_x = 18;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 4)
        ss_x = 19;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 5)
        ss_x = 20;
      else if (ANIM_GLOBAL_FRAME(SPELL_FAST) == 6)
        ss_x = 21;
      else
        ss_x = 22;

      ss_y = 14;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_RING_SPELL)
    {
      if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 0)
        ss_x = 10;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 1)
        ss_x = 11;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 2)
        ss_x = 12;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 3)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 4)
        ss_x = 14;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 5)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 6)
        ss_x = 12;
      else
        ss_x = 11;

      ss_y = 14;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_TWIN_SPELL)
    {
      if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 0)
        ss_x = 15;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 1)
        ss_x = 16;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 2)
        ss_x = 17;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 3)
        ss_x = 18;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 4)
        ss_x = 19;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 5)
        ss_x = 20;
      else if (ANIM_GLOBAL_FRAME(SPELL_SLOW) == 6)
        ss_x = 21;
      else
        ss_x = 22;

      ss_y = 15;

      lighting = 0;
      palette = 0;
    }
    /* pickups */
    else if ( (t->type == THING_TYPE_BLOOD_VIAL)        || 
              (t->type == THING_TYPE_BAT_VIAL)          || 
              (t->type == THING_TYPE_MANA_PRISM)        || 
              (t->type == THING_TYPE_SKELETON_KEY)      || 
              (t->type == THING_TYPE_SPIKE_DROPPER)     || 
              (t->type == THING_TYPE_SPIKE_RAISER)      || 
              (t->type == THING_TYPE_ARROW_SPINNER_CCW) || 
              (t->type == THING_TYPE_ARROW_SPINNER_CW))
    {
      if (ANIM_CURRENT_FRAME(PICKUP) == 0)
        ss_x = 29;
      else if (ANIM_CURRENT_FRAME(PICKUP) == 1)
        ss_x = 30;
      else if (ANIM_CURRENT_FRAME(PICKUP) == 2)
        ss_x = 29;
      else
        ss_x = 31;

      if (t->type == THING_TYPE_BLOOD_VIAL)
        ss_y = 6;
      else if (t->type == THING_TYPE_BAT_VIAL)
        ss_y = 7;
      else if (t->type == THING_TYPE_MANA_PRISM)
        ss_y = 8;
      else if (t->type == THING_TYPE_SKELETON_KEY)
        ss_y = 9;
      else if (t->type == THING_TYPE_SPIKE_DROPPER)
        ss_y = 10;
      else if (t->type == THING_TYPE_SPIKE_RAISER)
        ss_y = 11;
      else if (t->type == THING_TYPE_ARROW_SPINNER_CCW)
        ss_y = 12;
      else if (t->type == THING_TYPE_ARROW_SPINNER_CW)
        ss_y = 13;

      lighting = 0;
      palette = 0;
    }
    /* denizens */
    else if (t->type == THING_TYPE_PUMPKIN)
    {
      if (t->state == STATE_CASTING)
      {
        if (ANIM_CURRENT_FRAME(PUMPKIN) == 0)
          ss_x = 13;
        else if (ANIM_CURRENT_FRAME(PUMPKIN) == 1)
          ss_x = 14;
        else if (ANIM_CURRENT_FRAME(PUMPKIN) == 2)
          ss_x = 15;
        else
          ss_x = 14;
      }
      else
        ss_x = 12;

      ss_y = 0;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_AUDREY)
    {
      if (ANIM_GLOBAL_FRAME(AUDREY) == 0)
        ss_x = 12;
      else if (ANIM_GLOBAL_FRAME(AUDREY) == 1)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(AUDREY) == 2)
        ss_x = 14;
      else
        ss_x = 15;

      ss_y = 7;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_GHOST_ASLEEP)
    {
      ss_x = 12;

      if (t->facing == THING_FACING_RIGHT)
        ss_y = 3;
      else if (t->facing == THING_FACING_UP)
        ss_y = 2;
      else if (t->facing == THING_FACING_LEFT)
        ss_y = 4;
      else if (t->facing == THING_FACING_DOWN)
        ss_y = 1;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_GHOST_AWAKE)
    {
      if (ANIM_GLOBAL_FRAME(GHOST) == 0)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(GHOST) == 1)
        ss_x = 14;
      else if (ANIM_GLOBAL_FRAME(GHOST) == 2)
        ss_x = 15;
      else
        ss_x = 14;

      if (t->facing == THING_FACING_RIGHT)
        ss_y = 3;
      else if (t->facing == THING_FACING_UP)
        ss_y = 2;
      else if (t->facing == THING_FACING_LEFT)
        ss_y = 4;
      else if (t->facing == THING_FACING_DOWN)
        ss_y = 1;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_SNAKE_ASLEEP)
    {
      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x = 28;
        ss_y = 9;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x = 25;
        ss_y = 8;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x = 28;
        ss_y = 9;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x = 27;
        ss_y = 9;
      }

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_SNAKE_AWAKE)
    {
      if (t->state == STATE_CASTING)
        ss_x = 28;
      else
      {
        if (ANIM_GLOBAL_FRAME(SNAKE) == 0)
          ss_x = 25;
        else if (ANIM_GLOBAL_FRAME(SNAKE) == 1)
          ss_x = 26;
        else if (ANIM_GLOBAL_FRAME(SNAKE) == 2)
          ss_x = 27;
        else
          ss_x = 26;
      }

      if (t->facing == THING_FACING_RIGHT)
        ss_y = 7;
      else if (t->facing == THING_FACING_UP)
        ss_y = 8;
      else if (t->facing == THING_FACING_LEFT)
        ss_y = 7;
      else if (t->facing == THING_FACING_DOWN)
        ss_y = 6;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_SHADE)
    {
      if (t->state == STATE_CASTING)
        ss_x = 15;
      else
      {
        if (ANIM_GLOBAL_FRAME(SHADE) == 0)
          ss_x = 12;
        else if (ANIM_GLOBAL_FRAME(SHADE) == 1)
          ss_x = 13;
        else if (ANIM_GLOBAL_FRAME(SHADE) == 2)
          ss_x = 12;
        else
          ss_x = 14;
      }

      ss_y = 10;

      if (t->facing == THING_FACING_RIGHT)
        ss_x += 8;
      else if (t->facing == THING_FACING_UP)
        ss_x += 4;
      else if (t->facing == THING_FACING_LEFT)
        ss_x += 8;
      else if (t->facing == THING_FACING_DOWN)
        ss_x += 0;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_WITCH_ASLEEP)
    {
      if (t->facing == THING_FACING_RIGHT)
        ss_x = 26;
      else if (t->facing == THING_FACING_UP)
        ss_x = 25;
      else if (t->facing == THING_FACING_LEFT)
        ss_x = 26;
      else if (t->facing == THING_FACING_DOWN)
        ss_x = 24;

      ss_y = 9;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_WITCH_AWAKE)
    {
      if (t->state == STATE_CASTING)
        ss_x = 15;
      else
      {
        if (ANIM_GLOBAL_FRAME(WITCH) == 0)
          ss_x = 12;
        else if (ANIM_GLOBAL_FRAME(WITCH) == 1)
          ss_x = 13;
        else if (ANIM_GLOBAL_FRAME(WITCH) == 2)
          ss_x = 14;
        else
          ss_x = 13;
      }

      ss_y = 9;

      if (t->facing == THING_FACING_RIGHT)
        ss_x += 8;
      else if (t->facing == THING_FACING_UP)
        ss_x += 4;
      else if (t->facing == THING_FACING_LEFT)
        ss_x += 8;
      else if (t->facing == THING_FACING_DOWN)
        ss_x += 0;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_SORCERESS)
    {
      if (t->state == STATE_CASTING)
        ss_x = 15;
      else
      {
        if (ANIM_GLOBAL_FRAME(SORCERESS) == 0)
          ss_x = 12;
        else if (ANIM_GLOBAL_FRAME(SORCERESS) == 1)
          ss_x = 13;
        else if (ANIM_GLOBAL_FRAME(SORCERESS) == 2)
          ss_x = 12;
        else
          ss_x = 14;
      }

      ss_y = 11;

      if (t->facing == THING_FACING_RIGHT)
        ss_x += 8;
      else if (t->facing == THING_FACING_UP)
        ss_x += 4;
      else if (t->facing == THING_FACING_LEFT)
        ss_x += 8;
      else if (t->facing == THING_FACING_DOWN)
        ss_x += 0;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_IMP_ASLEEP)
    {
      if (t->facing == THING_FACING_RIGHT)
        ss_x = 10;
      else if (t->facing == THING_FACING_UP)
        ss_x = 9;
      else if (t->facing == THING_FACING_LEFT)
        ss_x = 10;
      else if (t->facing == THING_FACING_DOWN)
        ss_x = 8;

      ss_y = 17;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_IMP_AWAKE)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(IMP) == 0)
          ss_x = 5;
        else if (ANIM_GLOBAL_FRAME(IMP) == 1)
          ss_x = 6;
        else if (ANIM_GLOBAL_FRAME(IMP) == 2)
          ss_x = 5;
        else
          ss_x = 7;
      }
      else
        ss_x = 5;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_LIL_BOT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(LIL_BOT) == 0)
          ss_x = 16;
        else if (ANIM_GLOBAL_FRAME(LIL_BOT) == 1)
          ss_x = 17;
        else if (ANIM_GLOBAL_FRAME(LIL_BOT) == 2)
          ss_x = 16;
        else
          ss_x = 18;
      }
      else
        ss_x = 16;

      ss_y = 6;

      if (t->facing == THING_FACING_RIGHT)
        ss_x += 6;
      else if (t->facing == THING_FACING_UP)
        ss_x += 3;
      else if (t->facing == THING_FACING_LEFT)
        ss_x += 6;
      else if (t->facing == THING_FACING_DOWN)
        ss_x += 0;

      lighting = 0;
      palette = 0;
    }
    else if ( (t->type == THING_TYPE_FAERIE_HORIZONTAL) || 
              (t->type == THING_TYPE_FAERIE_VERTICAL))
    {
      if (ANIM_GLOBAL_FRAME(FAERIE) == 0)
        ss_x = 16;
      else if (ANIM_GLOBAL_FRAME(FAERIE) == 1)
        ss_x = 17;
      else if (ANIM_GLOBAL_FRAME(FAERIE) == 2)
        ss_x = 18;
      else
        ss_x = 17;

      ss_y = 7;

      if (t->facing == THING_FACING_RIGHT)
        ss_x += 6;
      else if (t->facing == THING_FACING_UP)
        ss_x += 3;
      else if (t->facing == THING_FACING_LEFT)
        ss_x += 6;
      else if (t->facing == THING_FACING_DOWN)
        ss_x += 0;

      lighting = 0;

      if (t->type == THING_TYPE_FAERIE_VERTICAL)
        palette = 1;
      else
        palette = 0;
    }
    else if (t->type == THING_TYPE_WISP_RT)
    {
      if (ANIM_GLOBAL_FRAME(WISP) == 0)
        ss_x = 12;
      else if (ANIM_GLOBAL_FRAME(WISP) == 1)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(WISP) == 2)
        ss_x = 14;
      else
        ss_x = 15;

      ss_y = 5;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_WISP_LT)
    {
      if (ANIM_GLOBAL_FRAME(WISP) == 0)
        ss_x = 12;
      else if (ANIM_GLOBAL_FRAME(WISP) == 1)
        ss_x = 13;
      else if (ANIM_GLOBAL_FRAME(WISP) == 2)
        ss_x = 14;
      else
        ss_x = 15;

      ss_y = 6;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_KNIGHT_RT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(KNIGHT) == 0)
          ss_x = 26;
        else if (ANIM_GLOBAL_FRAME(KNIGHT) == 1)
          ss_x = 27;
        else if (ANIM_GLOBAL_FRAME(KNIGHT) == 2)
          ss_x = 26;
        else
          ss_x = 28;
      }
      else
        ss_x = 26;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 3;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_KNIGHT_LT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(KNIGHT) == 0)
          ss_x = 26;
        else if (ANIM_GLOBAL_FRAME(KNIGHT) == 1)
          ss_x = 27;
        else if (ANIM_GLOBAL_FRAME(KNIGHT) == 2)
          ss_x = 26;
        else
          ss_x = 28;
      }
      else
        ss_x = 26;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 3;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 4;
    }
    else if (t->type == THING_TYPE_MUMMY_RT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(MUMMY_MOVING) == 0)
          ss_x = 20;
        else if (ANIM_GLOBAL_FRAME(MUMMY_MOVING) == 1)
          ss_x = 21;
        else if (ANIM_GLOBAL_FRAME(MUMMY_MOVING) == 2)
          ss_x = 20;
        else
          ss_x = 22;
      }
      else
        ss_x = 20;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 3;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_MUMMY_LT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(MUMMY_MOVING) == 0)
          ss_x = 20;
        else if (ANIM_GLOBAL_FRAME(MUMMY_MOVING) == 1)
          ss_x = 21;
        else if (ANIM_GLOBAL_FRAME(MUMMY_MOVING) == 2)
          ss_x = 20;
        else
          ss_x = 22;
      }
      else
        ss_x = 20;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 3;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 5;
    }
    else if ( (t->type == THING_TYPE_MUMMY_RT_COLLAPSING) || 
              (t->type == THING_TYPE_MUMMY_LT_COLLAPSING))
    {
      if (ANIM_CURRENT_FRAME(MUMMY_COLLAPSE) == 0)
        ss_x = 17;
      else if (ANIM_CURRENT_FRAME(MUMMY_COLLAPSE) == 1)
        ss_x = 18;
      else
        ss_x = 19;

      ss_y = 16;

      lighting = 0;

      if (t->type == THING_TYPE_MUMMY_LT_COLLAPSING)
        palette = 5;
      else
        palette = 0;
    }
    else if (t->type == THING_TYPE_JIANGSHI_RT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(JIANGSHI_MOVING) == 0)
          ss_x = 11;
        else if (ANIM_GLOBAL_FRAME(JIANGSHI_MOVING) == 1)
          ss_x = 12;
        else if (ANIM_GLOBAL_FRAME(JIANGSHI_MOVING) == 2)
          ss_x = 11;
        else
          ss_x = 13;
      }
      else
        ss_x = 11;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 3;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_JIANGSHI_LT)
    {
      if ((t->state == STATE_STARTED_MOVING)    || 
          (t->state == STATE_MOVING_ONE_SPACE)  || 
          (t->state == STATE_STOPPED_MOVING))
      {
        if (ANIM_GLOBAL_FRAME(JIANGSHI_MOVING) == 0)
          ss_x = 11;
        else if (ANIM_GLOBAL_FRAME(JIANGSHI_MOVING) == 1)
          ss_x = 12;
        else if (ANIM_GLOBAL_FRAME(JIANGSHI_MOVING) == 2)
          ss_x = 11;
        else
          ss_x = 13;
      }
      else
        ss_x = 11;

      ss_y = 16;

      if (t->facing == THING_FACING_RIGHT)
      {
        ss_x += 3;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_UP)
      {
        ss_x += 3;
        ss_y += 0;
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        ss_x += 0;
        ss_y += 1;
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        ss_x += 0;
        ss_y += 0;
      }

      lighting = 0;
      palette = 1;
    }
    else if ( (t->type == THING_TYPE_JIANGSHI_RT_STOPPED) || 
              (t->type == THING_TYPE_JIANGSHI_LT_STOPPED))
    {
      if (t->state == STATE_CHANGING)
      {
        if (ANIM_CURRENT_FRAME(JIANGSHI_STOP) == 0)
          ss_x = 18;
        else if (ANIM_CURRENT_FRAME(JIANGSHI_STOP) == 1)
          ss_x = 17;
        else
          ss_x = 19;
      }
      else
        ss_x = 17;

      ss_y = 17;

      lighting = 0;

      if (t->type == THING_TYPE_JIANGSHI_LT_STOPPED)
        palette = 1;
      else
        palette = 0;
    }
    /* ice cubes */
    else if (t->type == THING_TYPE_ICE_CUBE)
    {
      if (ANIM_ICE_FRAME(ICE_CUBE) == 0)
        ss_x = 26;
      else if (ANIM_ICE_FRAME(ICE_CUBE) == 1)
        ss_x = 27;
      else if (ANIM_ICE_FRAME(ICE_CUBE) == 2)
        ss_x = 28;
      else
        ss_x = 25;

      ss_y = 13;

      lighting = 0;
      palette = 0;
    }
    else if (t->type == THING_TYPE_ICE_CUBE_THAWING)
    {
      ss_x = 25;
      ss_y = 13;

      if (t->state == STATE_AWAITING_SNAP_AND_THAW)
        lighting = 2;
      else
      {
        if (ANIM_ICE_FRAME(ICE_CUBE) == 0)
          lighting = 0;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 1)
          lighting = 0;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 2)
          lighting = 0;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 3)
          lighting = 0;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 4)
          lighting = 0;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 5)
          lighting = 1;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 6)
          lighting = 2;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 7)
          lighting = 3;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 8)
          lighting = 2;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 9)
          lighting = 1;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 10)
          lighting = 2;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 11)
          lighting = 3;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 12)
          lighting = 2;
        else if (ANIM_ICE_FRAME(ICE_CUBE) == 13)
          lighting = 1;
        else
          lighting = 2;
      }

      palette = 0;
    }
    /* boxes, pots, flasks */
    else if ( (t->type == THING_TYPE_BOX_WOOD)  || 
              (t->type == THING_TYPE_BOX_METAL) || 
              (t->type == THING_TYPE_BOX_STONE))
    {
      ss_x = 20;
      ss_y = 13;

      lighting = 0;

      if (t->type == THING_TYPE_BOX_WOOD)
        palette = 0;
      else if (t->type == THING_TYPE_BOX_METAL)
        palette = 3;
      else if (t->type == THING_TYPE_BOX_STONE)
        palette = 2;
      else
        palette = 0;
    }
    else if ( (t->type == THING_TYPE_POT_WOOD)  || 
              (t->type == THING_TYPE_POT_METAL) || 
              (t->type == THING_TYPE_POT_STONE))
    {
      ss_x = 21;
      ss_y = 13;

      lighting = 0;

      if (t->type == THING_TYPE_POT_WOOD)
        palette = 0;
      else if (t->type == THING_TYPE_POT_METAL)
        palette = 3;
      else if (t->type == THING_TYPE_POT_STONE)
        palette = 2;
      else
        palette = 0;
    }
    else if ( (t->type == THING_TYPE_ACID_FLASK) || 
              (t->type == THING_TYPE_STONE_FLASK))
    {
      if (ANIM_GLOBAL_FRAME(FLASK) == 0)
        ss_x = 22;
      else if (ANIM_GLOBAL_FRAME(FLASK) == 1)
        ss_x = 23;
      else if (ANIM_GLOBAL_FRAME(FLASK) == 2)
        ss_x = 24;
      else
        ss_x = 23;

      ss_y = 13;

      lighting = 0;

      if (t->type == THING_TYPE_ACID_FLASK)
        palette = 0;
      else if (t->type == THING_TYPE_STONE_FLASK)
        palette = 4;
      else
        palette = 0;
    }
    /* puff */
    else if ( (t->type == THING_TYPE_PUFF_GREY)   || 
              (t->type == THING_TYPE_PUFF_ORANGE) || 
              (t->type == THING_TYPE_PUFF_CYAN)   || 
              (t->type == THING_TYPE_PUFF_PURPLE) || 
              (t->type == THING_TYPE_PUFF_GREEN))
    {
      ss_x = 28;

      if (ANIM_CURRENT_FRAME(PUFF) == 0)
        ss_y = 10;
      else if (ANIM_CURRENT_FRAME(PUFF) == 1)
        ss_y = 11;
      else
        ss_y = 12;

      lighting = 0;

      if (t->type == THING_TYPE_PUFF_GREY)
        palette = 6;
      else if (t->type == THING_TYPE_PUFF_ORANGE)
        palette = 0;
      else if (t->type == THING_TYPE_PUFF_CYAN)
        palette = 3;
      else if (t->type == THING_TYPE_PUFF_PURPLE)
        palette = 1;
      else if (t->type == THING_TYPE_PUFF_GREEN)
        palette = 4;
      else
        palette = 0;
    }
    else
      continue;

    /* compute z level */
    z = GRAPHICS_Z_LEVEL_THINGS - (GRAPHICS_Z_LEVEL_SUBDIVISION_STEP * k);

    /* add this sprite to the buffers */
    if ((t->type == THING_TYPE_FIRE_SPELL) || 
        (t->type == THING_TYPE_ICE_SPELL)  || 
        (t->type == THING_TYPE_RING_SPELL))
    {
      if (t->facing == THING_FACING_UP)
      {
        VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, ROTATE_90)
      }
      else if (t->facing == THING_FACING_LEFT)
      {
        VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, FLIP_HORI)
      }
      else if (t->facing == THING_FACING_DOWN)
      {
        VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, FLIP_AND_ROTATE_90)
      }
      else
      {
        VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, NORMAL)
      }
    }
    else if ( (t->type == THING_TYPE_VAMPIRE)                 || 
              (t->type == THING_TYPE_BAT)                     || 
              (t->type == THING_TYPE_SNAKE_ASLEEP)            || 
              (t->type == THING_TYPE_SNAKE_AWAKE)             || 
              (t->type == THING_TYPE_SHADE)                   || 
              (t->type == THING_TYPE_WITCH_ASLEEP)            || 
              (t->type == THING_TYPE_WITCH_AWAKE)             || 
              (t->type == THING_TYPE_SORCERESS)               || 
              (t->type == THING_TYPE_IMP_ASLEEP)              || 
              (t->type == THING_TYPE_IMP_AWAKE)               || 
              (t->type == THING_TYPE_LIL_BOT)                 || 
              (t->type == THING_TYPE_FAERIE_HORIZONTAL)       || 
              (t->type == THING_TYPE_FAERIE_VERTICAL)         || 
              (t->type == THING_TYPE_DOUBLE_NORMAL_VAMPIRE)   || 
              (t->type == THING_TYPE_DOUBLE_NORMAL_BAT)       || 
              (t->type == THING_TYPE_DOUBLE_REVERSE_VAMPIRE)  || 
              (t->type == THING_TYPE_DOUBLE_REVERSE_BAT))
    {
      if (t->facing == THING_FACING_LEFT)
      {
        VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, FLIP_HORI)
      }
      else
      {
        VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, NORMAL)
      }
    }
    else if ( (t->type == THING_TYPE_KNIGHT_LT)           || 
              (t->type == THING_TYPE_MUMMY_LT)            || 
              (t->type == THING_TYPE_MUMMY_LT_COLLAPSING) || 
              (t->type == THING_TYPE_JIANGSHI_LT)         || 
              (t->type == THING_TYPE_JIANGSHI_LT_STOPPED))
    {
      VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, FLIP_HORI)
    }
    else
    {
      VB_SPRITE_ADD_THING_TO_BUFFERS(pos_x, pos_y, z, ss_x, ss_y, lighting, palette, NORMAL)
    }
  }

/* update grid objects & things sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] = 
    sprite_index - GRAPHICS_GRID_OBJECTS_AND_THINGS_SPRITES_START_INDEX;

  /* update vbos */
  VB_SPRITE_UPDATE_GRID_OBJECTS_AND_THINGS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_sprite_clear_grid_objects_and_things()
*******************************************************************************/
short int vb_sprite_clear_grid_objects_and_things()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_GRID_OBJECTS_AND_THINGS] = 0;

  return 0;
}
