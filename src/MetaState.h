/*
 * MetaState.h
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

#ifndef METASTATE_H
#define METASTATE_H

using namespace std;

#include "Game.h"

// program states
#define MS_BOTH_KEY_WAIT                   (1 << 0)
#define MS_REMOTE_KEY_WAIT                 (1 << 1)
#define MS_LOCAL_KEY_WAIT                  (1 << 2)
#define MS_GAME_OVER_KEY_WAIT              (1 << 3)
#define MS_READY_GAME_START                (1 << 4)
#define MS_CELEBRATION_WAIT                (1 << 5)
#define MS_GAME_PLAY                       (1 << 6)
#define MS_CONCESSION                      (1 << 7)

/* static */ class MetaState {
public:
  static void programStart ( int _mode,
   char player_name[GC_PLAYER_NAME_LENGTH] );
  static void programEnd (   );
  static void gameStart (   );
  static void gameWon (   );
  static void gameLoss (   );
  static void gameFinish (   );

  static void celebrationComplete (   );

  static void localKeyPressed ( bool esc );
  static void remoteKeyPressed (   );
  static void remoteReady (   );

  // set to Game::time_step upon game finish
  static int final_time_step;

  // current game state - playing, local key wait, ...
  static int state;

  // global game mode - server, client, solo, ...
  static int mode;

  // player name
  static char player_name[GC_PLAYER_NAME_LENGTH];
};

#endif
