/*
 * MessageManager.h
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

#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H


#include "Displayer.h"

// message shapes
#define MS_1x1                  (1 << 0)
#define MS_2x1                  (1 << 1)
#define MS_4x1                  (1 << 2)

// messages
#define MS_COUNT_DOWN_GO        (0)
#define MS_COUNT_DOWN_1         (1)
#define MS_COUNT_DOWN_2         (2)
#define MS_COUNT_DOWN_3         (3)
#define MS_ANYKEY               (4)
#define MS_WAITING              (5)
#define MS_PAUSED               (6)
#define MS_WINNER               (7)
#define MS_LOSER                (8)
#define MS_GAME_OVER            (9)

// message modes
#define MM_NORMAL               (1 << 0)
#define MM_CELEBRATION          (1 << 1)

/* static */ class MessageManager {
public:
  static inline void readyMessage ( int message )
  {
    Displayer::readyMessage(message);
    message_shape = Displayer::message_shape[message];
    time_step = 0;
  }

  static inline void nextMessage ( int message )
  {
    Displayer::nextMessage(message);
    message_shape = Displayer::message_shape[message];
    time_step = 0;
  }

  static inline void freeMessage (   )
  {
    Displayer::freeMessage();
    message_shape = 0;
  }

  static inline void timeStep (   )
  {
    if (!message_shape) return;
    if (++time_step == DC_MESSAGE_PULSE_PERIOD)
      time_step = 0;
  }

  static int message_shape;
  static int time_step;
  static int mode;
};

#endif
