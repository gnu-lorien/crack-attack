/*
 * clock.cxx
 * Daniel Nelson - 11/2/0
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
 * Generates the textures for the clock.
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

GLuint Displayer::clock_digit_textures[11];

const char *clock_digit_texture_files[11]
 = { GC_DATA_DIRECTORY "clock_0.tga",
     GC_DATA_DIRECTORY "clock_1.tga",
     GC_DATA_DIRECTORY "clock_2.tga",
     GC_DATA_DIRECTORY "clock_3.tga",
     GC_DATA_DIRECTORY "clock_4.tga",
     GC_DATA_DIRECTORY "clock_5.tga",
     GC_DATA_DIRECTORY "clock_6.tga",
     GC_DATA_DIRECTORY "clock_7.tga",
     GC_DATA_DIRECTORY "clock_8.tga",
     GC_DATA_DIRECTORY "clock_9.tga",
     GC_DATA_DIRECTORY "clock_extra.tga" };

void Displayer::generateClock (   )
{
  GLubyte *texture;

  glGenTextures(11, clock_digit_textures);

  for (int n = 11; n--; ) {

    glBindTexture(GL_TEXTURE_2D, clock_digit_textures[n]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    texture = TextureLoader::loadAlphaTGA(clock_digit_texture_files[n],
     DC_CLOCK_TEX_LENGTH, DC_CLOCK_TEX_LENGTH);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, DC_CLOCK_TEX_LENGTH,
     DC_CLOCK_TEX_LENGTH, GL_FALSE, GL_ALPHA, GL_UNSIGNED_BYTE, texture);

    if (texture != null) {
      delete [] texture;
      texture = null;
    }
  }
}
