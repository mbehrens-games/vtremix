/*******************************************************************************
** vbmenu.c (vbo population functions - menus)
*******************************************************************************/

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "animate.h"
#include "controls.h"
#include "doremi.h"
#include "global.h"
#include "graphics.h"
#include "level.h"
#include "menu.h"
#include "palette.h"
#include "savegame.h"
#include "screen.h"
#include "story.h"
#include "texture.h"
#include "vbmenu.h"
#include "video.h"

enum
{
  VB_MENU_ALIGN_LEFT = 0,
  VB_MENU_ALIGN_CENTER,
  VB_MENU_ALIGN_RIGHT
};

#define VB_MENU_COMPUTE_PERCENT_DIGIT(index, percent, divisor)                 \
  if ((percent % (divisor * 10)) / divisor == 0)                               \
    buffer[index] = '0';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 1)                          \
    buffer[index] = '1';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 2)                          \
    buffer[index] = '2';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 3)                          \
    buffer[index] = '3';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 4)                          \
    buffer[index] = '4';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 5)                          \
    buffer[index] = '5';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 6)                          \
    buffer[index] = '6';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 7)                          \
    buffer[index] = '7';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 8)                          \
    buffer[index] = '8';                                                       \
  else if ((percent % (divisor * 10)) / divisor == 9)                          \
    buffer[index] = '9';                                                       \
  else                                                                         \
    buffer[index] = '0';

/* position is the center, width & height are in 8x8 cells */
#define VB_MENU_ADD_TO_VERTEX_BUFFER(pos_x, pos_y, width, height, z)           \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x - (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x + (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x - (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x + (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

#define VB_MENU_ADD_TO_VERTEX_BUFFER_FLIP_HORI(pos_x, pos_y, width, height, z) \
  G_vertex_buffer_sprites[12 * sprite_index + 0]   = pos_x + (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 1]   = pos_y - (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 2]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 3]   = pos_x - (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 4]   = pos_y - (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 5]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 6]   = pos_x + (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 7]   = pos_y + (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 8]   = z;                        \
                                                                               \
  G_vertex_buffer_sprites[12 * sprite_index + 9]   = pos_x - (4 * width);      \
  G_vertex_buffer_sprites[12 * sprite_index + 10]  = pos_y + (4 * height);     \
  G_vertex_buffer_sprites[12 * sprite_index + 11]  = z;

/* cell_x and cell_y are the top left corner, width & height are in 8x8 cells */
#define VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, width, height)                                \
  G_texture_coord_buffer_sprites[8 * sprite_index + 0] = G_texture_coord_table[cell_x];                   \
  G_texture_coord_buffer_sprites[8 * sprite_index + 1] = G_texture_coord_table[64 - cell_y];              \
                                                                                                          \
  G_texture_coord_buffer_sprites[8 * sprite_index + 2] = G_texture_coord_table[cell_x + width];           \
  G_texture_coord_buffer_sprites[8 * sprite_index + 3] = G_texture_coord_table[64 - cell_y];              \
                                                                                                          \
  G_texture_coord_buffer_sprites[8 * sprite_index + 4] = G_texture_coord_table[cell_x];                   \
  G_texture_coord_buffer_sprites[8 * sprite_index + 5] = G_texture_coord_table[64 - (cell_y + height)];   \
                                                                                                          \
  G_texture_coord_buffer_sprites[8 * sprite_index + 6] = G_texture_coord_table[cell_x + width];           \
  G_texture_coord_buffer_sprites[8 * sprite_index + 7] = G_texture_coord_table[64 - (cell_y + height)];

#define VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                   \
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

#define VB_MENU_ADD_TO_ELEMENT_BUFFER()                                        \
  G_index_buffer_sprites[6 * sprite_index + 0] = 4 * sprite_index + 0;         \
  G_index_buffer_sprites[6 * sprite_index + 1] = 4 * sprite_index + 2;         \
  G_index_buffer_sprites[6 * sprite_index + 2] = 4 * sprite_index + 1;         \
                                                                               \
  G_index_buffer_sprites[6 * sprite_index + 3] = 4 * sprite_index + 1;         \
  G_index_buffer_sprites[6 * sprite_index + 4] = 4 * sprite_index + 2;         \
  G_index_buffer_sprites[6 * sprite_index + 5] = 4 * sprite_index + 3;

#define VB_MENU_ADD_PANEL_PIECE_TO_BUFFERS(pos_x, pos_y, z, cell_x, cell_y, lighting, palette)  \
  if (sprite_index < GRAPHICS_PANELS_SPRITES_END_INDEX)                                         \
  {                                                                                             \
    VB_MENU_ADD_TO_VERTEX_BUFFER(pos_x, pos_y, 2, 2, z)                                         \
    VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, 2, 2)                                   \
    VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                               \
    VB_MENU_ADD_TO_ELEMENT_BUFFER()                                                             \
                                                                                                \
    sprite_index += 1;                                                                          \
  }

#define VB_MENU_ADD_FONT_CHARACTER_TO_BUFFERS(pos_x, pos_y, z, cell_x, cell_y, lighting, palette) \
  if (sprite_index < GRAPHICS_OVERLAY_SPRITES_END_INDEX)                                          \
  {                                                                                               \
    VB_MENU_ADD_TO_VERTEX_BUFFER(pos_x, pos_y, 1, 2, z)                                           \
    VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, 1, 2)                                     \
    VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                 \
    VB_MENU_ADD_TO_ELEMENT_BUFFER()                                                               \
                                                                                                  \
    sprite_index += 1;                                                                            \
  }

#define VB_MENU_ADD_HUD_SPRITE_TO_BUFFERS(pos_x, pos_y, z, cell_x, cell_y, lighting, palette)   \
  if (sprite_index < GRAPHICS_OVERLAY_SPRITES_END_INDEX)                                        \
  {                                                                                             \
    VB_MENU_ADD_TO_VERTEX_BUFFER(pos_x, pos_y, 2, 2, z)                                         \
    VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, 2, 2)                                   \
    VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                               \
    VB_MENU_ADD_TO_ELEMENT_BUFFER()                                                             \
                                                                                                \
    sprite_index += 1;                                                                          \
  }

#define VB_MENU_ADD_TITLE_LOGO_TO_BUFFERS(pos_x, pos_y, z, cell_x, cell_y, lighting, palette)   \
  if (sprite_index < GRAPHICS_OVERLAY_SPRITES_END_INDEX)                                        \
  {                                                                                             \
    VB_MENU_ADD_TO_VERTEX_BUFFER(pos_x, pos_y, 23, 6, z)                                        \
    VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, 23, 6)                                  \
    VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                               \
    VB_MENU_ADD_TO_ELEMENT_BUFFER()                                                             \
                                                                                                \
    sprite_index += 1;                                                                          \
  }

#define VB_MENU_ADD_PAGE_ARROW_RIGHT_TO_BUFFERS(pos_x, pos_y, z, cell_x, cell_y, lighting, palette) \
  if (sprite_index < GRAPHICS_OVERLAY_SPRITES_END_INDEX)                                            \
  {                                                                                                 \
    VB_MENU_ADD_TO_VERTEX_BUFFER(pos_x, pos_y, 3, 3, z)                                             \
    VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, 3, 3)                                       \
    VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                   \
    VB_MENU_ADD_TO_ELEMENT_BUFFER()                                                                 \
                                                                                                    \
    sprite_index += 1;                                                                              \
  }

