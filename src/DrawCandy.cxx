/*
 * DrawCandy.cxx
 * Daniel Nelson - 9/4/0
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
 * Draws all the sparkles and signs!
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "SparkleManager.h"
#include "SignManager.h"

const GLfloat mote_colors[12][3]
 = { { 1.0f, 0.0f, 0.0f },   // normal
     { 0.9f, 0.4f, 0.0f },   // yellow flare
     { 0.8f, 0.8f, 0.0f },   // orange flare
     { 0.3f, 0.3f, 1.0f },   // blue flare
     { 0.4f, 0.4f, 0.4f },   // gray
     { 0.0f, 0.0f, 0.0f },   // black
     { 0.9f, 0.9f, 0.9f },   // white
     { 0.73f, 0.0f, 0.73f }, // purple
     { 0.2f, 0.2f, 0.8f },   // blue
     { 0.0f, 0.6f, 0.05f },  // green
     { 0.85f, 0.85f, 0.0f }, // yellow
     { 1.0f, 0.4f, 0.0f } }; // orange

GLfloat sign_colors[8][4]
 = { { 1.0f, 1.0f, 1.0f, 0.0f },      // normal
     { 0.2f, 0.2f, 0.2f, 0.0f },      // black
     { 1.0f, 1.0f, 1.0f, 0.0f },      // white
     { 0.933f, 0.75f, 0.933f, 0.0f }, // purple
     { 0.8f, 0.8f, 0.95f, 0.0f },     // blue
     { 0.75f, 0.9f, 0.75f, 0.0f },    // green
     { 0.963f, 0.963f, 0.75f, 0.0f }, // yellow
     { 1.0f, 0.85f, 0.75f, 0.0f } };  // orange

// note that mote_light_colors[0] is hard coded into LightManager.h
const GLfloat Displayer::mote_light_colors[7][3]
 = { { 1.0f, 1.0f, 1.0f },    // normal
     { -1.0f, -1.0f, -1.0f }, // black
     { 0.8f, 0.0f, 0.8f },    // purple
     { 0.0f, 0.0f, 1.0f },    // blue
     { 0.0f, 1.0f, 0.0f },    // green
     { 0.8f, 0.8f, 0.0f },    // yellow
     { 1.0f, 0.7f, 0.0f } };  // orange

inline void Displayer::drawSign ( Sign &sign, int texture )
{
  glPushMatrix();

    glTranslatef(sign.x, sign.y, DC_PLAY_OFFSET_Z);

    // first hold
    if (sign.life_time < DC_SIGN_HOLD_TIME) {
      sign_colors[sign.color][3] = DC_SIGN_ALPHA;    

    // then fade, grow, and float
    } else {
      GLfloat fade = (DC_SIGN_LIFE_TIME - sign.life_time)
       * (1.0f / (GLfloat) DC_SIGN_FADE_TIME);
      sign_colors[sign.color][3] = DC_SIGN_ALPHA * fade * fade;

      GLfloat size = 1.0f
       + (DC_FINAL_INFLATE_SIZE - 1.0f) * (1.0f - fade) * (1.0f - fade);
      glScalef(size, size, 1.0f);
    }

    glColor4fv(sign_colors[sign.color]);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();

      glTranslatef(sign.subtexture_t, sign.subtexture_s, 0.0f);

      if (texture == ST_SMALL_TEXTURE)
        glCallList(sign_small_list);
      else
        glCallList(sign_large_list);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

  glPopMatrix();
}

void Displayer::drawCandy (   )
{
  glBindTexture(GL_TEXTURE_2D, spark_texture);

  int c = SparkleManager::spark_count;
  for (int n = 0; c; n++)
    if (SparkleManager::sparks[n].active) {
      Spark &spark = SparkleManager::sparks[n];
      c--;

      glPushMatrix();

        if (spark.life_time < DC_SPARK_FADE_TIME)
          glColor4f(block_colors[spark.color][0],
           block_colors[spark.color][1], block_colors[spark.color][2],
           spark.life_time * (1.0f / (GLfloat) DC_SPARK_FADE_TIME));

        else if (spark.life_time < DC_SPARK_FADE_TIME
         + DC_SPARK_PULSE_TIME) {
          GLfloat pulse = (spark.life_time - DC_SPARK_FADE_TIME)
           * (2.0f / (GLfloat) DC_SPARK_PULSE_TIME);
          if (pulse > 1.0f) pulse = 2.0f - pulse;

          glColor3f(pulse + (1.0f - pulse) * block_colors[spark.color][0],
           pulse + (1.0f - pulse) * block_colors[spark.color][1],
           pulse + (1.0f - pulse) * block_colors[spark.color][2]);

        } else
          glColor3fv(block_colors[spark.color]);

        glTranslatef(spark.x, spark.y, DC_PLAY_OFFSET_Z);
        glRotatef(spark.a, 0.0f, 0.0f, 1.0f);
        if (spark.size != 1.0f)
          glScalef(spark.size, spark.size, 1.0f);

        glCallList(sparkle_list);

      glPopMatrix();
    }

  c = SparkleManager::mote_count;
  GLuint last_type = MT_FOUR_POINTED_STAR;
  for (int n = 0; c; n++)
    if (SparkleManager::motes[n].active) {
      Mote &mote = SparkleManager::motes[n];
      c--;

      glPushMatrix();

        if (mote_textures[mote.type] != last_type) {
          glBindTexture(GL_TEXTURE_2D, mote_textures[mote.type]);
          last_type = mote_textures[mote.type];
        }

        // if an abnormal color
        if (mote.color > 0 && mote.color < DC_FIRST_SPECIAL_MOTE_COLOR)

          // fade in as color 0
          if (mote.life_time >= 0 && mote.life_time < GC_DYING_DELAY)
            glColor4f(mote_colors[0][0],
             mote_colors[0][1], mote_colors[0][2],
             mote.life_time * (1.0f / (GLfloat) GC_DYING_DELAY));

          // later fade to our color
          else if (mote.life_time > -DC_MOTE_COLOR_FADE_TIME) {
            GLfloat fade
             = -mote.life_time * (1.0f / (GLfloat) DC_MOTE_COLOR_FADE_TIME);
            glColor3f((1.0f - fade) * mote_colors[0][0]
             + fade * mote_colors[mote.color][0],
             (1.0f - fade) * mote_colors[0][1]
             + fade * mote_colors[mote.color][1],
             (1.0f - fade) * mote_colors[0][2]
             + fade * mote_colors[mote.color][2]);

          // now use our color
          } else
            glColor3fv(mote_colors[mote.color]);

        // if normal color and new, fade in
        else if (mote.life_time >= 0 && mote.life_time < GC_DYING_DELAY)
          glColor4f(mote_colors[mote.color][0],
           mote_colors[mote.color][1], mote_colors[mote.color][2],
           mote.life_time * (1.0f / (GLfloat) GC_DYING_DELAY));

        // otherwise, nothing special
        else
          glColor3fv(mote_colors[mote.color]);

        glTranslatef(mote.x, mote.y, DC_PLAY_OFFSET_Z);
        glRotatef(mote.a, 0.0f, 0.0f, 1.0f);
        glScalef(mote.size, mote.size, 1.0f);

        glCallList(sparkle_list);

      glPopMatrix();
    }

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D, sign_large_texture);
  
  c = SignManager::sign_count;
  for (int n = 0; c; n++)
    if (SignManager::signs[n].active) {
      Sign &sign = SignManager::signs[n];
      c--;

      if (sign.texture == ST_SMALL_TEXTURE) continue;

      drawSign(sign, ST_LARGE_TEXTURE);
    }

  glBindTexture(GL_TEXTURE_2D, sign_small_texture);
  
  c = SignManager::sign_count;
  for (int n = 0; c; n++)
    if (SignManager::signs[n].active) {
      Sign &sign = SignManager::signs[n];
      c--;

      if (sign.texture == ST_LARGE_TEXTURE) continue;

      drawSign(sign, ST_SMALL_TEXTURE);
    }
}
