/*
 * DrawScoreRecord.cxx
 * Daniel Nelson - 12/7/1
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
 * Draws the messages associated with the score record for solo play.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "ScoreRecordManager.h"
#include "Score.h"
#include "Sine.h"

void Displayer::drawScoreToBeatMessage_inline_split_ (   )
{
  if (!(MetaState::mode & CM_SOLO) || !(MetaState::state & MS_BOTH_KEY_WAIT))
    return;

  glBindTexture(GL_TEXTURE_2D, score_to_beat_texture);

  glPushMatrix();

    glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_GRID_ELEMENT_LENGTH
     * 0.4f * GC_SAFE_HEIGHT, DC_PLAY_OFFSET_Z);

    glScalef(DC_SCORE_TO_BEAT_SCALE, DC_SCORE_TO_BEAT_SCALE, 1.0f);

    glCallList(message_2x1_list);

  glPopMatrix();
}

void Displayer::drawScoreRecord_inline_split_ (   )
{
  if (!(MetaState::mode & CM_SOLO)
   || !(MetaState::state & MS_GAME_OVER_KEY_WAIT) || !WinRecord::won)
    return;

  if (Game::time_step < DC_WIN_FADE_TIME) return;

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  glPushMatrix();

    glTranslatef(0.0f, DC_PLAY_OFFSET_Y + DC_SCORE_REC_Y_OFFSET,
     DC_PLAY_OFFSET_Z);
         
    glScalef(DC_SCORE_REC_SCALE_X, DC_SCORE_REC_SCALE_Y, 1.0f);

    for (int n = 0; n < DC_SCORE_REC_NUMBER_DRAW
     && n <= ScoreRecordManager::top_rank; n++) {
      if (ScoreRecordManager::top_rank - n >= GC_SCORE_REC_LENGTH) continue;

      GLfloat y = (((DC_SCORE_REC_NUMBER_DRAW - 1) / 2.0f) - n)
       * DC_SCORE_REC_RANK_HEIGHT - ScoreRecordManager::offset;

      GLfloat yt = (y + DC_SCORE_REC_RANK_DRAW_HEIGHT / 2.0f)
       * (PI / (2.0f * (DC_SCORE_REC_NUMBER_DRAW / 2.0f)
       * DC_SCORE_REC_RANK_HEIGHT));
      GLfloat yb = (y - DC_SCORE_REC_RANK_DRAW_HEIGHT / 2.0f)
       * (PI / (2.0f * (DC_SCORE_REC_NUMBER_DRAW / 2.0f)
       * DC_SCORE_REC_RANK_HEIGHT));

      GLfloat yt_circle = Sine::sin(yt);
      GLfloat yb_circle = Sine::sin(yb);

      int texture = ScoreRecordManager::top_texture + n;
      if (texture >= DC_SCORE_REC_NUMBER_DRAW)
        texture -= DC_SCORE_REC_NUMBER_DRAW;
    
      glBindTexture(GL_TEXTURE_2D, record_textures[texture]);
      
      glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, yb_circle, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, yb_circle, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, yt_circle, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, yt_circle, 0.0f);
      glEnd();
    }

  glPopMatrix();
}