#define VB_MENU_ADD_PAGE_ARROW_LEFT_TO_BUFFERS(pos_x, pos_y, z, cell_x, cell_y, lighting, palette)  \
  if (sprite_index < GRAPHICS_OVERLAY_SPRITES_END_INDEX)                                            \
  {                                                                                                 \
    VB_MENU_ADD_TO_VERTEX_BUFFER_FLIP_HORI(pos_x, pos_y, 3, 3, z)                                   \
    VB_MENU_ADD_TO_TEXTURE_COORD_BUFFER(cell_x, cell_y, 3, 3)                                       \
    VB_MENU_ADD_TO_LIGHTING_AND_PALETTE_BUFFER(lighting, palette)                                   \
    VB_MENU_ADD_TO_ELEMENT_BUFFER()                                                                 \
                                                                                                    \
    sprite_index += 1;                                                                              \
  }

#define VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()                                                     \
  glBindBuffer(GL_ARRAY_BUFFER, G_vertex_buffer_id_sprites);                                        \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                  \
                  GRAPHICS_PANELS_SPRITES_START_INDEX * 12 * sizeof(GLfloat),                       \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] * 12 * sizeof(GLfloat),       \
                  &G_vertex_buffer_sprites[GRAPHICS_PANELS_SPRITES_START_INDEX * 12]);              \
                                                                                                    \
  glBindBuffer(GL_ARRAY_BUFFER, G_texture_coord_buffer_id_sprites);                                 \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                  \
                  GRAPHICS_PANELS_SPRITES_START_INDEX * 8 * sizeof(GLfloat),                        \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] * 8 * sizeof(GLfloat),        \
                  &G_texture_coord_buffer_sprites[GRAPHICS_PANELS_SPRITES_START_INDEX * 8]);        \
                                                                                                    \
  glBindBuffer(GL_ARRAY_BUFFER, G_lighting_and_palette_buffer_id_sprites);                          \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                  \
                  GRAPHICS_PANELS_SPRITES_START_INDEX * 8 * sizeof(GLfloat),                        \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] * 8 * sizeof(GLfloat),        \
                  &G_lighting_and_palette_buffer_sprites[GRAPHICS_PANELS_SPRITES_START_INDEX * 8]); \
                                                                                                    \
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G_index_buffer_id_sprites);                                 \
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,                                                          \
                  GRAPHICS_PANELS_SPRITES_START_INDEX * 6 * sizeof(unsigned short),                 \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] * 6 * sizeof(unsigned short), \
                  &G_index_buffer_sprites[GRAPHICS_PANELS_SPRITES_START_INDEX * 6]);

#define VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()                                                      \
  glBindBuffer(GL_ARRAY_BUFFER, G_vertex_buffer_id_sprites);                                          \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                    \
                  GRAPHICS_OVERLAY_SPRITES_START_INDEX * 12 * sizeof(GLfloat),                        \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] * 12 * sizeof(GLfloat),        \
                  &G_vertex_buffer_sprites[GRAPHICS_OVERLAY_SPRITES_START_INDEX * 12]);               \
                                                                                                      \
  glBindBuffer(GL_ARRAY_BUFFER, G_texture_coord_buffer_id_sprites);                                   \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                    \
                  GRAPHICS_OVERLAY_SPRITES_START_INDEX * 8 * sizeof(GLfloat),                         \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] * 8 * sizeof(GLfloat),         \
                  &G_texture_coord_buffer_sprites[GRAPHICS_OVERLAY_SPRITES_START_INDEX * 8]);         \
                                                                                                      \
  glBindBuffer(GL_ARRAY_BUFFER, G_lighting_and_palette_buffer_id_sprites);                            \
  glBufferSubData(GL_ARRAY_BUFFER,                                                                    \
                  GRAPHICS_OVERLAY_SPRITES_START_INDEX * 8 * sizeof(GLfloat),                         \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] * 8 * sizeof(GLfloat),         \
                  &G_lighting_and_palette_buffer_sprites[GRAPHICS_OVERLAY_SPRITES_START_INDEX * 8]);  \
                                                                                                      \
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G_index_buffer_id_sprites);                                   \
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,                                                            \
                  GRAPHICS_OVERLAY_SPRITES_START_INDEX * 6 * sizeof(unsigned short),                  \
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] * 6 * sizeof(unsigned short),  \
                  &G_index_buffer_sprites[GRAPHICS_OVERLAY_SPRITES_START_INDEX * 6]);

