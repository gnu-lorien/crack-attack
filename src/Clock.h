/*
 * Clock.h
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

#ifndef CLOCK_H
#define CLOCK_H

using namespace std;

#include "Game.h"

/* static */ class Clock {
public:
  static void gameStart (   );

  static inline void timeStepPlay (   )
  {
    if (++tick != GC_STEPS_PER_SECOND) return;
    tick = 0;
    previous_digits[0] = digits[0];
    previous_digits[1] = digits[1];
    previous_digits[2] = digits[2];
    previous_digits[3] = digits[3];
    if (++digits[0] == 10) {
      digits[0] = 0;
      if (++digits[1] == 6) {
        digits[1] = 0;
        if (++digits[2] == 10) {
          digits[2] = 0;
          if (++digits[3] == 6)
            digits[3] = 0;
        }
      }
    }
  }

  static inline void timeStepMeta (   )
  /*
   * Ensures competion of the final tick fade after game end.
   */
  {
    if (tick == -1) return;
    if (++tick != GC_STEPS_PER_SECOND) return;
    tick = -1;
    previous_digits[0] = digits[0];
    previous_digits[1] = digits[1];
    previous_digits[2] = digits[2];
    previous_digits[3] = digits[3];
  }

  static short digits[4];
  static short previous_digits[4];
  static int tick;
};

#endif
