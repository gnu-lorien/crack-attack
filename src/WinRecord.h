/*
 * WinRecord.h
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

#ifndef WINRECORD_H
#define WINRECORD_H

using namespace std;

#include "Game.h"
#include "MetaState.h"

// game results
#define GR_NOT_PLAYED             (1 << 0)
#define GR_BEING_PLAYED           (1 << 1)
#define GR_WON                    (1 << 2)
#define GR_LOST                   (1 << 3)

class Star {
public:
  GLfloat a;
  GLfloat v_a;
  GLfloat size;
  GLfloat v_size;
};

/* static */ class WinRecord {
public:
  static void initialize (   );
  static void timeStep (   );
  static void gameStart (   );
  static void gameWon (   );
  static void gameLoss (   );

  static inline bool isMatchFinished (   )
  {
    if (concession) return true;
    if (MetaState::mode & CM_SOLO) return true;
    if (won) {
      if (games_won == 1 + GC_GAMES_PER_MATCH / 2)
        return true;
    } else
      if (games_lost == 1 + GC_GAMES_PER_MATCH / 2)
        return true;
    return false;
  }

  static inline void matchConceded (   )
  {
    concession = true;
  }

  static int current_game;
  static bool won;
  static bool concession;
  static int games_won;
  static int games_lost;

  static int record[GC_GAMES_PER_MATCH];

  static Star stars[GC_GAMES_PER_MATCH];

  static int dynamic_star;
  static int displaced_star;
  static GLfloat win_star_x;
  static GLfloat win_star_y;
  static GLfloat win_star_v_x;
  static GLfloat win_star_v_y;
  static GLfloat old_star_a;
  static GLfloat old_star_size;

  static bool draw_old_star;
};

#endif