/*******************************************************************************
** vb_menu_load_panel()
*******************************************************************************/
short int vb_menu_load_panel(int offset_x, int offset_y, int width, int height)
{
  int m;
  int n;

  int sprite_index;

  int corner_x;
  int corner_y;

  int pos_x;
  int pos_y;

  int cell_x;
  int cell_y;

  int lighting;
  int palette;

  /* make sure the width and height are valid       */
  /* the width & height are in terms of 16x16 tiles */
  if ((width < 2) || (width > GRAPHICS_OVERSCAN_WIDTH / 16))
    return 1;

  if ((height < 2) || (height > GRAPHICS_OVERSCAN_HEIGHT / 16))
    return 1;

  /* determine coordinates of top left corner */
  corner_x = (GRAPHICS_OVERSCAN_WIDTH - 16 * width) / 2;
  corner_y = (GRAPHICS_OVERSCAN_HEIGHT - 16 * height) / 2;

  /* the offsets from the screen center are in 4x4 half-cells */
  corner_x += 4 * offset_x;
  corner_y += 4 * offset_y;

  /* make sure the panel is on the screen */
  if (corner_x + (16 * width) < 0)
    return 1;

  if (corner_x > GRAPHICS_OVERSCAN_WIDTH)
    return 1;

  if (corner_y + (16 * height) < 0)
    return 1;

  if (corner_y > GRAPHICS_OVERSCAN_HEIGHT)
    return 1;

  /* set lighting and palette */
  lighting = 0;
  palette = 6;

  /* draw the panel */
  sprite_index =  GRAPHICS_PANELS_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS];

  for (n = 0; n < height; n++)
  {
    for (m = 0; m < width; m++)
    {
      /* determine center of this piece */
      pos_x = corner_x + (16 * m) + 8;
      pos_y = corner_y + (16 * n) + 8;

      /* top left corner piece */
      if ((m == 0) && (n == 0))
      {
        cell_x = 58;
        cell_y = 6;
      }
      /* bottom left corner piece */
      else if ((m == 0) && (n == height - 1))
      {
        cell_x = 58;
        cell_y = 10;
      }
      /* top right corner piece */
      else if ((m == width - 1) && (n == 0))
      {
        cell_x = 62;
        cell_y = 6;
      }
      /* bottom right corner piece */
      else if ((m == width - 1) && (n == height - 1))
      {
        cell_x = 62;
        cell_y = 10;
      }
      /* top piece */
      else if (n == 0)
      {
        cell_x = 60;
        cell_y = 6;
      }
      /* bottom piece */
      else if (n == height - 1)
      {
        cell_x = 60;
        cell_y = 10;
      }
      /* left piece */
      else if (m == 0)
      {
        cell_x = 58;
        cell_y = 8;
      }
      /* right piece */
      else if (m == width - 1)
      {
        cell_x = 62;
        cell_y = 8;
      }
      /* middle piece */
      else
      {
        cell_x = 60;
        cell_y = 8;
      }

      VB_MENU_ADD_PANEL_PIECE_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_PANELS, 
                                          cell_x, cell_y, lighting, palette)
    }
  }

  /* update panels sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 
    sprite_index - GRAPHICS_PANELS_SPRITES_START_INDEX;

  return 0;
}

/*******************************************************************************
** vb_menu_load_text()
*******************************************************************************/
short int vb_menu_load_text(int offset_x, int offset_y, int align, 
                            int lighting, int palette, int max, char* str)
{
  int k;

  int length;

  int sprite_index;

  int start_x;
  int start_y;

  int pos_x;
  int pos_y;

  int cell_x;
  int cell_y;

  if (str == NULL)
    return 1;

  /* bound max length of text string */
  if (max < 1)
    max = 1;
  else if (max > 32)
    max = 32;

  /* determine length of text string */
  length = 0;

  while ((length < max) && (str[length] != '\0'))
    length++;

  if (length == 0)
    return 0;

  /* determine coordinates of center of first character */
  /* note that each character is 8x16                   */
  start_x = (GRAPHICS_OVERSCAN_WIDTH / 2);
  start_y = (GRAPHICS_OVERSCAN_HEIGHT / 2);

  /* left:    the center of the first character is at the screen center */
  /* center:  the center of the whole string is at the screen center    */
  /* right:   the center of the last character is at the screen center  */
  if (align == VB_MENU_ALIGN_LEFT)
    start_x += 0;
  else if (align == VB_MENU_ALIGN_CENTER)
    start_x -= 4 * (length - 1);
  else if (align == VB_MENU_ALIGN_RIGHT)
    start_x -= 8 * (length - 1);

  /* the offsets from the screen center are in 4x4 half-cells */
  start_x += 4 * offset_x;
  start_y += 4 * offset_y;

  /* make sure the text string is on the screen */
  if (start_x + (8 * (length - 1)) + 4 < 0)
    return 1;

  if (start_x - 4 > GRAPHICS_OVERSCAN_WIDTH)
    return 1;

  if (start_y + 8 < 0)
    return 1;

  if (start_y - 8 > GRAPHICS_OVERSCAN_HEIGHT)
    return 1;

  /* bound lighting and palette */
  if ((lighting < -4) || (lighting > 3))
    lighting = 0;

  if ((palette < 0) || (palette > 7))
    palette = 0;

  /* draw the string */
  sprite_index =  GRAPHICS_OVERLAY_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY];

  for (k = 0; k < length; k++)
  {
    /* determine center of this character */
    pos_x = start_x + (8 * k);
    pos_y = start_y;

    /* determine texture coordinates for this character */
    if ((str[k] > 32) && (str[k] < 64))
    {
      cell_x = str[k] - 32 + 32;
      cell_y = 0;
    }
    else if ((str[k] >= 64) && (str[k] < 96))
    {
      cell_x = str[k] - 64 + 32;
      cell_y = 2;
    }
    else if ((str[k] >= 96) && (str[k] < 127))
    {
      cell_x = str[k] - 96 + 32;
      cell_y = 4;
    }
    else
      continue;

    VB_MENU_ADD_FONT_CHARACTER_TO_BUFFERS(pos_x, pos_y, GRAPHICS_Z_LEVEL_OVERLAY, 
                                          cell_x, cell_y, lighting, palette)
  }

  /* update overlay sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 
    sprite_index - GRAPHICS_OVERLAY_SPRITES_START_INDEX;

  return 0;
}

/*******************************************************************************
** vb_menu_load_percent()
*******************************************************************************/
short int vb_menu_load_percent( int offset_x, int offset_y, int align, 
                                int lighting, int palette, int percent)
{
  int   i;
  char  buffer[6];

  /* make sure percent is valid */
  if ((percent < 0) || (percent > 1000))
    return 0;

  /* initialize string buffer */
  for (i = 0; i < 6; i++)
    buffer[i] = '\0';

  /* construct string */
  if (percent == 0)
    strcpy(buffer, "0%");
  else if (percent == 1000)
    strcpy(buffer, "100%");
  /* xx.x% */
  else if (percent > 100)
  {
    VB_MENU_COMPUTE_PERCENT_DIGIT(0, percent, 100)
    VB_MENU_COMPUTE_PERCENT_DIGIT(1, percent, 10)

    buffer[2] = '.';

    VB_MENU_COMPUTE_PERCENT_DIGIT(3, percent, 1)

    buffer[4] = '%';
    buffer[5] = '\0';
  }
  /* x.x% */
  else
  {
    VB_MENU_COMPUTE_PERCENT_DIGIT(0, percent, 10)

    buffer[1] = '.';

    VB_MENU_COMPUTE_PERCENT_DIGIT(2, percent, 1)

    buffer[3] = '%';
    buffer[4] = '\0';
  }

  vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 6, buffer);

  return 0;
}

/*******************************************************************************
** vb_menu_load_cursor()
*******************************************************************************/
short int vb_menu_load_cursor(int offset_x, int offset_y, 
                              int lighting, int palette)
{
  int sprite_index;

  int start_x;
  int start_y;

  int cell_x;
  int cell_y;

  unsigned int adjusted_timer_count;

  /* determine animation frame for cursor blink */
  adjusted_timer_count = G_timer_count;

  if (ANIM_CURRENT_FRAME(MENU_CURSOR_BLINK) == 1)
    return 0;

  /* determine coordinates of center of first character */
  /* note that each character is 8x16                   */
  start_x = (GRAPHICS_OVERSCAN_WIDTH / 2);
  start_y = (GRAPHICS_OVERSCAN_HEIGHT / 2);

  /* the offsets from the screen center are in 4x4 half-cells */
  start_x += 4 * offset_x;
  start_y += 4 * offset_y;

  /* make sure the text cursor is on the screen */
  if (start_x + 4 < 0)
    return 1;

  if (start_x - 4 > GRAPHICS_OVERSCAN_WIDTH)
    return 1;

  if (start_y + 8 < 0)
    return 1;

  if (start_y - 8 > GRAPHICS_OVERSCAN_HEIGHT)
    return 1;

  /* bound lighting and palette */
  if ((lighting < -4) || (lighting > 3))
    lighting = 0;

  if ((palette < 0) || (palette > 7))
    palette = 0;

  /* set texture coordinates for cursor */
  cell_x = 63;
  cell_y = 4;

  /* draw the cursor */
  sprite_index =  GRAPHICS_OVERLAY_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY];

  VB_MENU_ADD_FONT_CHARACTER_TO_BUFFERS(start_x, start_y, GRAPHICS_Z_LEVEL_OVERLAY, 
                                        cell_x, cell_y, lighting, palette)

  /* update overlay sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 
    sprite_index - GRAPHICS_OVERLAY_SPRITES_START_INDEX;

  return 0;
}

/*******************************************************************************
** vb_menu_load_slider()
*******************************************************************************/
short int vb_menu_load_slider(int offset_x, int offset_y, int align, 
                              int lighting, int palette, int value)
{
  /* make sure value is in range */
  if ((value < 0) || (value > 8))
    return 0;

  /* draw the slider */
  if (value == 0)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, "l........");
  else if (value == 1)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, ".l.......");
  else if (value == 2)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, "..l......");
  else if (value == 3)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, "...l.....");
  else if (value == 4)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, "....l....");
  else if (value == 5)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, ".....l...");
  else if (value == 6)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, "......l..");
  else if (value == 7)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, ".......l.");
  else if (value == 8)
    vb_menu_load_text(offset_x, offset_y, align, lighting, palette, 16, "........l");

  return 0;
}

