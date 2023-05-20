/*******************************************************************************
** texture.c (opengl graphics rom data texture)
*******************************************************************************/

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "texture.h"

GLuint G_texture_id_rom_data;

GLfloat G_texture_coord_table[65];

/*******************************************************************************
** texture_generate_coord_tables()
*******************************************************************************/
short int texture_generate_coord_tables()
{
  int k;

  /* texture coordinate table */
  /* finding coordinates at the borders of a 8x8 cell on the texture  */
  /* on a 512x512 texture, the pixel size is 1/512, so the borders    */
  /* are at multiples of 8/512 = 1/64 = 0.015625                      */
  for (k = 0; k < 64; k++)
    G_texture_coord_table[k] = k * 0.015625f;

  G_texture_coord_table[64] = 1.0f;

  return 0;
}

/*******************************************************************************
** texture_init()
*******************************************************************************/
short int texture_init()
{
  G_texture_id_rom_data = 0;

  return 0;
}

/*******************************************************************************
** texture_deinit()
*******************************************************************************/
short int texture_deinit()
{
  /* clear texture id from opengl */
  if (G_texture_id_rom_data != 0)
  {
    glDeleteTextures(1, &(G_texture_id_rom_data));
    G_texture_id_rom_data = 0;
  }

  return 0;
}

/*******************************************************************************
** texture_load_all_from_file()
*******************************************************************************/
short int texture_load_all_from_file(char* filename)
{
  FILE* fp;

  int   m;
  int   n;

  char  signature[4];
  char  type[4];

  int   buffer[4];

  unsigned char*  image_data;
  GLfloat*        texture_data;

  if (filename == NULL)
    return 1;

  /* initialize variables */
  texture_data = NULL;

  /* open file */
  fp = fopen(filename, "rb");

  /* if file did not open, return error */
  if (fp == NULL)
  {
    fclose(fp);
    return 1;
  }

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

  if ((type[0] != 'G') || 
      (type[1] != 'R') || 
      (type[2] != 'P') || 
      (type[3] != 'H'))
  {
    fclose(fp);
    return 1;
  }

  /* allocate image data */
  image_data = malloc(sizeof(unsigned char) * TEXTURE_SIZE * TEXTURE_SIZE);

  /* allocate texture data */
  texture_data = malloc(sizeof(GLfloat) * 2 * TEXTURE_SIZE * TEXTURE_SIZE);

  /* read image data from file */
  if (fread(image_data, 1, TEXTURE_SIZE * TEXTURE_SIZE, fp) == 0)
  {
    free(image_data);
    fclose(fp);
    return 1;
  }

  /* convert to floating point texture data */
  for (m = 0; m < TEXTURE_SIZE * TEXTURE_SIZE / 4; m++)
  {
    /* read 4 pixels from each set of 4 bytes         */
    /* (the pixels in the image data are 8 bits each) */
    buffer[0] = image_data[4 * m + 0];
    buffer[1] = image_data[4 * m + 1];
    buffer[2] = image_data[4 * m + 2];
    buffer[3] = image_data[4 * m + 3];

    /* write out each pixel to the texture    */
    /* the 1st byte is the palette index      */
    /* the 2nd byte is the transparency       */
    for (n = 0; n < 4; n++)
    {
      /* find the coordinates at the center of a pixel on the palette   */
      /* on a 256x256 texture, the pixel size is 1/256, so the centers  */
      /* are at odd multiples of 1/512 = 0.001953125                    */
      texture_data[2 * (4 * m + n) + 0] = (2 * buffer[n] + 1) * 0.001953125f;

      /* set transparency (for sprite rendering) */
      if (buffer[n] == 255)
        texture_data[2 * (4 * m + n) + 1] = 0.0f;
      else
        texture_data[2 * (4 * m + n) + 1] = 1.0f;
    }
  }

  /* assign gl texture id to this texture, set it to the current gl texture */
  glGenTextures(1, &(G_texture_id_rom_data));
  glBindTexture(GL_TEXTURE_2D, G_texture_id_rom_data);

  /* set up texture in opengl */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  /* load the texture to opengl from the texture data */
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RG32F, TEXTURE_SIZE, TEXTURE_SIZE, 
                0, GL_RG, GL_FLOAT, texture_data);

  /* close file */
  fclose(fp);

  /* clear image data */
  if (image_data != NULL)
  {
    free(image_data);
    image_data = NULL;
  }

  /* clear texture data */
  if (texture_data != NULL)
  {
    free(texture_data);
    texture_data = NULL;
  }

  return 0;
}
