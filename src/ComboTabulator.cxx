/*
 * ComboTabulator.cxx
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
 *
 * Each one keeps track of a set of or potential set of elimination combos.
 */

#include "Game.h"
#include "Block.h"
#include "GarbageGenerator.h"
#include "ComboTabulator.h"
#include "SparkleManager.h"
#include "SignManager.h"
#include "BlockManager.h"
#include "Score.h"

using namespace std;

void ComboTabulator::initialize (   )
{
  magnitude = 0;
  special_magnitude = 0;
  multiplier = 1;
  n_multipliers_this_step = 0;
  base_accumulated_score =0;
  base_score_this_step = 0;

  creation_time_stamp = Game::time_step;

  involvement_count = 0;
}

void ComboTabulator::reportElimination ( int _magnitude, Block &kernel )
{
  // hold the location for standard reward mote creation
  x = kernel.x;
  y = kernel.y;

  time_stamp = Game::time_step;

  // only increase the multiplier if this is a late elimination
  if (Game::time_step != creation_time_stamp) {

    multiplier++;
    n_multipliers_this_step++;

    SignManager::createSign(x, y, ST_MULTIPLIER, multiplier - 2);
    SparkleManager::createRewardMote(x, y, multiplier + 9);
  }

  // if this is a special flavor combo match
  if (BlockManager::isColorlessFlavor(kernel.flavor))
    special_magnitude += _magnitude;

  // if this is a normal flavor combo match
  else
    magnitude += _magnitude;
}
