/*
 * Clock.cxx
 * Daniel Nelson - 11/5/0
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
 * Handles the timer state.
 */

#include "Game.h"
#include "Clock.h"
#include "WinRecord.h"

using namespace std;

short Clock::digits[4];
short Clock::previous_digits[4];
int Clock::tick;

void Clock::gameStart (   )
{
  tick = 0;
  for (int n = 4; n--; )
    digits[n] = 0;
  if (WinRecord::current_game == 0)
    for (int n = 4; n--; )
      previous_digits[n] = 0;
}
