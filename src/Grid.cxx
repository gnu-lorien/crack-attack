/*
 * Grid.cxx
 * Daniel Nelson - 8/23/0
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
 * Holds a map of the play area and also deals with elimination checks.
 */

using namespace std;

#include "Game.h"
#include "Grid.h"
#include "Garbage.h"
#include "ComboManager.h"
#include "LevelLights.h"
#include "Swapper.h"
#include "BlockManager.h"
#include "Random.h"

GridElement Grid::grid[GC_PLAY_WIDTH][GC_PLAY_HEIGHT];
CheckRegistryElement Grid::check_registry[GC_GRID_SIZE];
int Grid::check_count;
int Grid::shatter_count;
int Grid::shatter_top;
int Grid::shatter_bottom;
bool Grid::gray_shatter;
int Grid::top_occupied_row;
int Grid::top_effective_row;

void Grid::gameStart (   )
{
  check_count = 0;

  top_occupied_row = 0;

  for (int i = GC_PLAY_WIDTH; i--; )
    for (int j = GC_PLAY_HEIGHT; j--; ) {
      grid[i][j].state = GR_EMPTY;
      grid[i][j].resident_type = GR_EMPTY;
      grid[i][j].resident = null;
    }

  // Generate the initial blocks.

  int short_collumn = Random::number(GC_PLAY_WIDTH);

  for (int x = GC_PLAY_WIDTH; x--; ) {

    // generate the height
    int height = (short_collumn == x ? 2 : 7) + Random::number2(2);
    if (height - 1 > top_occupied_row)
      top_occupied_row = height - 1;

    for (int y = height; --y; ) {

      // determine the flavor
      int flavor;
      do {
        flavor = Random::number(BF_NUMBER_NORMAL);

        if (!(Grid::stateAt(x, y + 1) & GR_EMPTY)
         && Grid::blockAt(x, y + 1).flavor == flavor) continue;

        if (x == GC_PLAY_WIDTH - 1) break;

        if (!(Grid::stateAt(x + 1, y) & GR_EMPTY)
         && Grid::blockAt(x + 1, y).flavor == flavor) continue;

        break;
      } while (true);

      // set the block manager block creation state
      if (y == 2)
        BlockManager::second_to_last_row_c[x] = flavor;
      else if (y == 1)
        BlockManager::last_row_c[x] = flavor;

      // create the block
      BlockManager::newBlock(x, y, flavor);
    }
  }

  top_effective_row = top_occupied_row;
}

void Grid::timeStep (   )
{
  // Process elimination check requests.

  // loop through the request registry
  for (int n = 0; check_count; n++)
    if (check_registry[n].mark) {
      check_registry[n].mark = false;
      check_count--;

      Block &block = BlockManager::blockStore[n];

      // insure that the block is still static
      if (!(block.state & BS_STATIC)) continue;

      // use the block's combo, if it has one
      handleEliminationCheckRequest(block,
       block.current_combo ? block.current_combo : check_registry[n].combo);
    }

  // Update top_occupied_row.
  top_occupied_row++;
  bool flag = true;
  do {
    top_occupied_row--;
    for (int x = GC_PLAY_WIDTH; x--; )
      if (!(stateAt(x, top_occupied_row) & GR_EMPTY)) {
        flag = false;
        break;
      }
  } while (flag);

  // Update top_effective_row.
  int o = top_effective_row;
  top_effective_row++;
  flag = true;
  do {
    top_effective_row--;
    for (int x = GC_PLAY_WIDTH; x--; )
      if (!(residentTypeAt(x, top_effective_row) & GR_EMPTY)
       && !((residentTypeAt(x, top_effective_row) & GR_GARBAGE)
       && garbageAt(x, top_effective_row).initial_fall)) {
        flag = false;
        break;
      }
  } while (flag);
  if (top_effective_row < o)
    LevelLights::levelLower(top_effective_row);
}

