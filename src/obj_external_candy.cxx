/* 
 * external_candy.cxx
 * Daniel Nelson - 10/13/0
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
 * Generates lists and textures for candy external to the game.
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
#include "MetaState.h"
#include "GarbageFlavorImage.h"

GLuint Displayer::logo_texture;
GLuint Displayer::losebar_texture;

GLuint Displayer::logo_list;

void Displayer::generateExternalCandy (   )
{
  // the logo

  glGenTextures(1, &logo_texture);

  glBindTexture(GL_TEXTURE_2D, logo_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLubyte *texture;
  if (MetaState::mode & CM_X)
    texture = TextureLoader::loadNoAlphaTGA(GC_X_LOGO_TEX_FILE_NAME,
     DC_LOGO_TEX_LENGTH, DC_LOGO_TEX_LENGTH);
  else
    texture = TextureLoader::loadNoAlphaTGA(GC_LOGO_TEX_FILE_NAME,
     DC_LOGO_TEX_LENGTH, DC_LOGO_TEX_LENGTH);

#if 0 // use to see the first entry in the garbage flavor texture set
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DC_GARBAGE_TEX_LENGTH,
   DC_GARBAGE_TEX_LENGTH, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE,
   garbage_texture_data[0]);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DC_LOGO_TEX_LENGTH,
   DC_LOGO_TEX_LENGTH, GL_FALSE, GL_RGB, GL_UNSIGNED_BYTE, texture);
#endif

  if (texture != null) {
    delete [] texture;
    texture = null;
  }

  logo_list = glGenLists(1);

  glNewList(logo_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(DC_LEFT_EXTERNAL_CENTER - DC_LOGO_LENGTH / 2.0f,
       DC_LOGO_OFFSET_Y - DC_LOGO_LENGTH / 2.0f, DC_EXTERNAL_OFFSET_Z);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(DC_LEFT_EXTERNAL_CENTER + DC_LOGO_LENGTH / 2.0f,
       DC_LOGO_OFFSET_Y - DC_LOGO_LENGTH / 2.0f, DC_EXTERNAL_OFFSET_Z);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(DC_LEFT_EXTERNAL_CENTER - DC_LOGO_LENGTH / 2.0f,
       DC_LOGO_OFFSET_Y + DC_LOGO_LENGTH / 2.0f, DC_EXTERNAL_OFFSET_Z);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(DC_LEFT_EXTERNAL_CENTER + DC_LOGO_LENGTH / 2.0f,
       DC_LOGO_OFFSET_Y + DC_LOGO_LENGTH / 2.0f, DC_EXTERNAL_OFFSET_Z);

    glEnd();
  glEndList();

  // the losebar

  glGenTextures(1, &losebar_texture);

  glBindTexture(GL_TEXTURE_2D, losebar_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLfloat lb_texture[DC_LOSEBAR_TEX_LENGTH_T][DC_LOSEBAR_TEX_LENGTH_S][2];

  // calculate the losebar texture, because I'm better at math than at drawing

  for (int s = DC_LOSEBAR_TEX_LENGTH_S; s--; ) {

    // clamp left and right edges
    if (s == 0 || s == DC_LOSEBAR_TEX_LENGTH_S - 1) {
      for (int t = DC_LOSEBAR_TEX_LENGTH_T; t--; ) {
        lb_texture[t][s][0] = 0.0f;
        lb_texture[t][s][1] = 0.0f;
      }
      continue;
    }

    GLfloat left = s * (1.0f / (GLfloat) DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S);
    for (int t = DC_LOSEBAR_TEX_LENGTH_T; t--; ) {
      GLfloat top = t * (1.0f / (GLfloat) DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S);

      GLfloat lumin = 0.0f;
      GLfloat alpha = 0.0f;

      for (int i = DC_LOSEBAR_ANTIALIAS_LENGTH; i--; ) {
        // runs from 0.0f to 1.0f
        GLfloat x = left + (i + 0.5f) * (1.0f
         / (GLfloat) (DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S
           * DC_LOSEBAR_ANTIALIAS_LENGTH));

        for (int j = DC_LOSEBAR_ANTIALIAS_LENGTH; j--; ) {
          // runs from 0.0f to 2.0f * DC_LOSEBAR_END_RATIO
          GLfloat y = top + (j + 0.5f) * (1.0f
           / (GLfloat) (DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S
           * DC_LOSEBAR_ANTIALIAS_LENGTH));

          GLfloat normal[3];

          // left end
          if (x < DC_LOSEBAR_END_RATIO) {
            GLfloat r_sqrd = (y - DC_LOSEBAR_END_RATIO)
             * (y - DC_LOSEBAR_END_RATIO) + (DC_LOSEBAR_END_RATIO - x)
             * (DC_LOSEBAR_END_RATIO - x);
            if (r_sqrd >= (DC_LOSEBAR_END_RATIO * DC_LOSEBAR_END_RATIO))
              continue;

            normal[0] = sin((DC_LOSEBAR_END_RATIO - x) * (0.5f * PI
             / DC_LOSEBAR_END_RATIO));
            normal[1] = cos(y * (0.5f * PI / DC_LOSEBAR_END_RATIO))
             * cos((DC_LOSEBAR_END_RATIO - x) * (0.5f * PI
             / DC_LOSEBAR_END_RATIO));
            normal[2] = sin(y * (0.5f * PI / DC_LOSEBAR_END_RATIO))
             * cos((DC_LOSEBAR_END_RATIO - x) * (0.5f * PI
             / DC_LOSEBAR_END_RATIO));

          // right end
          } else if (x > (1.0f - DC_LOSEBAR_END_RATIO)) {
            GLfloat r_sqrd = (y - DC_LOSEBAR_END_RATIO)
             * (y - DC_LOSEBAR_END_RATIO) + ((1.0f - DC_LOSEBAR_END_RATIO) - x)
             * ((1.0f - DC_LOSEBAR_END_RATIO) - x);
            if (r_sqrd >= (DC_LOSEBAR_END_RATIO * DC_LOSEBAR_END_RATIO))
              continue;

            normal[0] = sin(((1.0f - DC_LOSEBAR_END_RATIO) - x) * (0.5f * PI
             / DC_LOSEBAR_END_RATIO));
            normal[1] = cos(y * (0.5f * PI / DC_LOSEBAR_END_RATIO))
             * cos(((1.0f - DC_LOSEBAR_END_RATIO) - x) * (0.5f * PI
             / DC_LOSEBAR_END_RATIO));
            normal[2] = sin(y * (0.5f * PI / DC_LOSEBAR_END_RATIO))
             * cos(((1.0f - DC_LOSEBAR_END_RATIO) - x) * (0.5f * PI
             / DC_LOSEBAR_END_RATIO));

          // center
          } else {
            normal[0] = 0.0f;
            normal[1] = cos(y * (0.5f * PI / DC_LOSEBAR_END_RATIO));
            normal[2] = sin(y * (0.5f * PI / DC_LOSEBAR_END_RATIO));
          }

          float light = normal[0] * DC_LOSEBAR_LIGHT_VECTOR_X
           + normal[1] * DC_LOSEBAR_LIGHT_VECTOR_Y
           + normal[2] * DC_LOSEBAR_LIGHT_VECTOR_Z;

          lumin += 1.0f - pow(1.0f - light, DC_LOSEBAR_LIGHT_SPECULAR_POWER);

          alpha += DC_LOSEBAR_LIGHT_AMBIENT_RATIO
           + (1.0f - DC_LOSEBAR_LIGHT_AMBIENT_RATIO) * light;
        }
      }

      lb_texture[t][s][0] = lumin * (1.0f
       / (GLfloat) (DC_LOSEBAR_ANTIALIAS_LENGTH * DC_LOSEBAR_ANTIALIAS_LENGTH));
      lb_texture[t][s][1] = alpha * (1.0f
       / (GLfloat) (DC_LOSEBAR_ANTIALIAS_LENGTH * DC_LOSEBAR_ANTIALIAS_LENGTH));
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, DC_LOSEBAR_TEX_LENGTH_S,
    DC_LOSEBAR_TEX_LENGTH_T, GL_FALSE, GL_LUMINANCE_ALPHA, GL_FLOAT,
    lb_texture);
}
