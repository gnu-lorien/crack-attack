/*
 * sparkle.cxx
 * Daniel Nelson - 9/5/0
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
 * Generates the display list and textures for the little sparkles!
 */

#include <GL/glut.h>
#include <cmath>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"

GLuint Displayer::sparkle_list;
GLuint Displayer::mote_textures[DC_NUMBER_MOTE_TYPES];
GLuint Displayer::spark_texture;

void Displayer::generateSparkleList (   )
{
  GLfloat texture[DC_SPARKLE_TEX_LENGTH][DC_SPARKLE_TEX_LENGTH][2];

  sparkle_list = glGenLists(1);

  glGenTextures(DC_NUMBER_MOTE_TYPES, mote_textures);
  spark_texture = mote_textures[MT_FOUR_POINTED_STAR];

  for (int n = DC_NUMBER_MOTE_TYPES; n--; ) {

    glBindTexture(GL_TEXTURE_2D, mote_textures[n]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
    for (int s = DC_SPARKLE_TEX_LENGTH; s--; )
      for (int t = DC_SPARKLE_TEX_LENGTH; t--; ) {
        float x = s * (2.0f / (float) (DC_SPARKLE_TEX_LENGTH - 1)) - 1.0f;
        float y = t * (2.0f / (float) (DC_SPARKLE_TEX_LENGTH - 1)) - 1.0f;
        float r = sqrt(x * x + y * y);
        float a = atan(y / x);
        float u;
        if (x < 0.0f) a = PI - a;

        switch (n) {
        case MT_FOUR_POINTED_STAR:
          texture[s][t][0] = 0.3f * (1.0f - 2.5f * fabs(x * x - y * y))
           * exp(-r * r) / sqrt(r);
          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(r);
          break;

        case MT_FIVE_POINTED_STAR:
          a *= 5.0f / 4.0f;
          r *= 1.4f;
          texture[s][t][0] = 0.3f * (1.0f - 2.5f
           * fabs(pow(r, 2.2f) * cos(a) * cos(a)
           - pow(r, 2.2f) * sin(a) * sin(a))) * exp(-r * r) / sqrt(r);
          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(r);
          break;

        case MT_SIX_POINTED_STAR:
          a *= 6.0f / 4.0f;
          r *= 1.4f;
          texture[s][t][0] = 0.3f * (1.0f - 2.5f
           * fabs(pow(r, 2.2f) * cos(a) * cos(a)
           - pow(r, 2.2f) * sin(a) * sin(a))) * exp(-r * r) / sqrt(r);
          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(r);
          break;

        case MT_SPECIAL_STAR:
          a *= 8.0f / 4.0f;
          r *= 1.4f;
          texture[s][t][0] = 0.3f * (1.0f - 2.5f
           * fabs(pow(r, 4.2f) * cos(a) * cos(a)
           - pow(r, 4.2f) * sin(a) * sin(a))) * exp(-r * r) / sqrt(r);
          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(r);
          break;

        case MT_MULTIPLIER_ONE_STAR:
          a *= 3.0f / 4.0f;
          r *= 1.9f;
          texture[s][t][0] = 0.3f * (1.0f - 2.5f
           * fabs(pow(r, 1.5f) * cos(a) * cos(a)
           - pow(r, 1.5f) * sin(a) * sin(a))) * exp(-r * r) / sqrt(r);

          if (r < 1.0f) {
            a += PI / 4.0f;
            u = 0.3f * (1.0f - 2.5f * fabs(pow(r, 0.7f) * cos(a) * cos(a)
             - pow(r, 0.7f) * sin(a) * sin(a))) * exp(-0.3f * r * r) / sqrt(r);
            if (u > 0.0f) texture[s][t][0] += u;
            if (texture[s][t][0] > 0.5f)
              texture[s][t][0] *= 0.3f + 0.7f / (0.5f + texture[s][t][0]);
          }

          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(r);
          break;

        case MT_MULTIPLIER_TWO_STAR:
          texture[s][t][0] = 0.3f * (1.0f - 2.5f * fabs(x * x - y * y))
           * exp(-r * r) / sqrt(r);

          a += PI / 4.0f;
          u = 0.3f * (1.0f - 2.5f * fabs(pow(r, 0.7f) * cos(a) * cos(a)
           - pow(r, 0.7f) * sin(a) * sin(a))) * exp(-0.1f * r * r) / sqrt(r);
          if (u > 0.0f) texture[s][t][0] += u;
          if (texture[s][t][0] > 0.5f)
            texture[s][t][0] *= 0.3f + 0.7f / (0.5f + texture[s][t][0]);

          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(r);
          break;

        case MT_MULTIPLIER_THREE_STAR:
          a += PI / 4.0f;
          u = 0.3f * (1.0f - 3.1f * fabs(pow(r, 0.2f) * cos(a) * cos(a)
           - pow(r, 0.2f) * sin(a) * sin(a))) * exp(-0.1f * r * r) / sqrt(r);
          if (u > 0.0f)
            texture[s][t][0] = u;
          else
            texture[s][t][0] = 0.0f;

          texture[s][t][0] += 0.3f * (1.0f - 6.0f * fabs(x * x - y * y))
           * exp(-0.4 * r * r) / sqrt(2.0 * r);
          if (texture[s][t][0] > 0.5f)
            texture[s][t][0] *= 0.3f + 0.7f / (0.5f + texture[s][t][0]);

          texture[s][t][1] = 4.5f * texture[s][t][0] * sqrt(2 * r);

          u = 0.15f * exp(-350.0f * (r - 0.9f) * (r - 0.9f))
           * (1.15f + 0.3f * cos(4.0f * a));

          if (u > texture[s][t][0]) {
            texture[s][t][0] = u;
            texture[s][t][1] = 4.5f * u;
          }
          break;
        }
      }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, DC_SPARKLE_TEX_LENGTH,
     DC_SPARKLE_TEX_LENGTH, GL_FALSE, GL_LUMINANCE_ALPHA, GL_FLOAT, texture);
  }

  glNewList(sparkle_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-DC_SPARKLE_LENGTH, -DC_SPARKLE_LENGTH, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(DC_SPARKLE_LENGTH, -DC_SPARKLE_LENGTH, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-DC_SPARKLE_LENGTH, DC_SPARKLE_LENGTH, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(DC_SPARKLE_LENGTH, DC_SPARKLE_LENGTH, 0.0f);

    glEnd();
  glEndList();
}