/*******************************************************************************
** vb_menu_load_title_logo()
*******************************************************************************/
short int vb_menu_load_title_logo(int offset_x, int offset_y)
{
  int sprite_index;

  int center_x;
  int center_y;

  int cell_x;
  int cell_y;

  int lighting;
  int palette;

  /* determine coordinates of center */
  center_x = GRAPHICS_OVERSCAN_WIDTH / 2;
  center_y = GRAPHICS_OVERSCAN_HEIGHT / 2;

  /* the offsets from the screen center are in 4x4 half-cells */
  center_x += 4 * offset_x;
  center_y += 4 * offset_y;

  /* make sure the logo is on the screen */
  if (center_x + 92 < 0)
    return 1;

  if (center_x - 92 > GRAPHICS_OVERSCAN_WIDTH)
    return 1;

  if (center_y + 24 < 0)
    return 1;

  if (center_y - 24 > GRAPHICS_OVERSCAN_HEIGHT)
    return 1;

  /* set texture coordinates */
  cell_x = 32;
  cell_y = 6;

  /* set lighting and palette */
  lighting = 0;
  palette = 0;

  /* draw the logo */
  sprite_index =  GRAPHICS_OVERLAY_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY];

  VB_MENU_ADD_TITLE_LOGO_TO_BUFFERS(center_x, center_y, GRAPHICS_Z_LEVEL_OVERLAY, 
                                    cell_x, cell_y, lighting, palette)

  /* update overlay sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 
    sprite_index - GRAPHICS_OVERLAY_SPRITES_START_INDEX;

  return 0;
}

/*******************************************************************************
** vb_menu_load_page_arrow_right()
*******************************************************************************/
short int vb_menu_load_page_arrow_right()
{
  int sprite_index;

  int pos_x;
  int pos_y;

  int cell_x;
  int cell_y;

  int lighting;
  int palette;

  unsigned int adjusted_timer_count;

  /* set position */
  pos_x = GRAPHICS_OVERSCAN_WIDTH / 2;
  pos_y = GRAPHICS_OVERSCAN_HEIGHT / 2;

  pos_x += 140;
  pos_y += 16;

  /* set texture coordinates */
  adjusted_timer_count = G_timer_count;

  if (ANIM_CURRENT_FRAME(MENU_PAGE_ARROW) == 0)
  {
    cell_x = 55;
    cell_y = 6;
  }
  else
  {
    cell_x = 55;
    cell_y = 9;
  }

  /* set lighting and palette */
  lighting = 0;
  palette = 0;

  /* draw the arrow */
  sprite_index =  GRAPHICS_OVERLAY_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY];

  VB_MENU_ADD_PAGE_ARROW_RIGHT_TO_BUFFERS(pos_x, pos_y, GRAPHICS_Z_LEVEL_OVERLAY,
                                          cell_x, cell_y, lighting, palette)

  /* update overlay sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 
    sprite_index - GRAPHICS_OVERLAY_SPRITES_START_INDEX;

  return 0;
}

/*******************************************************************************
** vb_menu_load_page_arrow_left()
*******************************************************************************/
short int vb_menu_load_page_arrow_left()
{
  int sprite_index;

  int pos_x;
  int pos_y;

  int cell_x;
  int cell_y;

  int lighting;
  int palette;

  unsigned int adjusted_timer_count;

  /* set position */
  pos_x = GRAPHICS_OVERSCAN_WIDTH / 2;
  pos_y = GRAPHICS_OVERSCAN_HEIGHT / 2;

  pos_x -= 140;
  pos_y += 16;

  /* set texture coordinates */
  adjusted_timer_count = G_timer_count;

  if (ANIM_CURRENT_FRAME(MENU_PAGE_ARROW) == 0)
  {
    cell_x = 55;
    cell_y = 6;
  }
  else
  {
    cell_x = 55;
    cell_y = 9;
  }

  /* set lighting and palette */
  lighting = 0;
  palette = 0;

  /* draw the arrow */
  sprite_index =  GRAPHICS_OVERLAY_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY];

  VB_MENU_ADD_PAGE_ARROW_LEFT_TO_BUFFERS( pos_x, pos_y, GRAPHICS_Z_LEVEL_OVERLAY,
                                          cell_x, cell_y, lighting, palette)

  /* update overlay sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 
    sprite_index - GRAPHICS_OVERLAY_SPRITES_START_INDEX;

  return 0;
}

/*******************************************************************************
** vb_menu_load_title_panels()
*******************************************************************************/
short int vb_menu_load_title_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* menu panel */
  vb_menu_load_panel(0, 3, 9, 6);

  /* signature panel */
  vb_menu_load_panel(0, 22, 14, 2);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_title_overlay()
