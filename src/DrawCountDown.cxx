/*
 * DrawCountDown.cxx
 * Daniel Nelson - 10/27/0
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
 * Draws the game start count down.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "CountDownManager.h"
#include "MessageManager.h"

void Displayer::drawCountDown_inline_split_ (   )
{
  glBindTexture(GL_TEXTURE_2D, message_texture);

  glPushMatrix();

    GLfloat lambda = CountDownManager::message_switch_alarm
     * (1.0f / (GLfloat) (GC_START_PAUSE_DELAY / 3));

    if (CountDownManager::state != 0) {
      glColor4f(1.0f, 1.0f, 1.0f, lambda);

      glTranslatef(0.0f, lambda * lambda * (DC_GRID_ELEMENT_LENGTH
       * GC_SAFE_HEIGHT * (1.0f - DC_COUNT_DOWN_HEIGHT_RATIO))
       + (DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH * GC_SAFE_HEIGHT
       * DC_COUNT_DOWN_HEIGHT_RATIO), DC_PLAY_OFFSET_Z);

      glScalef((1.0f - lambda) * (1.0f - lambda)
       * (DC_COUNT_DOWN_MAX_SCALE - 1.0f) + 1.0f,
       (1.0f - lambda) * (1.0f - lambda)
       * (DC_COUNT_DOWN_MAX_SCALE - 1.0f) + 1.0f, 1.0f);

    } else {
      if (lambda > DC_COUNT_DOWN_GO_INFLECTION)
        lambda = 1.0f;
      else
        lambda = Game::sqrt(lambda * (1.0f / DC_COUNT_DOWN_GO_INFLECTION));

      glColor4f(1.0f, 1.0f, 1.0f, lambda);

      glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH
       * GC_SAFE_HEIGHT * 0.5f, DC_PLAY_OFFSET_Z);

      glScalef(DC_COUNT_DOWN_MAX_SCALE, DC_COUNT_DOWN_MAX_SCALE, 1.0f);
    }

  if (MessageManager::message_shape & MS_1x1)
    glCallList(message_1x1_list);
  else if (MessageManager::message_shape & MS_2x1)
    glCallList(message_2x1_list);
  else
    glCallList(message_4x1_list);


  glPopMatrix();
}
