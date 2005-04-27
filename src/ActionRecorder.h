/*
 * ActionRecorder.h
 * 
 * Crack Attack! is the legal property of its developers, whose names 
 * are too numerous to list here.  Please refer to the COPYRIGHT file
 * distributed with this source distribution for a full listing.
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
 */

#ifndef ACTIONRECORDER_H
#define ACTIONRECORDER_H

#include "Game.h"
#include "Controller.h"

#define GC_REPLAY_FILE_NAME           "replay"

class Action {
public:
  int name;
  int time_step;

  Action(): name(0), time_step(0) {}
  Action(int n, int ts): name(n), time_step(ts) {}
};

/* static */ class ActionRecorder {
public:
  static inline void initialize (   )
  {
    actions.clear();
  }

  static inline void addAction ( int state )
  {
    Action a(state, Game::time_step);
    actions.push_back(a);
  }

  static void gameFinish (   );

private:
  static std::vector<Action> actions;
};

#endif 
