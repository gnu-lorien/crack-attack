/*
 * ComboTabulator.h
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

#ifndef COMBOTABULATOR_H
#define COMBOTABULATOR_H

using namespace std;

#include "Game.h"

class Block;

class ComboTabulator {
public:
  void initialize (   );

  void reportElimination ( int _magnitude, Block &kernel );

  inline void incrementInvolvement (   )
    { ++involvement_count; }
  inline void decrementInvolvement (   )
    { --involvement_count; }

  // free store id
  int id;

  // latest elimination time stamp
  int time_stamp;

  // creation time stamp
  int creation_time_stamp;

  // number of blocks involved in combo
  int involvement_count;

  // normal elimination magnitude
  int magnitude;

  // special elimination magnitude
  int special_magnitude;

  // combo multiplier
  int multiplier;

  // number of multipliers this time step
  int n_multipliers_this_step;

  // base score earned so far on this multiplier
  int base_accumulated_score;

  // base score earned so far on this turn
  int base_score_this_step;

  // used by blocks the step after elimination to determine spark number
  int latest_magnitude;

  // keep track of the location of the eliminations
  int x, y;

  // keeps track of the various special blocks which have been eliminated
  int special[BF_NUMBER_SPECIAL];
};

#endif
