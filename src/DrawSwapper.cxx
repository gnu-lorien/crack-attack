/*
 * DrawSwapper.cxx
 * Daniel Nelson - 9/30/0
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
 * Draws the swapper thing.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "Swapper.h"
#include "LightManager.h"
#include "X.h"

const GLfloat swapper_colors[4][3]
 = { { 1.0f, 1.0f, 1.0f },         // normal
     { 0.85f, 0.85f, 0.0f },       // reverse control
     { 0.0f, 0.6f, 0.05f },        // invisible swapper
     { 0.425f, 0.725f, 0.025f } }; // both

void Displayer::drawSwapper (   )
{
  // blocks have already been drawn, so we use their material calls

  if (!X::invisibleSwapper())
    glColor3fv(swapper_colors[Swapper::color]);

  else {
    if (!X::needDrawSwapper()) return;

    glEnable(GL_BLEND);
    glColor4f(swapper_colors[Swapper::color][0],
     swapper_colors[Swapper::color][1], swapper_colors[Swapper::color][2],
     X::swapperAlpha());
  }

  glPushMatrix();

    GLfloat x = Swapper::x * DC_GRID_ELEMENT_LENGTH
     + (DC_PLAY_OFFSET_X + 0.5f * DC_GRID_ELEMENT_LENGTH);
    GLfloat y = Swapper::y * DC_GRID_ELEMENT_LENGTH + play_offset_y;

    LightManager::setupGarbageLights(x - (0.5f * DC_GRID_ELEMENT_LENGTH), y,
     1, 2);

    if (Swapper::state & SS_SWAPPING) {

      glTranslatef(x, y, DC_PLAY_OFFSET_Z);
      glRotatef(180.0f * Swapper::swap_factor, 0.0f, 1.0f, 0.0f);

      glPushMatrix();

        glTranslatef(-DC_SWAPPER_GRAB_LENGTH, DC_SWAPPER_GRAB_LENGTH, 0.0f);
        glCallList(swapper_list);

        glTranslatef(2.0f * DC_SWAPPER_GRAB_LENGTH,
         -2.0f * DC_SWAPPER_GRAB_LENGTH, 0.0f);
        glScalef(-1.0f, -1.0f, 1.0f);
        glCallList(swapper_list);

        glTranslatef(2.0f * DC_SWAPPER_GRAB_LENGTH, 0.0f, 0.0f);
        glScalef(-1.0f, 1.0f, 1.0f);
        glCallList(swapper_list);

        glTranslatef(2.0f * DC_SWAPPER_GRAB_LENGTH,
         -2.0f * DC_SWAPPER_GRAB_LENGTH, 0.0f);
        glScalef(-1.0f, -1.0f, 1.0f);
        glCallList(swapper_list);

      glPopMatrix();

      glScalef(1.0f, 1.0f, -1.0f);

      glTranslatef(-DC_SWAPPER_GRAB_LENGTH, DC_SWAPPER_GRAB_LENGTH, 0.0f);
      glCallList(swapper_list);

      glTranslatef(2.0f * DC_SWAPPER_GRAB_LENGTH,
       -2.0f * DC_SWAPPER_GRAB_LENGTH, 0.0f);
      glScalef(-1.0f, -1.0f, 1.0f);
      glCallList(swapper_list);

      glTranslatef(2.0f * DC_SWAPPER_GRAB_LENGTH, 0.0f, 0.0f);
      glScalef(-1.0f, 1.0f, 1.0f);
      glCallList(swapper_list);

      glTranslatef(2.0f * DC_SWAPPER_GRAB_LENGTH,
       -2.0f * DC_SWAPPER_GRAB_LENGTH, 0.0f);
      glScalef(-1.0f, -1.0f, 1.0f);
      glCallList(swapper_list);

      glPopMatrix();

      return;
    }

    if (Swapper::state & SS_MOVE_PAUSE) {
      GLfloat shift = (Game::time_step - Swapper::move_pause_alarm)
       * (1.0f / (GLfloat) GC_MOVE_DELAY);
      shift *= shift;

      if ((Swapper::state & SS_MOVE_MASK) & SS_MOVE_LEFT)
        x += shift;
      else if ((Swapper::state & SS_MOVE_MASK) & SS_MOVE_RIGHT)
        x -= shift;
      else if ((Swapper::state & SS_MOVE_MASK) & SS_MOVE_UP)
        y -= shift;
      else
        y += shift;
    }

    glTranslatef(x, y, DC_PLAY_OFFSET_Z);

    glCallList(swapper_list);

    glScalef(-1.0f, -1.0f, 1.0f);
    glCallList(swapper_list);

    glScalef(-1.0f, 1.0f, 1.0f);
    glCallList(swapper_list);

    glScalef(-1.0f, -1.0f, 1.0f);
    glCallList(swapper_list);

  glPopMatrix();

  if (X::invisibleSwapper())
    glDisable(GL_BLEND);
}
