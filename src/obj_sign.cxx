/*
 * sign.cxx
 * Daniel Nelson - 9/14/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Daniel Nelson - aluminumangel.org
 * 174 W. 18th Ave.
 * Columbus, OH  43210
 *
 * Generates the display list and textures for the reward signs.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "TextureLoader.h"

GLuint Displayer::sign_small_list;
GLuint Displayer::sign_large_list;
GLuint Displayer::sign_small_texture;
GLuint Displayer::sign_large_texture;

const char *sign_small_subtexture_files[DC_NUMBER_SMALL_SIGN_SUBTEXTURES]
 = { GC_DATA_DIRECTORY "sign_4.tga",
     GC_DATA_DIRECTORY "sign_5.tga",
     GC_DATA_DIRECTORY "sign_6.tga",
     GC_DATA_DIRECTORY "sign_7.tga",
     GC_DATA_DIRECTORY "sign_8.tga",
     GC_DATA_DIRECTORY "sign_9.tga" };

const char *sign_large_subtexture_files[DC_NUMBER_LARGE_SIGN_SUBTEXTURES]
 = { GC_DATA_DIRECTORY "sign_10.tga",
     GC_DATA_DIRECTORY "sign_11.tga",
     GC_DATA_DIRECTORY "sign_12.tga",
     GC_DATA_DIRECTORY "sign_x2.tga",
     GC_DATA_DIRECTORY "sign_x3.tga",
     GC_DATA_DIRECTORY "sign_x4.tga",
     GC_DATA_DIRECTORY "sign_x5.tga",
     GC_DATA_DIRECTORY "sign_x6.tga",
     GC_DATA_DIRECTORY "sign_x7.tga",
     GC_DATA_DIRECTORY "sign_x8.tga",
     GC_DATA_DIRECTORY "sign_x9.tga",
     GC_DATA_DIRECTORY "sign_x10.tga",
     GC_DATA_DIRECTORY "sign_x11.tga",
     GC_DATA_DIRECTORY "sign_x12.tga",
     GC_DATA_DIRECTORY "sign_bonus.tga" };

void Displayer::generateSignList (   )
{
  GLubyte *subtextures
   [max(DC_NUMBER_SMALL_SIGN_SUBTEXTURES, DC_NUMBER_LARGE_SIGN_SUBTEXTURES)];

  // load and pack the small sign textures

  glGenTextures(1, &sign_small_texture);

  glBindTexture(GL_TEXTURE_2D, sign_small_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  for (int n = DC_NUMBER_SMALL_SIGN_SUBTEXTURES; n--; )
    subtextures[n] = TextureLoader::loadAlphaTGA(sign_small_subtexture_files[n],
     DC_SIGN_SMALL_SUBTEX_LENGTH_S, DC_SIGN_SMALL_SUBTEX_LENGTH_T);
  
  GLubyte small_texture[DC_SIGN_SMALL_TEX_LENGTH_S][DC_SIGN_SMALL_TEX_LENGTH_T];

  for (int s = 0; s < DC_SIGN_SMALL_TEX_LENGTH_S; s++)
    for (int t = 0; t < DC_SIGN_SMALL_TEX_LENGTH_T; t++) {
      unsigned n = (s / DC_SIGN_SMALL_SUBTEX_LENGTH_S)
       * DC_SIGN_SMALL_TEX_GRID_LENGTH_T + t / DC_SIGN_SMALL_SUBTEX_LENGTH_T;

      if (n >= DC_NUMBER_SMALL_SIGN_SUBTEXTURES)
        small_texture[s][t] = 0;
      else
        small_texture[s][t] = subtextures[n][s % DC_SIGN_SMALL_SUBTEX_LENGTH_S
         * DC_SIGN_SMALL_SUBTEX_LENGTH_T + t % DC_SIGN_SMALL_SUBTEX_LENGTH_T];
    }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, DC_SIGN_SMALL_TEX_LENGTH_T,
   DC_SIGN_SMALL_TEX_LENGTH_S, GL_FALSE, GL_ALPHA, GL_UNSIGNED_BYTE,
   small_texture);

  for (int n = DC_NUMBER_SMALL_SIGN_SUBTEXTURES; n--; )
    if (subtextures[n] != null) {
      delete [] subtextures[n];
      subtextures[n] = null;
    }

  // load and pack the large sign textures

  glGenTextures(1, &sign_large_texture);

  glBindTexture(GL_TEXTURE_2D, sign_large_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  for (int n = DC_NUMBER_LARGE_SIGN_SUBTEXTURES; n--; )
    subtextures[n] = TextureLoader::loadAlphaTGA(sign_large_subtexture_files[n],
     DC_SIGN_LARGE_SUBTEX_LENGTH_S, DC_SIGN_LARGE_SUBTEX_LENGTH_T);
  
  GLubyte large_texture[DC_SIGN_LARGE_TEX_LENGTH_S][DC_SIGN_LARGE_TEX_LENGTH_T];

  for (int s = 0; s < DC_SIGN_LARGE_TEX_LENGTH_S; s++)
    for (int t = 0; t < DC_SIGN_LARGE_TEX_LENGTH_T; t++) {
      unsigned n = (s / DC_SIGN_LARGE_SUBTEX_LENGTH_S)
       * DC_SIGN_LARGE_TEX_GRID_LENGTH_T + t / DC_SIGN_LARGE_SUBTEX_LENGTH_T;

      if (n >= DC_NUMBER_LARGE_SIGN_SUBTEXTURES)
        large_texture[s][t] = 0;
      else 
        large_texture[s][t] = subtextures[n][s % DC_SIGN_LARGE_SUBTEX_LENGTH_S
         * DC_SIGN_LARGE_SUBTEX_LENGTH_T + t % DC_SIGN_LARGE_SUBTEX_LENGTH_T];
    }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, DC_SIGN_LARGE_TEX_LENGTH_T,
   DC_SIGN_LARGE_TEX_LENGTH_S, GL_FALSE, GL_ALPHA, GL_UNSIGNED_BYTE,
   large_texture);

  for (int n = DC_NUMBER_LARGE_SIGN_SUBTEXTURES; n--; )
    if (subtextures[n] != null) {
      delete [] subtextures[n];
      subtextures[n] = null;
    }

  // build the sign display lists

  sign_small_list = glGenLists(1);

  glNewList(sign_small_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, DC_SIGN_SMALL_SUBTEX_COORD_S);
      glVertex3f(-DC_SIGN_LENGTH, -DC_SIGN_LENGTH, 0.0f);
      glTexCoord2f(DC_SIGN_SMALL_SUBTEX_COORD_T, DC_SIGN_SMALL_SUBTEX_COORD_S);
      glVertex3f(DC_SIGN_LENGTH, -DC_SIGN_LENGTH, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-DC_SIGN_LENGTH, DC_SIGN_LENGTH, 0.0f);
      glTexCoord2f(DC_SIGN_SMALL_SUBTEX_COORD_T, 0.0f);
      glVertex3f(DC_SIGN_LENGTH, DC_SIGN_LENGTH, 0.0f);

    glEnd();
  glEndList();

  sign_large_list = glGenLists(1);

  glNewList(sign_large_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, DC_SIGN_LARGE_SUBTEX_COORD_S);
      glVertex3f(-2.0f * DC_SIGN_LENGTH, -DC_SIGN_LENGTH, 0.0f);
      glTexCoord2f(DC_SIGN_LARGE_SUBTEX_COORD_T, DC_SIGN_LARGE_SUBTEX_COORD_S);
      glVertex3f(2.0f * DC_SIGN_LENGTH, -DC_SIGN_LENGTH, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-2.0f * DC_SIGN_LENGTH, DC_SIGN_LENGTH, 0.0f);
      glTexCoord2f(DC_SIGN_LARGE_SUBTEX_COORD_T, 0.0f);
      glVertex3f(2.0f * DC_SIGN_LENGTH, DC_SIGN_LENGTH, 0.0f);

    glEnd();
  glEndList();
}
