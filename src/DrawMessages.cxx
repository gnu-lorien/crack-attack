/*
 * DrawMessages.cxx
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
 * Draws various messages.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "Communicator.h"
#include "CelebrationManager.h"
#include "MessageManager.h"
#include "WinRecord.h"

void Displayer::readyMessage ( int message )
/*
 * Ready the message texture for displaying.  Note that messages and the count
 * down use the same texture.  Thus, both can not be active at once.
 */
{
  glGenTextures(1, &message_texture);

  glBindTexture(GL_TEXTURE_2D, message_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, message_width[message],
   message_height[message], GL_FALSE, GL_ALPHA, GL_UNSIGNED_BYTE,
   message_texture_data[message]);
}

void Displayer::nextMessage ( int message )
/*
 * Replace the current message texture.  Note that it is assumed that the
 * caller knows what's going on as far as message texture sizes.  Thus, changing
 * the message texture sizes may break code.
 */
{
  glBindTexture(GL_TEXTURE_2D, message_texture);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, message_width[message],
   message_height[message], GL_ALPHA, GL_UNSIGNED_BYTE,
   message_texture_data[message]);
}

void Displayer::freeMessage (   )
/*
 * Deallocate the message texture.
 */
{
  glDeleteTextures(1, &message_texture);
}

void Displayer::drawMessage (   )
{
  glBindTexture(GL_TEXTURE_2D, message_texture);

  glPushMatrix();

    // normal message
    if (MessageManager::mode & MM_NORMAL) {
     
      glColor4f(1.0f, 1.0f, 1.0f, message_alpha[MessageManager::time_step]);

      if ((MetaState::mode & CM_SOLO) && (MetaState::state & MS_BOTH_KEY_WAIT))
        glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH
         * 2.0f * GC_SAFE_HEIGHT / 3.0f, DC_PLAY_OFFSET_Z);
      else
        glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH
         * GC_SAFE_HEIGHT / 2.0f, DC_PLAY_OFFSET_Z);

      glScalef(DC_MESSAGE_SCALE, DC_MESSAGE_SCALE, 1.0f);

      if (MessageManager::message_shape & MS_1x1)
        glCallList(message_1x1_list);
      else if (MessageManager::message_shape & MS_2x1)
        glCallList(message_2x1_list);
      else
        glCallList(message_4x1_list);

    // win/loss message
    } else {
      // if we won
      if (WinRecord::won) {

        GLfloat color[4];
        color[0] = DC_WIN_MESSAGE_RED;
        color[1] = DC_WIN_MESSAGE_GREEN;
        color[2] = DC_WIN_MESSAGE_BLUE;
        if (CelebrationManager::win_alpha == 1.0f)
          color[3] = 1.0f;
        else {
          color[3]
           = CelebrationManager::win_alpha * CelebrationManager::win_alpha;
          color[3] *= color[3];
        }

        if (CelebrationManager::win_flash1) {
          GLfloat flash = CelebrationManager::win_flash1
           * (2.0f / (GLfloat) DC_WIN_FLASH_1_TIME);
          if (flash > 1.0f) flash = 2.0f - flash;
          flash *= flash;
          color[1] += (1.0f - DC_WIN_MESSAGE_GREEN) * flash;
          color[2] += (1.0f - DC_WIN_MESSAGE_BLUE) * flash;
        }

        if (CelebrationManager::win_flash2) {
          GLfloat flash = CelebrationManager::win_flash2
           * (2.0f / (GLfloat) DC_WIN_FLASH_2_TIME);
          if (flash > 1.0f) flash = 2.0f - flash;
          flash *= flash;
          color[1] -= (1.0f - DC_WIN_MESSAGE_GREEN) * flash;
          color[2] += (1.0f - DC_WIN_MESSAGE_BLUE) * flash;
        }

        glColor4fv(color);
      
        if (MetaState::mode & CM_SOLO)
          glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH
           * 0.75f * GC_SAFE_HEIGHT, DC_PLAY_OFFSET_Z);
        else
          glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH
           * GC_SAFE_HEIGHT / 2.0f, DC_PLAY_OFFSET_Z);

        glScalef(CelebrationManager::win_scale * DC_MESSAGE_SCALE,
         CelebrationManager::win_scale * DC_MESSAGE_SCALE, 1.0f);

        glCallList(message_1x1_list);

      // if we lost
      } else {
        glColor3f(DC_LOSS_MESSAGE_RED, DC_LOSS_MESSAGE_GREEN,
         DC_LOSS_MESSAGE_BLUE);
        
        glTranslatef(0.0f, CelebrationManager::loss_height + (DC_PLAY_OFFSET_Y
         + DC_GRID_ELEMENT_LENGTH * GC_SAFE_HEIGHT / 2.0f), DC_PLAY_OFFSET_Z);

        if (!CelebrationManager::loss_rescale)
          glScalef(DC_MESSAGE_SCALE, DC_MESSAGE_SCALE, 1.0f);
        else
          glScalef(DC_MESSAGE_SCALE * DC_GAME_OVER_SCALE_RATIO,
           DC_MESSAGE_SCALE * DC_GAME_OVER_SCALE_RATIO, 1.0f);

        glCallList(message_1x1_list);
      }
    }

  glPopMatrix();
}
