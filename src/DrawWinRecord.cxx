/*
 * DrawWinRecord.cxx
 * Daniel Nelson - 11/5/0
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
 * Draws the little win record stars.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "WinRecord.h"

void Displayer::drawWinRecord (   )
{
  glBindTexture(GL_TEXTURE_2D, mote_textures[MT_FIVE_POINTED_STAR]);

  int n;
  if (MetaState::mode & CM_SOLO)
    n = DC_SOLO_STAR_ID + 1;
  else
    n = GC_GAMES_PER_MATCH;
  while (n--) {
    Star &star = WinRecord::stars[n];

    glPushMatrix();

      switch (WinRecord::record[n]) {

      // game not yet played
      case GR_NOT_PLAYED:
        glColor3f(DC_STAR_UNPLAYED_RED, DC_STAR_UNPLAYED_GREEN,
         DC_STAR_UNPLAYED_BLUE);
        glTranslatef(DC_STAR_OFFSET_X + DC_STAR_DISPLACEMENT * n,
         DC_STAR_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);
        glScalef(DC_STAR_SIZE_EQUILIBRIUM, -DC_STAR_SIZE_EQUILIBRIUM, 0.0f);
        glCallList(sparkle_list);
        break;

      // game being played
      case GR_BEING_PLAYED:
        glColor3f(DC_STAR_UNPLAYED_RED, DC_STAR_UNPLAYED_GREEN,
         DC_STAR_UNPLAYED_BLUE);
        glTranslatef(DC_STAR_OFFSET_X + DC_STAR_DISPLACEMENT * n,
         DC_STAR_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);
        glRotatef(star.a, 0.0f, 0.0f, 1.0f);
        glScalef(DC_STAR_SIZE_EQUILIBRIUM, -DC_STAR_SIZE_EQUILIBRIUM, 0.0f);
        glCallList(sparkle_list);
        break;

      // game has been lost
      case GR_LOST:
        // star has already faded out
        if (WinRecord::current_game != n)
          glColor4f(DC_STAR_UNPLAYED_RED, DC_STAR_UNPLAYED_GREEN,
           DC_STAR_UNPLAYED_BLUE, DC_STAR_LOST_ALPHA);

        // star is fading out
        else if (Game::time_step < DC_CELEBRATION_TIME) {
          GLfloat fade
           = Game::time_step * (1.0f / (GLfloat) DC_CELEBRATION_TIME);
          fade = Game::sqrt(fade);
          glColor4f(DC_STAR_UNPLAYED_RED, DC_STAR_UNPLAYED_GREEN,
           DC_STAR_UNPLAYED_BLUE, 1.0f - (1.0f - DC_STAR_LOST_ALPHA) * fade);

        // star has already faded out
        } else
          glColor4f(DC_STAR_UNPLAYED_RED, DC_STAR_UNPLAYED_GREEN,
           DC_STAR_UNPLAYED_BLUE, DC_STAR_LOST_ALPHA);

        glTranslatef(DC_STAR_OFFSET_X + DC_STAR_DISPLACEMENT * n,
         DC_STAR_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);
        glRotatef(star.a, 0.0f, 0.0f, 1.0f);
        glScalef(DC_STAR_SIZE_EQUILIBRIUM, -DC_STAR_SIZE_EQUILIBRIUM, 0.0f);
        glCallList(sparkle_list);
       break;

      // game has been won
      case GR_WON:
        // if we need to, draw the old star
        if (WinRecord::draw_old_star && WinRecord::current_game == n) {
          glPushMatrix();
            glColor3f(DC_STAR_UNPLAYED_RED, DC_STAR_UNPLAYED_GREEN,
             DC_STAR_UNPLAYED_BLUE);
            glTranslatef(DC_STAR_OFFSET_X + DC_STAR_DISPLACEMENT * n,
             DC_STAR_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);
            glRotatef(WinRecord::old_star_a, 0.0f, 0.0f, 1.0f);
            glScalef(WinRecord::old_star_size, -WinRecord::old_star_size, 0.0f);
            glCallList(sparkle_list);
          glPopMatrix();
        }

        glColor3f(DC_STAR_WIN_RED, DC_STAR_WIN_GREEN, DC_STAR_WIN_BLUE);

        // star is static but displaced
        if (n == WinRecord::displaced_star)
          if (MetaState::mode & CM_SOLO)
            glTranslatef(DC_STAR_WIN_OFFSET_X, DC_STAR_WIN_SOLO_OFFSET_Y,
             DC_EXTERNAL_OFFSET_Z);
          else
            glTranslatef(DC_STAR_WIN_OFFSET_X, DC_STAR_WIN_OFFSET_Y,
             DC_EXTERNAL_OFFSET_Z);

        // star is in motion
        else if (n == WinRecord::dynamic_star)
          glTranslatef(DC_STAR_OFFSET_X + DC_STAR_DISPLACEMENT * n
           + WinRecord::win_star_x, DC_STAR_OFFSET_Y + WinRecord::win_star_y,
           DC_EXTERNAL_OFFSET_Z);

        // star is at rest in the standard location
        else
          glTranslatef(DC_STAR_OFFSET_X + DC_STAR_DISPLACEMENT * n,
           DC_STAR_OFFSET_Y, DC_EXTERNAL_OFFSET_Z);

        glRotatef(star.a, 0.0f, 0.0f, 1.0f);
        glScalef(star.size, -star.size, 0.0f);
        glCallList(sparkle_list);
        break;
      }

    glPopMatrix();
    
    if (MetaState::mode & CM_SOLO) break;
  }
}
