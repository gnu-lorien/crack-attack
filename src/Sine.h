/*
 * Sine.h
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

#ifndef SINE_H
#define SINE_H

using namespace std;

#include "Game.h"

/* static */ class Sine {
public:
  static void initialize (   );

  static inline float sin ( float x )
  {
    int n = (int) ((x - GC_SINE_TABLE_LOWER_BOUND)
     * (1.0f / GC_SINE_TABLE_STEP_WIDTH));

    if (n < 0) n = 0;
    if (n > GC_SINE_TABLE_LENGTH - 2) n = GC_SINE_TABLE_LENGTH - 2;

    float remainder = x - GC_SINE_TABLE_LOWER_BOUND
     - n * GC_SINE_TABLE_STEP_WIDTH;

    float estimate = table[n] + (table[n + 1] - table[n])
     * remainder * (1.0f / GC_SINE_TABLE_STEP_WIDTH);
                    
    return estimate;
  }

  static float table[GC_SINE_TABLE_LENGTH];
};

#endif
