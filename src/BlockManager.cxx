/*
 * BlockManager.cxx
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
 * Allocates and frees blocks.
 */

using namespace std;

#include "Game.h"
#include "BlockManager.h"
#include "ComboTabulator.h"
#include "Random.h"
#include "X.h"

int BlockManager::block_count;
Block BlockManager::blockStore[GC_BLOCK_STORE_SIZE];
bool BlockManager::storeMap[GC_BLOCK_STORE_SIZE];

int BlockManager::last_flavor_a, BlockManager::second_to_last_flavor_a;
int BlockManager::last_flavor_c, BlockManager::second_to_last_flavor_c;
int BlockManager::last_row_a[GC_PLAY_WIDTH];
int BlockManager::second_to_last_row_a[GC_PLAY_WIDTH];
int BlockManager::last_row_c[GC_PLAY_WIDTH];
int BlockManager::second_to_last_row_c[GC_PLAY_WIDTH];
int BlockManager::special_block_location;

int BlockManager::next_pop_direction;

void BlockManager::gameStart (   )
{
  block_count = 0;
  for (int n = GC_BLOCK_STORE_SIZE; n--; ) {
    storeMap[n] = false;
    blockStore[n].id = n;
  }

  last_flavor_a = second_to_last_flavor_a = 0;
  last_flavor_c = second_to_last_flavor_c = 0;
  for (int x = GC_PLAY_WIDTH; x--; ) {
    last_row_a[x] = 0;
    second_to_last_row_a[x] = 0;
    last_row_c[x] = 0;
    second_to_last_row_c[x] = 0;
  }

  next_pop_direction = (1 << 0);

  special_block_location = -1;
}

void BlockManager::newAwakingBlock ( int x, int y, int pop_delay,
 int awake_delay, ComboTabulator *combo, int pop_color )
{
  int flavor;
  
  do {
    flavor = Random::number(BF_NUMBER_NORMAL);
  } while ((flavor == last_flavor_a
   && last_flavor_a == second_to_last_flavor_a)
   || (flavor == last_row_a[x] && last_row_a[x] == second_to_last_row_a[x]));

  second_to_last_row_a[x] = last_row_a[x];
  last_row_a[x] = flavor;

  second_to_last_flavor_a = last_flavor_a;
  last_flavor_a = flavor;

  newBlock(x, y, flavor, pop_delay, awake_delay, combo, pop_color);
}

void BlockManager::newCreepBlock ( int x )
{
  int flavor = 0;

  if (x != special_block_location) {
    do
      flavor = Random::number(BF_NUMBER_NORMAL);
    while ((flavor == last_flavor_c && last_flavor_c
     == second_to_last_flavor_c) || (flavor == last_row_c[x] && last_row_c[x]
     == second_to_last_row_c[x]));

    second_to_last_row_c[x] = last_row_c[x];
    last_row_c[x] = flavor;

    second_to_last_flavor_c = last_flavor_c;
    last_flavor_c = flavor;

  } else {
    int base_flavor = 0;

    if (!(MetaState::mode & CM_X)) {
      if ((BF_GRAY == last_flavor_c && last_flavor_c
       == second_to_last_flavor_c) || (BF_GRAY == last_row_c[x]
       && last_row_c[x] == second_to_last_row_c[x]))
        do
          flavor = Random::number(BF_NUMBER_NORMAL);
        while ((flavor == last_flavor_c && last_flavor_c
         == second_to_last_flavor_c) || (flavor == last_row_c[x]
         && last_row_c[x] == second_to_last_row_c[x]));
      else
        flavor = BF_GRAY;
      base_flavor = flavor;

    } else {
      do {
        switch (Random::number(10)) {
        case 0: case 1: case 2:
          base_flavor = flavor = BF_GRAY;
          break;
        case 3: case 4: case 5: case 6:
          base_flavor = mapSpecialColorFlavorToColor(flavor
           = Random::number(BF_NUMBER_NORMAL) + BF_SPECIAL_COLOR_1);
          if (!X::specialColorAllowed())
            flavor = base_flavor;
          break;
        case 7: case 8:
          if (X::wildAllowed())
            base_flavor = flavor = BF_WILD;
          else
            base_flavor = flavor = BF_GRAY;
          break;
        case 9:
          if (Random::chanceIn2(2))
            flavor = BF_BLACK;
          else
            flavor = BF_WHITE;
          base_flavor = mapFlavorToBaseFlavor(flavor);
          break;
        }
      } while ((base_flavor == last_flavor_c && last_flavor_c
       == second_to_last_flavor_c) || (base_flavor == last_row_c[x]
       && last_row_c[x] == second_to_last_row_c[x]));
    }

    second_to_last_row_c[x] = last_row_c[x];
    last_row_c[x] = base_flavor;

    second_to_last_flavor_c = last_flavor_c;
    last_flavor_c = base_flavor;
  }

  newBlock(x, 0, flavor);
}
