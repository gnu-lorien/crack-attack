/*
 * DrawExternalCandy.cxx
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
 * Draws everything dumb outside of the play area.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "MetaState.h"
#include "Clock.h"
#include "Score.h"
#include "LoseBar.h"

inline void Displayer::drawDigit ( GLfloat alpha )
{
  glColor4f(0.3f, 0.3f, 1.0f, alpha);

  glBegin(GL_TRIANGLE_STRIP);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-DC_CLOCK_DIGIT_LENGTH, -DC_CLOCK_DIGIT_LENGTH, 0.0f);
    glColor4f(0.5f, 0.5f, 1.0f, alpha);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(DC_CLOCK_DIGIT_LENGTH, -DC_CLOCK_DIGIT_LENGTH, 0.0f);
    glColor4f(0.5f, 0.5f, 1.0f, alpha);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-DC_CLOCK_DIGIT_LENGTH, DC_CLOCK_DIGIT_LENGTH, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(DC_CLOCK_DIGIT_LENGTH, DC_CLOCK_DIGIT_LENGTH, 0.0f);

  glEnd();
}

inline void Displayer::drawLeftBar ( GLfloat b, GLfloat t, GLfloat color[3] )
{
  if (t < DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f) return;

  glBegin(GL_TRIANGLE_STRIP);

    glColor3fv(color);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-DC_LOSEBAR_LENGTH / 2.0f, DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);
    glTexCoord2f(t - (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 0.0f);
    glVertex3f(b - (DC_LOSEBAR_FADE_LENGTH / 2.0f), DC_LOSEBAR_HEIGHT / 2,
     0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-DC_LOSEBAR_LENGTH / 2.0f, -DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);
    glTexCoord2f(t - (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 1.0f);
    glVertex3f(b - (DC_LOSEBAR_FADE_LENGTH / 2.0f), -DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);

  glEnd();
}

inline void Displayer::drawRightBar ( GLfloat b, GLfloat t, GLfloat color[3] )
{
  if (t > 1.0f - DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f) return;

  glBegin(GL_TRIANGLE_STRIP);

    glColor3fv(color);
    glTexCoord2f(t + (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 0.0f);
    glVertex3f(b + (DC_LOSEBAR_FADE_LENGTH / 2.0f), DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(DC_LOSEBAR_LENGTH / 2.0f, DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);
    glTexCoord2f(t + (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 1.0f);
    glVertex3f(b + (DC_LOSEBAR_FADE_LENGTH / 2.0f), -DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(DC_LOSEBAR_LENGTH / 2.0f, -DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);

  glEnd();
}

inline void Displayer::drawCenterBar ( GLfloat b, GLfloat t, GLfloat color1[3],
 GLfloat color2[3] )
{
  glBegin(GL_TRIANGLE_STRIP);

    glColor3fv(color1);
    glTexCoord2f(t - (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 0.0f);
    glVertex3f(b - (DC_LOSEBAR_FADE_LENGTH / 2.0f), DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);
    glColor3fv(color2);
    glTexCoord2f(t + (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 0.0f);
    glVertex3f(b + (DC_LOSEBAR_FADE_LENGTH / 2.0f), DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);
    glColor3fv(color1);
    glTexCoord2f(t - (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 1.0f);
    glVertex3f(b - (DC_LOSEBAR_FADE_LENGTH / 2.0f), -DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);
    glColor3fv(color2);
    glTexCoord2f(t + (DC_LOSEBAR_FADE_TEX_LENGTH / 2.0f), 1.0f);
    glVertex3f(b + (DC_LOSEBAR_FADE_LENGTH / 2.0f), -DC_LOSEBAR_HEIGHT / 2.0f,
     0.0f);

  glEnd();
}

inline void Displayer::drawFullBar ( GLfloat color[3] )
{
  glBegin(GL_TRIANGLE_STRIP);

    glColor3fv(color);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-DC_LOSEBAR_LENGTH / 2.0f, DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(DC_LOSEBAR_LENGTH / 2.0f, DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-DC_LOSEBAR_LENGTH / 2.0f, -DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(DC_LOSEBAR_LENGTH / 2.0f, -DC_LOSEBAR_HEIGHT / 2.0f, 0.0f);

  glEnd();
}

void Displayer::drawExternalCandy (   )
{
  int texture_bound;

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

  GLfloat b = (DC_LOSEBAR_LENGTH + DC_LOSEBAR_FADE_LENGTH)
   * (LoseBar::bar - 0.5f);
  GLfloat t = LoseBar::bar * (1.0f + DC_LOSEBAR_FADE_TEX_LENGTH)
   - 0.5f * DC_LOSEBAR_FADE_TEX_LENGTH;

  glBindTexture(GL_TEXTURE_2D, losebar_texture);

  glPushMatrix();

    glTranslatef(DC_LEFT_EXTERNAL_CENTER, DC_LOSEBAR_OFFSET_Y,
     DC_EXTERNAL_OFFSET_Z);

    GLfloat color1[3], color2[3];

    float fade;
    switch (LoseBar::state) {
    case LB_INACTIVE:
      color1[0] = DC_LOSEBAR_INACTIVE_RED;
      color1[1] = DC_LOSEBAR_INACTIVE_GREEN;
      color1[2] = DC_LOSEBAR_INACTIVE_BLUE;

      drawFullBar(color1);
      break;

    case LB_LOW_ALERT:
      color1[0] = DC_LOSEBAR_LOW_ALERT_RED;
      color1[1] = DC_LOSEBAR_LOW_ALERT_GREEN;
      color1[2] = DC_LOSEBAR_LOW_ALERT_BLUE;

      color2[0] = DC_LOSEBAR_INACTIVE_RED;
      color2[1] = DC_LOSEBAR_INACTIVE_GREEN;
      color2[2] = DC_LOSEBAR_INACTIVE_BLUE;

      drawLeftBar(b, t, color1);
      drawCenterBar(b, t, color1, color2);
      drawRightBar(b, t, color2);
      break;

    case LB_HIGH_ALERT:
      color1[0] = DC_LOSEBAR_HIGH_ALERT_RED;
      color1[1] = DC_LOSEBAR_HIGH_ALERT_GREEN;
      color1[2] = DC_LOSEBAR_HIGH_ALERT_BLUE;

      color2[0] = DC_LOSEBAR_LOW_ALERT_RED;
      color2[1] = DC_LOSEBAR_LOW_ALERT_GREEN;
      color2[2] = DC_LOSEBAR_LOW_ALERT_BLUE;

      drawLeftBar(b, t, color1);
      drawCenterBar(b, t, color1, color2);
      drawRightBar(b, t, color2);
      break;

    case LB_FADE_LOW_TO_INACTIVE:
      fade = LoseBar::fade_timer * (1.0f / (GLfloat) DC_LOSEBAR_FADE_TIME);

      color1[0] = DC_LOSEBAR_INACTIVE_RED
       + fade * (DC_LOSEBAR_LOW_ALERT_RED - DC_LOSEBAR_INACTIVE_RED);
      color1[1] = DC_LOSEBAR_INACTIVE_GREEN
       + fade * (DC_LOSEBAR_LOW_ALERT_GREEN - DC_LOSEBAR_INACTIVE_GREEN);
      color1[2] = DC_LOSEBAR_INACTIVE_BLUE
       + fade * (DC_LOSEBAR_LOW_ALERT_BLUE - DC_LOSEBAR_INACTIVE_BLUE);

      color2[0] = DC_LOSEBAR_INACTIVE_RED;
      color2[1] = DC_LOSEBAR_INACTIVE_GREEN;
      color2[2] = DC_LOSEBAR_INACTIVE_BLUE;

      drawLeftBar(b, t, color1);
      drawCenterBar(b, t, color1, color2);
      drawRightBar(b, t, color2);
      break;

    case LB_FADE_HIGH_TO_INACTIVE:
      fade = LoseBar::fade_timer * (1.0f / (GLfloat) DC_LOSEBAR_FADE_TIME);

      color1[0] = DC_LOSEBAR_INACTIVE_RED
       + fade * (DC_LOSEBAR_HIGH_ALERT_RED - DC_LOSEBAR_INACTIVE_RED);
      color1[1] = DC_LOSEBAR_INACTIVE_GREEN
       + fade * (DC_LOSEBAR_HIGH_ALERT_GREEN - DC_LOSEBAR_INACTIVE_GREEN);
      color1[2] = DC_LOSEBAR_INACTIVE_BLUE
       + fade * (DC_LOSEBAR_HIGH_ALERT_BLUE - DC_LOSEBAR_INACTIVE_BLUE);

      color2[0] = DC_LOSEBAR_INACTIVE_RED
       + fade * (DC_LOSEBAR_LOW_ALERT_RED - DC_LOSEBAR_INACTIVE_RED);
      color2[1] = DC_LOSEBAR_INACTIVE_GREEN
       + fade * (DC_LOSEBAR_LOW_ALERT_GREEN - DC_LOSEBAR_INACTIVE_GREEN);
      color2[2] = DC_LOSEBAR_INACTIVE_BLUE
       + fade * (DC_LOSEBAR_LOW_ALERT_BLUE - DC_LOSEBAR_INACTIVE_BLUE);

      drawLeftBar(b, t, color1);
      drawCenterBar(b, t, color1, color2);
      drawRightBar(b, t, color2);
      break;

    case LB_FADE_RESET_HIGH:
      fade = LoseBar::fade_timer * (1.0f / (GLfloat) DC_LOSEBAR_FADE_TIME);

      color1[0] = DC_LOSEBAR_LOW_ALERT_RED
       + fade * (DC_LOSEBAR_HIGH_ALERT_RED - DC_LOSEBAR_LOW_ALERT_RED);
      color1[1] = DC_LOSEBAR_LOW_ALERT_GREEN
       + fade * (DC_LOSEBAR_HIGH_ALERT_GREEN - DC_LOSEBAR_LOW_ALERT_GREEN);
      color1[2] = DC_LOSEBAR_LOW_ALERT_BLUE
       + fade * (DC_LOSEBAR_HIGH_ALERT_BLUE - DC_LOSEBAR_LOW_ALERT_BLUE);

      color2[0] = DC_LOSEBAR_LOW_ALERT_RED;
      color2[1] = DC_LOSEBAR_LOW_ALERT_GREEN;
      color2[2] = DC_LOSEBAR_LOW_ALERT_BLUE;

      drawLeftBar(b, t, color1);
      drawCenterBar(b, t, color1, color2);
      drawRightBar(b, t, color2);
      break;
    }

  glPopMatrix();

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // draw clock
  if (MetaState::mode & (CM_SERVER | CM_CLIENT)) {

    glPushMatrix();

      glTranslatef(DC_LEFT_EXTERNAL_CENTER - DC_CLOCK_DIGIT_OFFSET / 2,
       DC_CLOCK_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);

      glBindTexture(GL_TEXTURE_2D, clock_digit_textures[10]);
      drawDigit(1.0f);
      
      glTranslatef(DC_CLOCK_CENTER_OFFSET / 2.0f
       + 3.0f * DC_CLOCK_DIGIT_OFFSET / 2.0f, 0.0f, 0.0f);

      GLfloat alpha = Clock::tick * (1.0f / (GLfloat) GC_STEPS_PER_SECOND);

      glBindTexture(GL_TEXTURE_2D, clock_digit_textures[Clock::digits[0]]);
      if (Clock::digits[0] != Clock::previous_digits[0]) {
        drawDigit(alpha);
        glBindTexture(GL_TEXTURE_2D,
         clock_digit_textures[Clock::previous_digits[0]]);
        drawDigit(1.0f - alpha);
        texture_bound = Clock::previous_digits[0];
      } else {
        drawDigit(1.0f);
        texture_bound = Clock::digits[0];
      }

      glTranslatef(-DC_CLOCK_DIGIT_OFFSET, 0.0f, 0.0f);

      if (Clock::digits[1] != texture_bound)
        glBindTexture(GL_TEXTURE_2D, clock_digit_textures[Clock::digits[1]]);
      if (Clock::digits[1] == Clock::previous_digits[1]) {
        drawDigit(1.0f);
        texture_bound = Clock::digits[1];
      } else {
        drawDigit(alpha);
        glBindTexture(GL_TEXTURE_2D,
         clock_digit_textures[Clock::previous_digits[1]]);
        drawDigit(1.0f - alpha);
        texture_bound = Clock::previous_digits[1];
      }

      glTranslatef(-DC_CLOCK_DIGIT_OFFSET - DC_CLOCK_CENTER_OFFSET, 0.0f, 0.0f);

      if (Clock::digits[2] != texture_bound)
        glBindTexture(GL_TEXTURE_2D, clock_digit_textures[Clock::digits[2]]);
      if (Clock::digits[2] == Clock::previous_digits[2]) {
        drawDigit(1.0f);
        texture_bound = Clock::digits[2];
      } else {
        drawDigit(alpha);
        glBindTexture(GL_TEXTURE_2D,
         clock_digit_textures[Clock::previous_digits[2]]);
        drawDigit(1.0f - alpha);
        texture_bound = Clock::previous_digits[2];
      }

      if (Clock::digits[3] || Clock::previous_digits[3]) {
        glTranslatef(-DC_CLOCK_DIGIT_OFFSET, 0.0f, 0.0f);

        if (Clock::digits[3] == Clock::previous_digits[3]) {
          if (texture_bound != Clock::digits[3])
            glBindTexture(GL_TEXTURE_2D,
             clock_digit_textures[Clock::digits[3]]);
          drawDigit(1.0f);
        } else {
          if (Clock::digits[3]) {
            if (texture_bound != Clock::digits[3])
              glBindTexture(GL_TEXTURE_2D,
               clock_digit_textures[Clock::digits[3]]);
            drawDigit(alpha);
            texture_bound = Clock::digits[3];
          }
          if (Clock::previous_digits[3]) {
            if (texture_bound != Clock::previous_digits[3])
              glBindTexture(GL_TEXTURE_2D,
               clock_digit_textures[Clock::previous_digits[3]]);
            drawDigit(1.0f - alpha);
          }
        }
      }

    glPopMatrix();

  // draw score
  } else {

    glPushMatrix();

      glTranslatef(DC_LEFT_EXTERNAL_CENTER + ((Score::n_digits_displayed - 1)
       * DC_CLOCK_DIGIT_OFFSET) / 2, DC_CLOCK_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);

      GLfloat alpha = 1.0f - (GLfloat) Score::fade_timer
       * Score::inverse_timer_start;

      glBindTexture(GL_TEXTURE_2D,
       clock_digit_textures[Score::digits[0]]);
      texture_bound = Score::digits[0];
      drawDigit(alpha);
      if (Score::fade_timer && Score::previous_digits[0]
       != Score::digits[0]) {
        glBindTexture(GL_TEXTURE_2D,
         clock_digit_textures[Score::previous_digits[0]]);
        texture_bound = Score::previous_digits[0];
        drawDigit(1.0f - alpha);
      }

      for (int n = 1; n < Score::n_digits_displayed; n++) {
        glTranslatef(-DC_CLOCK_DIGIT_OFFSET, 0.0f, 0.0f);
        if (texture_bound != Score::digits[n]) {
          glBindTexture(GL_TEXTURE_2D, clock_digit_textures[Score::digits[n]]);
          texture_bound = Score::digits[n];
        }
        if (Score::fade_timer && Score::previous_digits[n]
         != Score::digits[n]) {
          drawDigit(alpha);
          glBindTexture(GL_TEXTURE_2D,
           clock_digit_textures[Score::previous_digits[n]]);
          texture_bound = Score::previous_digits[n];
          drawDigit(1.0f - alpha);
        } else
          drawDigit(1.0f);
      }

    glPopMatrix();
  }

  glDisable(GL_BLEND);

  // draw logo

  glBindTexture(GL_TEXTURE_2D, logo_texture);
  glCallList(logo_list);

  // draw names

  glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, name_texture);
    glTranslatef(DC_LEFT_EXTERNAL_CENTER, DC_NAME_OFFSET_Y,
     DC_EXTERNAL_OFFSET_Z);
    glScalef(DC_NAME_SCALE, DC_NAME_SCALE, 1.0f);
    glCallList(message_2x1_list);
  glPopMatrix();
}