void Grid::handleEliminationCheckRequest ( Block &block, ComboTabulator *combo )
{
  int x = block.x;
  int y = block.y;

  // Look in four directions for matching lines.

  int l = x;
  while (l > 0) {
    if (!(stateAt(l - 1, y) & GR_BLOCK)) break;
    if (!matchAt(l - 1, y, block)) break;
    l--;
  }

  int r = x + 1;
  while (r < GC_PLAY_WIDTH) {
    if (!(stateAt(r, y) & GR_BLOCK)) break;
    if (!matchAt(r, y, block)) break;
    r++;
  }

  int b = y;
  while (b > 1) {
    if (!(stateAt(x, b - 1) & GR_BLOCK)) break;
    if (!matchAt(x, b - 1, block)) break;
    b--;
  }

  int t = y + 1;
  while (t < GC_PLAY_HEIGHT) {
    if (!(stateAt(x, t) & GR_BLOCK)) break;
    if (!matchAt(x, t, block)) break;
    t++;
  }

  int w = r - l;
  int h = t - b;

  int magnitude = 0;
  int pattern = 0;
  if (w >= GC_MIN_PATTERN_LENGTH) {
    pattern |= PT_HORIZONTAL;
    magnitude += w;
  }
  if (h >= GC_MIN_PATTERN_LENGTH) {
    pattern |= PT_VERTICAL;
    magnitude += h;
  }

  // if no pattern found
  if (pattern == 0) {
    block.endComboInvolvement(combo);
    return;
  }

  // create a combo for the elimination
  if (!combo)
    combo = &ComboManager::newComboTabulator();

  // if pattern matches both directions
  if (pattern == (PT_HORIZONTAL | PT_VERTICAL)) magnitude--;

  // Kill the pattern's blocks and look for touching garbage.

  shatter_count = 0;
  shatter_top = 0;
  shatter_bottom = GC_PLAY_HEIGHT;

  // we need to know if it's a gray shatter in order to know if black
  // garbage should shatter
  if (!BlockManager::isColorlessFlavor(block.flavor))
    gray_shatter = false;
  else
    gray_shatter = true;

  ComboManager::specialBlockTally(*combo, block);
  block.startDying(combo, magnitude);

  if (pattern & PT_HORIZONTAL) {
    // kill the pattern's blocks
    for (int k_x = l; k_x < r; k_x++)
      if (k_x != x) {
        ComboManager::specialBlockTally(*combo, blockAt(k_x, y));
        blockAt(k_x, y).startDying(combo, magnitude);
      }

    // look for garbage to the left and below the pattern
    if (l > 0)
      shatterGarbage(l - 1, y);
    if (y > 1)
      for (int k_x = l; k_x < r; k_x++)
        shatterGarbage(k_x, y - 1);

    // look for garbage to the right and above the pattern
    if (r < GC_PLAY_WIDTH)
      shatterGarbage(r, y);
    if (y < GC_PLAY_HEIGHT - 1)
      for (int k_x = l; k_x < r; k_x++)
        shatterGarbage(k_x, y + 1);
  }

  if (pattern & PT_VERTICAL) {
    // kill the pattern's blocks
    for (int k_y = b; k_y < t; k_y++)
      if (k_y != y) {
        ComboManager::specialBlockTally(*combo, blockAt(x, k_y));
        blockAt(x, k_y).startDying(combo, magnitude);
      }

    // look for garbage to the left and below the pattern
    if (b > 1)
      shatterGarbage(x, b - 1);
    if (x > 0)
      for (int k_y = b; k_y < t; k_y++)
        shatterGarbage(x - 1, k_y);

    // look for garbage to the right and above the pattern
    if (t < GC_PLAY_HEIGHT)
      shatterGarbage(x, t);
    if (x < GC_PLAY_WIDTH - 1)
      for (int k_y = b; k_y < t; k_y++)
        shatterGarbage(x + 1, k_y);
  }

  // The grid now contains shattered elements.  We now traverse the grid and
  // syncronize the shattering garbages' timers.  The garbage itself deals with
  // the creation of blocks and such, as it's behavior may varry.

  // delay until the new blocks fall
  int awaken_delay = GC_INITIAL_POP_DELAY + GC_FINAL_POP_DELAY
   + GC_INTERNAL_POP_DELAY * (shatter_count - 1);

  // delay until the next block pops
  int pop_delay = GC_INITIAL_POP_DELAY;

  // traverse the shatter area; s_x and pop_delay will be advanced during
  // startShattering()
  for (int s_y = shatter_bottom; s_y < shatter_top; s_y++)
    for (int s_x = 0; s_x < GC_PLAY_WIDTH; )
      if (stateAt(s_x, s_y) & GR_SHATTERING)
        garbageAt(s_x, s_y).startShattering(s_x, s_y, pop_delay, awaken_delay,
         combo);
      else
        s_x++;

  // notify the combo of the pattern match
  combo->reportElimination(magnitude, block);
}

void Grid::shatterGarbage_inline_split_ ( int x, int y, Garbage *due_to )
{
  Garbage &garbage = garbageAt(x, y);

  // ask the garbage about shattering; we have to do black ourselves
  if (!garbage.considerShattering(due_to)) return;
  if (garbage.flavor == GF_BLACK && !gray_shatter) return;

  // keep track of the bounds on the shattered area
  if (y + garbage.height > shatter_top)
    shatter_top = y + garbage.height;
  if (y < shatter_bottom)
    shatter_bottom = y;

  shatter_count += garbage.width * garbage.height;

  // mark the garbage grid locations as shattered
  for (int h = garbage.height; h--; )
    for (int w = garbage.width; w--; )
      grid[garbage.x + w][garbage.y + h].state = GR_SHATTERING;

  // look for connecting garbage recursively
  if (garbage.x > 0)
    for (int h = garbage.height; h--; )
      shatterGarbage(garbage.x - 1, garbage.y + h, &garbage);
  if (garbage.x + garbage.width < GC_PLAY_WIDTH)
    for (int h = garbage.height; h--; )
      shatterGarbage(garbage.x + garbage.width, garbage.y + h, &garbage);
  if (garbage.y > 1)
    for (int w = garbage.width; w--; )
      shatterGarbage(garbage.x + w, garbage.y - 1, &garbage);
  if (garbage.y + garbage.height < GC_PLAY_HEIGHT)
    for (int w = garbage.width; w--; )
      shatterGarbage(garbage.x + w, garbage.y + garbage.height, &garbage);
}

bool Grid::shiftGridUp (   )
{
  if (top_occupied_row == GC_PLAY_HEIGHT - 1) return false;

  // shift the grid
  for (int y = top_occupied_row + 1; y--; )
    for (int x = GC_PLAY_WIDTH; x--; )
      grid[x][y + 1] = grid[x][y];

#ifndef NDEBUG
  // otherwise the assert() will tag us
  for (int x = GC_PLAY_WIDTH; x--; )
    grid[x][0].state = GR_EMPTY;
#endif

  top_occupied_row++;
  top_effective_row++;
  LevelLights::levelRaise(top_effective_row);

  // shift blocks and garbage up with the grid
  BlockManager::shiftUp();
  GarbageManager::shiftUp();

  // shift swapper up
  Swapper::shiftUp();

  return true;
}