*******************************************************************************/
short int vb_menu_load_title_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* title logo */
  vb_menu_load_title_logo(0, -20);

  /* menu text */
  vb_menu_load_text(-13, -5, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Play the Game!");
  vb_menu_load_text(-13, -1, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Options");
  vb_menu_load_text(-13,  3, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Help");
  vb_menu_load_text(-13,  7, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Credits");
  vb_menu_load_text(-13, 11, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Quit");

  /* signature text */
  vb_menu_load_text(0, 22, VB_MENU_ALIGN_CENTER, 0, 6, 32, "2022 Michael Behrens v1.0c");

  /* cursor */
  vb_menu_load_cursor(-15, -5 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_save_game_select_panels()
*******************************************************************************/
short int vb_menu_load_save_game_select_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name panel */
  vb_menu_load_panel(0, -22, 10, 2);

  /* save game panel */
  if (G_save_game_percent != 0)
    vb_menu_load_panel(0, 4, 10, 8);
  else
    vb_menu_load_panel(0, 4, 9, 5);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_save_game_select_overlay()
*******************************************************************************/
short int vb_menu_load_save_game_select_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 18, "Select Save Game!");

  /* save game text */
  if (G_save_game_percent != 0)
  {
    if (G_screen_page == 0)
      vb_menu_load_text(0, -8, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 1");
    else if (G_screen_page == 1)
      vb_menu_load_text(0, -8, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 2");
    else if (G_screen_page == 2)
      vb_menu_load_text(0, -8, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 3");

    vb_menu_load_text(0, -2, VB_MENU_ALIGN_CENTER, 0, 1, 16, "Rooms Complete");
    vb_menu_load_percent(0, 2, VB_MENU_ALIGN_CENTER, 0, 0, G_save_game_percent);

    vb_menu_load_text(-15, 8, VB_MENU_ALIGN_LEFT, 0, 6, 16, "World Select");
    vb_menu_load_text(-15, 12, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Story Scenes");
    vb_menu_load_text(-15, 16, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Clear Save Game");
  }
  else
  {
    if (G_screen_page == 0)
      vb_menu_load_text(0, -2, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 1");
    else if (G_screen_page == 1)
      vb_menu_load_text(0, -2, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 2");
    else if (G_screen_page == 2)
      vb_menu_load_text(0, -2, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 3");

    vb_menu_load_text(0, 4, VB_MENU_ALIGN_CENTER, 0, 1, 16, "* New Game *");

    vb_menu_load_text(-13, 10, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Start the Game!");
  }

  /* cursor */
  if (G_save_game_percent != 0)
    vb_menu_load_cursor(-17, 8 + (4 * G_screen_choice), 0, 1);
  else
    vb_menu_load_cursor(-15, 10, 0, 1);

  /* page arrows */
  if (G_screen_page > 0)
    vb_menu_load_page_arrow_left();

  if (G_screen_page < SCREEN_SAVE_GAME_SELECT_NUM_PAGES - 1)
    vb_menu_load_page_arrow_right();

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_confirm_clear_save_game_panels()
*******************************************************************************/
short int vb_menu_load_confirm_clear_save_game_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name panel */
  vb_menu_load_panel(0, -22, 10, 2);

  /* clear save game panel */
  vb_menu_load_panel(0, 4, 9, 6);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_confirm_clear_save_game_overlay()
*******************************************************************************/
short int vb_menu_load_confirm_clear_save_game_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 18, "Select Save Game!");

  /* clear save game text */
  if (G_save_game_current_slot == 1)
    vb_menu_load_text(0, -4, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 1");
  else if (G_save_game_current_slot == 2)
    vb_menu_load_text(0, -4, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 2");
  else if (G_save_game_current_slot == 3)
    vb_menu_load_text(0, -4, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Save Game 3");

  vb_menu_load_text(0, 2, VB_MENU_ALIGN_CENTER, 0, 1, 24, "Clear Save Game?");

  vb_menu_load_text(-2,  8, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Yes");
  vb_menu_load_text(-2, 12, VB_MENU_ALIGN_LEFT, 0, 6, 16, "No");

  /* cursor */
  vb_menu_load_cursor(-4, 8 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_world_select_panels()
*******************************************************************************/
short int vb_menu_load_world_select_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name panel */
  vb_menu_load_panel(0, -22, 8, 2);

  /* world names panel */
  vb_menu_load_panel(0, 4, 10, 7);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_world_select_overlay()
*******************************************************************************/
short int vb_menu_load_world_select_overlay()
{
  int k;

  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 16, "Select World!");

  /* world names text */
  if (G_screen_page == 0)
    vb_menu_load_text(0, -6, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Episode 1");
  else if (G_screen_page == 1)
    vb_menu_load_text(0, -6, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Episode 2");
  else if (G_screen_page == 2)
    vb_menu_load_text(0, -6, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Episode 3");
  else if (G_screen_page == 3)
    vb_menu_load_text(0, -6, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Episode 4");
  else if (G_screen_page == 4)
    vb_menu_load_text(0, -6, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Episode 5");

  vb_menu_load_text(0, 0, VB_MENU_ALIGN_CENTER, 0, 1, 
                    LEVEL_EPISODE_NAME_SIZE, &G_episode_names[G_screen_page][0]);

  for (k = 0; k < SCREEN_WORLD_SELECT_NUM_CHOICES; k++)
  {
    vb_menu_load_text(-15, 6 + 4 * k, VB_MENU_ALIGN_LEFT, 0, 6, 
                      LEVEL_WORLD_NAME_SIZE, &G_world_names[SCREEN_WORLD_SELECT_NUM_CHOICES * G_screen_page + k][0]);
  }

  /* cursor */
  vb_menu_load_cursor(-17, 6 + (4 * G_screen_choice), 0, 1);

  /* page arrows */
  if (G_screen_page > 0)
    vb_menu_load_page_arrow_left();

  if (G_screen_page < SCREEN_WORLD_SELECT_NUM_PAGES - 1)
  {
    if (((G_screen_page == 0) && (G_save_game_percent >= 200)) || 
        ((G_screen_page == 1) && (G_save_game_percent >= 400)) || 
        ((G_screen_page == 2) && (G_save_game_percent >= 600)) || 
        ((G_screen_page == 3) && (G_save_game_percent >= 800)))
    {
      vb_menu_load_page_arrow_right();
    }
  }

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_room_select_panels()
*******************************************************************************/
short int vb_menu_load_room_select_panels()
{
  int k;
  int world_name_length;

  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* determine length of world name */
  world_name_length = LEVEL_WORLD_NAME_SIZE;

  for (k = 1; k < LEVEL_WORLD_NAME_SIZE; k++)
  {
    if (G_world_names[G_screen_alternate][k] == '\0')
    {
      world_name_length = k;
      break;
    }
  }

  if (world_name_length < 1)
    world_name_length = 1;
  else if (world_name_length > LEVEL_WORLD_NAME_SIZE)
    world_name_length = LEVEL_WORLD_NAME_SIZE;

  /* world name panel */
  if (world_name_length % 2 == 0)
    vb_menu_load_panel(0, -22, (world_name_length + 2) / 2, 2);
  else
    vb_menu_load_panel(0, -22, (world_name_length + 3) / 2, 2);

  /* room names panel */
  vb_menu_load_panel(0, 4, 14, 9);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_room_select_overlay()
*******************************************************************************/
short int vb_menu_load_room_select_overlay()
{
  int k;
  int bound;

  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* world name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 
                    LEVEL_WORLD_NAME_SIZE, &G_world_names[G_screen_alternate][0]);

  /* compute bound for displayed room names */
  if (G_save_game_data[G_screen_alternate] <= 0)
    bound = 1;
  else if (G_save_game_data[G_screen_alternate] >= SCREEN_ROOM_SELECT_NUM_CHOICES)
    bound = SCREEN_ROOM_SELECT_NUM_CHOICES;
  else
    bound = G_save_game_data[G_screen_alternate] + 1;

  /* display room names */
  for (k = 0; k < bound; k++)
  {
    /* for the last name, check if we should display this in        */
    /* another color to denote that it has not been completed yet.  */
    if ((k == bound - 1) && (G_save_game_data[G_screen_alternate] != SAVE_GAME_ROOMS_PER_WORLD))
    {
      vb_menu_load_text(-23, -10 + 4 * k, VB_MENU_ALIGN_LEFT, 0, 0, 
                        LEVEL_ROOM_NAME_SIZE, &G_room_names[SCREEN_ROOM_SELECT_NUM_CHOICES * G_screen_alternate + k][0]);
    }
    else
    {
      vb_menu_load_text(-23, -10 + 4 * k, VB_MENU_ALIGN_LEFT, 0, 6, 
                        LEVEL_ROOM_NAME_SIZE, &G_room_names[SCREEN_ROOM_SELECT_NUM_CHOICES * G_screen_alternate + k][0]);
    }
  }

  /* display ??? for rooms that have not been reached yet */
  for (k = bound; k < SCREEN_ROOM_SELECT_NUM_CHOICES; k++)
  {
    vb_menu_load_text(-23, -10 + 4 * k, VB_MENU_ALIGN_LEFT, -2, 6, 
                      8, "???");
  }

  /* cursor */
  vb_menu_load_cursor(-25, -10 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_story_select_panels()
*******************************************************************************/
short int vb_menu_load_story_select_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name panel */
  vb_menu_load_panel(0, -22, 11, 2);

  /* story scene names panel */
  vb_menu_load_panel(0, 4, 12, 7);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_story_select_overlay()
*******************************************************************************/
short int vb_menu_load_story_select_overlay()
{
  int k;
  int bound;

  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 20, "Select Story Scene!");

  /* story scene names text */
  if (G_save_game_percent < 200)
    bound = 1;
  else if (G_save_game_percent < 400)
    bound = 2;
  else if (G_save_game_percent < 600)
    bound = 3;
  else if (G_save_game_percent < 800)
    bound = 4;
  else if (G_save_game_percent < 1000)
    bound = 5;
  else
    bound = 6;

  for (k = 0; k < bound; k++)
  {
    vb_menu_load_text(-19, -6 + 4 * k, VB_MENU_ALIGN_LEFT, 0, 6, 
                      STORY_TITLE_SIZE, &G_story_scene_titles[k][0]);
  }

  for (k = bound; k < SCREEN_STORY_SELECT_NUM_CHOICES; k++)
  {
    vb_menu_load_text(-19, -6 + 4 * k, VB_MENU_ALIGN_LEFT, -2, 6, 
                      8, "???");
  }

  /* cursor */
  vb_menu_load_cursor(-21, -6 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_story_scene_panels()
*******************************************************************************/
short int vb_menu_load_story_scene_panels()
{
  int k;
  int title_length;

  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* determine length of story scene title */
  title_length = STORY_TITLE_SIZE;

  for (k = 1; k < STORY_TITLE_SIZE; k++)
  {
    if (G_story_scene_titles[G_screen_alternate][k] == '\0')
    {
      title_length = k;
      break;
    }
  }

  if (title_length < 1)
    title_length = 1;
  else if (title_length > STORY_TITLE_SIZE)
    title_length = STORY_TITLE_SIZE;

  /* title panel */
  if (title_length % 2 == 0)
    vb_menu_load_panel(0, -22, (title_length + 2) / 2, 2);
  else
    vb_menu_load_panel(0, -22, (title_length + 3) / 2, 2);

  /* lines panel */
  vb_menu_load_panel(0, 4, 17, 10);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_story_scene_overlay()
*******************************************************************************/
short int vb_menu_load_story_scene_overlay()
{
  int k;
  int offset_y;

  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* title text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 
                    STORY_TITLE_SIZE, &G_story_scene_titles[G_screen_alternate][0]);

  /* lines text */
  offset_y = -12;

  for (k = 0; k < STORY_LINES_PER_SCENE; k++)
  {
    /* if this is not a blank line, and space is available, load the text */
    if ((G_story_scene_lines[G_screen_alternate][k][0] != '\0') && (offset_y <= 21))
    {
      vb_menu_load_text(0, offset_y, VB_MENU_ALIGN_CENTER, 0, 6, 
                        STORY_LINE_SIZE, &G_story_scene_lines[G_screen_alternate][k][0]);
    }

    /* blank lines are treated as a half-line */
    if (G_story_scene_lines[G_screen_alternate][k][0] == '\0')
      offset_y += 2;
    else
      offset_y += 4;
  }

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_options_panels()
*******************************************************************************/
short int vb_menu_load_options_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* options (part 1) */
  if (G_screen_alternate == SCREEN_OPTIONS_ALTERNATE_STANDARD)
  {
    /* screen name panel */
    vb_menu_load_panel(0, -22, 5, 2);

    /* options panel */
    vb_menu_load_panel(0, 4, 14, 7);
  }
  /* more options (part 2) */
  else if (G_screen_alternate == SCREEN_OPTIONS_ALTERNATE_MORE)
  {
    /* screen name panel */
    vb_menu_load_panel(0, -22, 7, 2);

    /* options panel */
    vb_menu_load_panel(0, 4, 14, 6);
  }

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_options_overlay()
*******************************************************************************/
short int vb_menu_load_options_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* options (part 1) */
  if (G_screen_alternate == SCREEN_OPTIONS_ALTERNATE_STANDARD)
  {
    /* screen name text */
    vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 16, "Options");

    /* options text */
    vb_menu_load_text(-23, -6, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Window Size");
    vb_menu_load_text(-23, -2, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Fullscreen");
    vb_menu_load_text(-23,  2, VB_MENU_ALIGN_LEFT, 0, 6, 16, "V Sync");
    vb_menu_load_text(-23,  6, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Brightness");
    vb_menu_load_text(-23, 10, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Upscaling Mode");
    vb_menu_load_text(-23, 14, VB_MENU_ALIGN_LEFT, 0, 6, 16, "More Options");

    /* cursor */
    vb_menu_load_cursor(-25, -6 + (4 * G_screen_choice), 0, 1);

    /* current resolution */
    if (G_graphics_resolution == GRAPHICS_RESOLUTION_480P)
      vb_menu_load_text(25, -6, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "480p");
    else if (G_graphics_resolution == GRAPHICS_RESOLUTION_720P)
      vb_menu_load_text(25, -6, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "720p");
    else if (G_graphics_resolution == GRAPHICS_RESOLUTION_768P)
      vb_menu_load_text(25, -6, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "768p");
    else if (G_graphics_resolution == GRAPHICS_RESOLUTION_1080P)
      vb_menu_load_text(25, -6, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "1080p");

    /* current fullscreen setting */
    if (G_flag_window_fullscreen == 0)
      vb_menu_load_text(25, -2, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "Off");
    else
      vb_menu_load_text(25, -2, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "On");

    /* current vsync setting */
    if (G_flag_window_vsync == 0)
      vb_menu_load_text(25,  2, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "Off");
    else
      vb_menu_load_text(25,  2, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "On");

    /* brightness slider */
    vb_menu_load_slider(25,  6, VB_MENU_ALIGN_RIGHT, 0, 6, G_brightness_index);

    /* upscaling mode */
    if (G_upscaling_mode == VIDEO_UPSCALING_MODE_LINEAR)
      vb_menu_load_text(25, 10, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "Linear");
    else if (G_upscaling_mode == VIDEO_UPSCALING_MODE_PIXELS)
      vb_menu_load_text(25, 10, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "Pixels");
    else if (G_upscaling_mode == VIDEO_UPSCALING_MODE_SCANLINES)
      vb_menu_load_text(25, 10, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "Scanlines");
    else
      vb_menu_load_text(25, 10, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "Linear");
  }
  /* more options (part 2) */
  else if (G_screen_alternate == SCREEN_OPTIONS_ALTERNATE_MORE)
  {
    /* screen name text */
    vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 16, "More Options");

    /* options text */
    vb_menu_load_text(-23, -4, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Sound Volume");

    if (G_active_gamepad != CONTROLS_ACTIVE_GAMEPAD_NONE)
    {
      vb_menu_load_text(-23,  0, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Gamepad No.");
      vb_menu_load_text(-23,  4, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Game Buttons");
      vb_menu_load_text(-23,  8, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Menu Buttons");
    }
    else
    {
      vb_menu_load_text(-23,  0, VB_MENU_ALIGN_LEFT, -2, 6, 16, "Gamepad No.");
      vb_menu_load_text(-23,  4, VB_MENU_ALIGN_LEFT, -2, 6, 16, "Game Buttons");
      vb_menu_load_text(-23,  8, VB_MENU_ALIGN_LEFT, -2, 6, 16, "Menu Buttons");
    }

    vb_menu_load_text(-23, 12, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Menu Keys");

    /* cursor */
    vb_menu_load_cursor(-25, -4 + (4 * G_screen_choice), 0, 1);

    /* sound volume slider */
    vb_menu_load_slider(25,  -4, VB_MENU_ALIGN_RIGHT, 0, 6, (G_sound_volume + 8) / 16);

    /* gamepad options */
    if (G_active_gamepad != CONTROLS_ACTIVE_GAMEPAD_NONE)
    {
      if (G_active_gamepad == CONTROLS_ACTIVE_GAMEPAD_READY)
        vb_menu_load_text(25,  0, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "???");
      else if (G_active_gamepad == CONTROLS_ACTIVE_GAMEPAD_1)
        vb_menu_load_text(25,  0, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "1");
      else if (G_active_gamepad == CONTROLS_ACTIVE_GAMEPAD_2)
        vb_menu_load_text(25,  0, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "2");
      else if (G_active_gamepad == CONTROLS_ACTIVE_GAMEPAD_3)
        vb_menu_load_text(25,  0, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "3");
      else if (G_active_gamepad == CONTROLS_ACTIVE_GAMEPAD_4)
        vb_menu_load_text(25,  0, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "4");

      if (G_game_button_layout == CONTROLS_GAME_BUTTON_LAYOUT_BA)
        vb_menu_load_text(25,  4, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "BA");
      else if (G_game_button_layout == CONTROLS_GAME_BUTTON_LAYOUT_YB)
        vb_menu_load_text(25,  4, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "YB");

      if (G_menu_button_layout == CONTROLS_MENU_BUTTON_LAYOUT_BA)
        vb_menu_load_text(25,  8, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "BA");
      else if (G_menu_button_layout == CONTROLS_MENU_BUTTON_LAYOUT_AB)
        vb_menu_load_text(25,  8, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "AB");
    }
    else
    {
      vb_menu_load_text(25,  0, VB_MENU_ALIGN_RIGHT, -2, 6, 16, "n/a");
      vb_menu_load_text(25,  4, VB_MENU_ALIGN_RIGHT, -2, 6, 16, "n/a");
      vb_menu_load_text(25,  8, VB_MENU_ALIGN_RIGHT, -2, 6, 16, "n/a");
    }

    /* keyboard options */
    if (G_menu_key_layout == CONTROLS_MENU_KEY_LAYOUT_ZX)
      vb_menu_load_text(25, 12, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "ZX");
    else if (G_menu_key_layout == CONTROLS_MENU_KEY_LAYOUT_XZ)
      vb_menu_load_text(25, 12, VB_MENU_ALIGN_RIGHT, 0, 6, 16, "XZ");
  }

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_help_panels()
*******************************************************************************/
short int vb_menu_load_help_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name panel */
  vb_menu_load_panel(0, -22, 3, 2);

  /* help panel */
  vb_menu_load_panel(0, 4, 14, 9);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_help_overlay()
*******************************************************************************/
short int vb_menu_load_help_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 16, "Help");

  /* help text */
  if (G_screen_page == 0)
  {
    vb_menu_load_text(0, -10, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Controls");

    vb_menu_load_text(-25, -6, VB_MENU_ALIGN_LEFT, 0, 1, 32, "Arrow Keys:");
    vb_menu_load_text( -1, -6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Move");

    vb_menu_load_text(-25, -2, VB_MENU_ALIGN_LEFT, 0, 1, 32, "Z:");
    vb_menu_load_text(-19, -2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Bat Transform");

    vb_menu_load_text(-19,  2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "(Needs 1 Bat Vial)");

    vb_menu_load_text(-25,  6, VB_MENU_ALIGN_LEFT, 0, 1, 32, "X:");
    vb_menu_load_text(-19,  6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Cast Ice Spell");

    vb_menu_load_text(-19, 10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "(Needs 1 Mana Prism)");

    vb_menu_load_text(-25, 14, VB_MENU_ALIGN_LEFT, 0, 6, 32, "If Already a Bat...");

    vb_menu_load_text(-25, 18, VB_MENU_ALIGN_LEFT, 0, 1, 32, "Z:");
    vb_menu_load_text(-19, 18, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Vampire Transform");
  }
  else if (G_screen_page == 1)
  {
    vb_menu_load_text(-25, -10, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, -10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Pick up all the vials");
    vb_menu_load_text(-21, -6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "to open the portcullis.");

    vb_menu_load_text(-25, -2, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, -2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Then, walk through the");
    vb_menu_load_text(-21,  2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "opened portcullis to");
    vb_menu_load_text(-21,  6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "complete the room!");

    vb_menu_load_text(-25, 10, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, 10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Try not to get zapped");
    vb_menu_load_text(-21, 14, VB_MENU_ALIGN_LEFT, 0, 6, 32, "by the pumpkins, ghosts,");
    vb_menu_load_text(-21, 18, VB_MENU_ALIGN_LEFT, 0, 6, 32, "or anything else!");
  }
  else if (G_screen_page == 2)
  {
    vb_menu_load_text(-25, -10, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, -10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "When turned into a bat,");
    vb_menu_load_text(-21, -6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "you can fly over spikes,");
    vb_menu_load_text(-21, -2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "water, pots, and flasks.");

    vb_menu_load_text(-25,  2, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21,  2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "However, the bat cannot");
    vb_menu_load_text(-21,  6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "pick up vials.");

    vb_menu_load_text(-25, 10, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, 10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "The ice spell will turn");
    vb_menu_load_text(-21, 14, VB_MENU_ALIGN_LEFT, 0, 6, 32, "most denizens into an");
    vb_menu_load_text(-21, 18, VB_MENU_ALIGN_LEFT, 0, 6, 32, "ice cube! Brrr!");
  }
  else if (G_screen_page == 3)
  {
    vb_menu_load_text(-25, -10, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, -10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Some of the denizens");
    vb_menu_load_text(-21, -6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "are not the same as you");
    vb_menu_load_text(-21, -2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "remember!");

    vb_menu_load_text(-25,  2, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21,  2, VB_MENU_ALIGN_LEFT, 0, 6, 32, "Just experiment and play");
    vb_menu_load_text(-21,  6, VB_MENU_ALIGN_LEFT, 0, 6, 32, "the game and find out!");

    vb_menu_load_text(-25, 10, VB_MENU_ALIGN_LEFT, 0, 1, 32, "*");
    vb_menu_load_text(-21, 10, VB_MENU_ALIGN_LEFT, 0, 6, 32, "And most importantly...");
    vb_menu_load_text(0, 16, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Have fun!");
  }

  /* page arrows */
  if (G_screen_page > 0)
    vb_menu_load_page_arrow_left();

  if (G_screen_page < SCREEN_HELP_NUM_PAGES - 1)
    vb_menu_load_page_arrow_right();

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_credits_panels()
*******************************************************************************/
short int vb_menu_load_credits_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name panel */
  vb_menu_load_panel(0, -22, 5, 2);

  /* credits panel */
  vb_menu_load_panel(0, 4, 13, 9);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_credits_overlay()
*******************************************************************************/
short int vb_menu_load_credits_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* screen name text */
  vb_menu_load_text(0, -22, VB_MENU_ALIGN_CENTER, 0, 1, 16, "Credits");

  /* credits text */
  vb_menu_load_text(0, -10, VB_MENU_ALIGN_CENTER, 0, 6, 24, "Programming, Design,");
  vb_menu_load_text(0, -6, VB_MENU_ALIGN_CENTER, 0, 6, 24, "Story, Graphics,");
  vb_menu_load_text(0, -2, VB_MENU_ALIGN_CENTER, 0, 6, 24, "SFX, and Music by");

  vb_menu_load_text(0, 6, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Michael Behrens");

  vb_menu_load_text(0, 14, VB_MENU_ALIGN_CENTER, 0, 6, 24, "Please do not feed the");
  vb_menu_load_text(0, 18, VB_MENU_ALIGN_CENTER, 0, 6, 24, "vampires. They bite!");

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_hud_overlay()
*******************************************************************************/
short int vb_menu_load_hud_overlay()
{
  int cell_x;
  int cell_y;

  int sprite_index;

  /* initialize sprite index */
  sprite_index =  GRAPHICS_OVERLAY_SPRITES_START_INDEX + 
                  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY];

  /* load bat vial & mana prism */
  VB_MENU_ADD_HUD_SPRITE_TO_BUFFERS(22, 40, GRAPHICS_Z_LEVEL_OVERLAY, 
                                    60, 14, 0, 0)

  VB_MENU_ADD_HUD_SPRITE_TO_BUFFERS(22, 68, GRAPHICS_Z_LEVEL_OVERLAY, 
                                    60, 16, 0, 0)

  /* load digit for bat vial count */
  if ((G_num_bat_vials >= 0) && (G_num_bat_vials <= 9))
  {
    cell_x = 48 + G_num_bat_vials;
    cell_y = 0;
  }
  else
  {
    cell_x = 57;
    cell_y = 0;
  }

  VB_MENU_ADD_FONT_CHARACTER_TO_BUFFERS(38, 40, GRAPHICS_Z_LEVEL_OVERLAY, 
                                        cell_x, cell_y, 0, 6)

  /* load digit for mana prism count */
  if ((G_num_mana_prisms >= 0) && (G_num_mana_prisms <= 9))
  {
    cell_x = 48 + G_num_mana_prisms;
    cell_y = 0;
  }
  else
  {
    cell_x = 57;
    cell_y = 0;
  }

  VB_MENU_ADD_FONT_CHARACTER_TO_BUFFERS(38, 68, GRAPHICS_Z_LEVEL_OVERLAY, 
                                        cell_x, cell_y, 0, 6)

  /* update overlay sprite layer count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 
    sprite_index - GRAPHICS_OVERLAY_SPRITES_START_INDEX;

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_room_intro_panels()
*******************************************************************************/
short int vb_menu_load_room_intro_panels()
{
  int i;
  int length;

  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* determine length of room name */
  length = LEVEL_ROOM_NAME_SIZE;

  for (i = 1; i < LEVEL_ROOM_NAME_SIZE; i++)
  {
    if (G_room_names[G_current_room][i] == '\0')
    {
      length = i;
      break;
    }
  }

  if (length < 1)
    length = 1;
  else if (length > LEVEL_ROOM_NAME_SIZE)
    length = LEVEL_ROOM_NAME_SIZE;

  /* draw room intro panel */
  if (length % 2 == 0)
    vb_menu_load_panel(0, 0, (length + 2) / 2, 2);
  else
    vb_menu_load_panel(0, 0, (length + 3) / 2, 2);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_room_intro_overlay()
*******************************************************************************/
short int vb_menu_load_room_intro_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* draw room intro text */
  vb_menu_load_text(0, 0, VB_MENU_ALIGN_CENTER, 0, 0, 
                    LEVEL_ROOM_NAME_SIZE, &G_room_names[G_current_room][0]);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_room_complete_panels()
*******************************************************************************/
short int vb_menu_load_room_complete_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* draw room complete panel */
  vb_menu_load_panel(0, 0, 8, 2);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_room_complete_overlay()
*******************************************************************************/
short int vb_menu_load_room_complete_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* draw room complete text */
  vb_menu_load_text(0, 0, VB_MENU_ALIGN_CENTER, 0, 0, 16, "Room Complete!");

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_pause_panels()
*******************************************************************************/
short int vb_menu_load_pause_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* pause panel */
  vb_menu_load_panel(0, 0, 8, 6);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_pause_overlay()
*******************************************************************************/
short int vb_menu_load_pause_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* pause text */
  vb_menu_load_text(-11, -8, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Back to Game");
  vb_menu_load_text(-11, -4, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Retry Room");
  vb_menu_load_text(-11,  0, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Options");
  vb_menu_load_text(-11,  4, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Help");
  vb_menu_load_text(-11,  8, VB_MENU_ALIGN_LEFT, 0, 6, 16, "Room Select");

  /* cursor */
  vb_menu_load_cursor(-13, -8 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_retry_panels()
*******************************************************************************/
short int vb_menu_load_retry_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* retry panel */
  vb_menu_load_panel(0, 0, 4, 4);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_retry_overlay()
*******************************************************************************/
short int vb_menu_load_retry_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* retry text */
  vb_menu_load_text(0, -4, VB_MENU_ALIGN_CENTER, 0, 0, 8, "Retry?");
  vb_menu_load_text(-2,  0, VB_MENU_ALIGN_LEFT, 0, 6, 8, "Yes");
  vb_menu_load_text(-2,  4, VB_MENU_ALIGN_LEFT, 0, 6, 8, "No");

  /* cursor */
  vb_menu_load_cursor(-4, 0 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_try_again_panels()
*******************************************************************************/
short int vb_menu_load_try_again_panels()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* try again panel */
  vb_menu_load_panel(0, 0, 6, 4);

  /* update vbos */
  VB_MENU_UPDATE_PANELS_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_load_try_again_overlay()
*******************************************************************************/
short int vb_menu_load_try_again_overlay()
{
  /* reset overlay sprite vbo count */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  /* try again text */
  vb_menu_load_text(0, -4, VB_MENU_ALIGN_CENTER, 0, 0, 12, "Try Again?");
  vb_menu_load_text(-2,  0, VB_MENU_ALIGN_LEFT, 0, 6, 8, "Yes");
  vb_menu_load_text(-2,  4, VB_MENU_ALIGN_LEFT, 0, 6, 8, "No");

  /* cursor */
  vb_menu_load_cursor(-4, 0 + (4 * G_screen_choice), 0, 1);

  /* update vbos */
  VB_MENU_UPDATE_OVERLAY_SPRITES_IN_VBOS()

  return 0;
}

/*******************************************************************************
** vb_menu_clear_panels_and_overlay()
*******************************************************************************/
short int vb_menu_clear_panels_and_overlay()
{
  /* reset sprite vbo counts */
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_PANELS] = 0;
  G_sprite_layer_counts[GRAPHICS_SPRITE_LAYER_OVERLAY] = 0;

  return 0;
}

