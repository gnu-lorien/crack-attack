/*
 * CountDownManager.h
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

#ifndef COUNTDOWNMANAGER_H
#define COUNTDOWNMANAGER_H

using namespace std;

/* static */ class CountDownManager {
public:
  static void gameStart (   );
  static void cleanUp (   );

  static inline void timeStep (   )
  {
    if (state == -1) return;
    timeStep_inline_split_();
  }

  static int state;
  static int start_pause_alarm;
  static int message_switch_alarm;

private:
  static void timeStep_inline_split_ (   );
};

#endif
