/*
 * names.cxx
 * Daniel Nelson - 11/9/0
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
 * Build the name texture.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "String.h"
#include "MetaState.h"
#include "Communicator.h"

GLuint Displayer::name_texture;

void Displayer::generateNameTexture (   )
{
  glGenTextures(1, &name_texture);

  glBindTexture(GL_TEXTURE_2D, name_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLubyte texture[DC_NAME_TEX_LENGTH_T][DC_NAME_TEX_LENGTH_S][3];
  for (int t = DC_NAME_TEX_LENGTH_T; t--; )
    for (int s = DC_NAME_TEX_LENGTH_S; s--; )
      texture[t][s][0] = texture[t][s][1] = texture[t][s][2] = 0;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DC_NAME_TEX_LENGTH_S,
   DC_NAME_TEX_LENGTH_T, GL_FALSE, GL_RGB, GL_UNSIGNED_BYTE, texture);

  if (MetaState::mode & CM_SOLO) {

    int width = String::stringWidth(MetaState::player_name,
     DC_NAME_TEX_LENGTH_S);

    GLubyte *name_subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
    String::fillStringTexture(MetaState::player_name, name_subtexture, width);

    glTexSubImage2D(GL_TEXTURE_2D, 0, (DC_NAME_TEX_LENGTH_S - width) / 2,
     (DC_NAME_TEX_LENGTH_T - DC_LETTER_TEX_LENGTH) / 2, width,
     DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, name_subtexture);

    if (name_subtexture != null) {
      delete [] name_subtexture;
      name_subtexture = null;
    }

  } else {
    int x;
    int width;
    GLubyte *string_subtexture;

    width = String::stringWidth(DC_VS_STRING, DC_NAME_TEX_LENGTH_S);

    string_subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
    String::fillStringTexture(DC_VS_STRING, string_subtexture, width);

    glTexSubImage2D(GL_TEXTURE_2D, 0, (DC_NAME_TEX_LENGTH_S - width) / 2,
     (DC_NAME_TEX_LENGTH_T - DC_LETTER_TEX_LENGTH) / 2, width,
     DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, string_subtexture);

    if (string_subtexture != null) {
      delete [] string_subtexture;
      string_subtexture = null;
    }

    width = String::stringWidth(MetaState::player_name, DC_NAME_TEX_LENGTH_S);

    string_subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
    String::fillStringTexture(MetaState::player_name, string_subtexture, width);

    if (width >= 2 * DC_NAME_TEX_LENGTH_S / 3)
      x = 0;
    else
      x = ((2 * DC_NAME_TEX_LENGTH_S / 3) - width) / 2;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, width,
     DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, string_subtexture);

    if (string_subtexture != null) {
      delete [] string_subtexture;
      string_subtexture = null;
    }

    width = String::stringWidth(Communicator::opponent_name,
     DC_NAME_TEX_LENGTH_S);

    string_subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
    String::fillStringTexture(Communicator::opponent_name, string_subtexture,
     width);

    if (width >= 2 * DC_NAME_TEX_LENGTH_S / 3)
      x = DC_NAME_TEX_LENGTH_S - width;
    else
      x = DC_NAME_TEX_LENGTH_S - width
       - ((2 * DC_NAME_TEX_LENGTH_S / 3) - width) / 2;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x,
     DC_NAME_TEX_LENGTH_T - DC_LETTER_TEX_LENGTH, width,
     DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, string_subtexture);

    if (string_subtexture != null) {
      delete [] string_subtexture;
      string_subtexture = null;
    }
  }
}
