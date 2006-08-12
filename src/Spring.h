/*
 * Spring.h
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

#ifndef SPRING_H
#define SPRING_H


// spring constants, if you will
#define SP_IMPACT_VELOCITY                      (0.1f)
#define SP_GARBAGE_DENSITY                      (0.2f)
#define SP_STIFFNESS                            (0.1f)
#define SP_DRAG                                 (0.1f)

/* static */ class Spring {
public:
  static void gameStart (   );

  static inline void notifyImpact ( int height, int width )
  {
    float dv = (SP_IMPACT_VELOCITY + v) * (height * width) * SP_GARBAGE_DENSITY;
    if (dv > 0.0f)
      v -= dv;
  };

  static inline void timeStep (   )
  {
    y += v;
    v -= SP_STIFFNESS * y + SP_DRAG * v;
  };

  static float y;

private:
  static float v;
};

#endif
