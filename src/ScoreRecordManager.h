/*
 * ScoreRecordManager.h
 * Daniel Nelson - 8/24/0
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
 */

#ifndef SCORERECORDMANAGER_H
#define SCORERECORDMANAGER_H

#include <GL/glut.h>

#include "glext.h"

using namespace std;

#include "WinRecord.h"
#include "Displayer.h"
#include "Score.h"

/* static */ class ScoreRecordManager {
public:
  static void initialize (   );
  static void gameFinish_inline_split_ (   );
  static void timeStep_inline_split_ (   );

  static inline void gameStart (   )
  {
    if (!(MetaState::mode & CM_SOLO)) return;

    glDeleteTextures(1, &Displayer::score_to_beat_texture);
  }

  static inline void gameFinish (   )
  {
    if (!(MetaState::mode & CM_SOLO) || (MetaState::mode & CM_AI)) return;
    if (!WinRecord::won) return;

    gameFinish_inline_split_();
  }

  static inline void timeStep (   )
  {
    if (!(MetaState::mode & CM_SOLO)) return;
    if (!WinRecord::won) return;
    if (Game::time_step < DC_WIN_FADE_TIME) return;

    timeStep_inline_split_();
  }

  static inline GLubyte *chooseTexture ( int rank )
  {
    if (rank > Score::player_rank)
      return Displayer::record_texture_data[rank - 1];
    if (rank < Score::player_rank)
      return Displayer::record_texture_data[rank];
    return Displayer::player_rank_texture_data;
  }

  static int top_rank;
  static int top_texture;
  static GLfloat offset;
  static GLfloat velocity;

  static bool spring_active;
  static bool control_active;
  static bool ignore_up;
  static bool ignore_down;
};

#endif
