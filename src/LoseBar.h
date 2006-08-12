/*
 * LoseBar.h
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

#ifndef LOSEBAR_H
#define LOSEBAR_H

#include <cassert>


#define LB_INACTIVE                  (1 << 0)
#define LB_LOW_ALERT                 (1 << 1)
#define LB_HIGH_ALERT                (1 << 2)
#define LB_FADE_LOW_TO_INACTIVE      (1 << 3)
#define LB_FADE_HIGH_TO_INACTIVE     (1 << 4)
#define LB_FADE_RESET_HIGH           (1 << 5)

/* static */ class LoseBar {
public:
  static void initialize (   );
  static void gameStart (   );

  static void timeStep (   );

  static inline void highAlertReset (   )
  /*
   * Called by Creep when the high alert timer is reset.
   */
  {
    assert(state == LB_HIGH_ALERT);

    fade_timer = DC_LOSEBAR_FADE_TIME;
    state = LB_FADE_RESET_HIGH;
  }

  static int state;
  static GLfloat bar;
  static int fade_timer;

private:
  static inline void enterLowToInactiveFade (   )
  {
    fade_timer = DC_LOSEBAR_FADE_TIME;
    state = LB_FADE_LOW_TO_INACTIVE;
  }

  static inline void enterHighToInactiveFade (   )
  {
    fade_timer = DC_LOSEBAR_FADE_TIME;
    state = LB_FADE_HIGH_TO_INACTIVE;
  }
};

#endif
