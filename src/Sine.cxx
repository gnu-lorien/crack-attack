/*
 * Sine.cxx
 * Daniel Nelson - 12/3/1
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
 *
 * Approximates sin(x) with a table and interpolation.
 */

using namespace std;

#include "Game.h"
#include "MetaState.h"
#include "Sine.h"

float Sine::table[GC_SINE_TABLE_LENGTH];

void Sine::initialize (   )
{
  // currently only used in CM_SOLO mode
  if (!(MetaState::mode & CM_SOLO)) return;

  for (int n = GC_SINE_TABLE_LENGTH; n--; ) {
    float x = n * GC_SINE_TABLE_STEP_WIDTH + GC_SINE_TABLE_LOWER_BOUND;
    table[n] = ::sin(x);
  }
}
