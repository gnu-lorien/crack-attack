/*
 * GarbageManager.cxx
 * Daniel Nelson - 8/25/0
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
 * Allocates and frees blocks.
 */

using namespace std;

#include "Game.h"
#include "GarbageManager.h"
#include "Grid.h"
#include "Random.h"

int GarbageManager::garbage_count;
Garbage GarbageManager::garbageStore[GC_GARBAGE_STORE_SIZE];
bool GarbageManager::storeMap[GC_GARBAGE_STORE_SIZE];

void GarbageManager::gameStart (   )
{
  garbage_count = 0;
  for (int n = GC_GARBAGE_STORE_SIZE; n--; ) {
    storeMap[n] = false;
    garbageStore[n].id = n;
  }
}

bool GarbageManager::newFallingGarbage ( int height, int width, int flavor )
/*
 * The Grid variable top_occupied_row must be accurate at the time of call.
 */
{
  int drop_row;

  if (height > GC_MAX_GARBAGE_HEIGHT) height = GC_MAX_GARBAGE_HEIGHT;

  if (Grid::top_occupied_row >= GC_SAFE_HEIGHT)
    drop_row = Grid::top_occupied_row + 1;
  else
    drop_row = GC_SAFE_HEIGHT + 1;

  // no room in the inn; try again later; don't allow it to occupy the top
  // row, to leave room for the final creep
  if (drop_row + height > GC_PLAY_HEIGHT - 1)
    return false;

  Grid::top_occupied_row = drop_row + height - 1;

  if (width == GC_PLAY_WIDTH)
    newFallingGarbage(0, drop_row, height, width, flavor);
  else
    newFallingGarbage(Random::number((GC_PLAY_WIDTH + 1) - width), drop_row,
     height, width, flavor);

  return true;
}
