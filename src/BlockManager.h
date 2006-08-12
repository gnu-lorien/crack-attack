/*
 * BlockManager.h
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

#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <cassert>

#include "Game.h"
#include "Block.h"
#include "Random.h"
#include "MetaState.h"
#include "X.h"

class ComboTabulator;

/* static */ class BlockManager {
public:
  static void gameStart (   );
  static void newAwakingBlock ( int x, int y, int pop_delay, int awake_delay,
   ComboTabulator *combo, int pop_color );

  static inline void newCreepRow (   )
  {
    if (!(MetaState::mode & CM_X))
      if (Random::chanceIn(GC_NO_SPECIAL_BLOCK_CHANCE_IN))
        special_block_location = -1;
      else
        special_block_location = Random::number(GC_PLAY_WIDTH);
    else
      if (Random::chanceIn(GC_X_NO_SPECIAL_BLOCK_CHANCE_IN))
        special_block_location = -1;
      else
        special_block_location = Random::number(GC_PLAY_WIDTH);

    for (int x = GC_PLAY_WIDTH; x--; )
      newCreepBlock(x);
  }

  static inline void newBlock ( int x, int y, int flavor )
  {
    if (block_count == GC_BLOCK_STORE_SIZE) return;

    int id = findFreeId();
    allocateId(id);
    Block &block = blockStore[id];

    block.initializeStatic(x, y, flavor);
  }

  static inline void newBlock ( int x, int y, int flavor, int pop_delay,
   int awake_delay, ComboTabulator *combo, int pop_color )
  {
    if (block_count == GC_BLOCK_STORE_SIZE) return;

    int id = findFreeId();
    allocateId(id);
    Block &block = blockStore[id];

    block.initializeAwaking(x, y, flavor, pop_delay, awake_delay, combo,
     pop_color);
  }

  static inline void deleteBlock ( Block *block )
  {
    freeId(block->id);
  }

  static inline void shiftUp (   )
  {
    int c = block_count;
    for (int n = 0; c; n++)
      if (storeMap[n]) {
        c--;
        blockStore[n].y++;
      }
  }

  static inline int generatePopDirection (   )
  {
    if (next_pop_direction & (1 << 3))
      return next_pop_direction = (1 << 0);
    else
      return next_pop_direction <<= 1;
  }

  static inline int generatePopDirection ( int n )
  {
    int npd;
    if (next_pop_direction & (1 << 3))
      npd = next_pop_direction = (1 << 0);
    else
      npd = next_pop_direction <<= 1;
    while (--n)
      if (next_pop_direction & (1 << 3))
        next_pop_direction = (1 << 0);
      else
        next_pop_direction <<= 1;
    return npd;
  }

  static inline bool flavorMatch ( Block &block_1, Block &block_2 )
  {
    if (!X::wildActive())
      return mapFlavorToBaseFlavor(block_1.flavor)
       == mapFlavorToBaseFlavor(block_2.flavor);
    else if (block_1.flavor != BF_WILD && block_2.flavor != BF_WILD)
      return mapFlavorToBaseFlavor(block_1.flavor)
       == mapFlavorToBaseFlavor(block_2.flavor);
    else {
      int f1;
      if (block_1.flavor == BF_WILD)
        f1 = X::wildFlavor(block_1);
      else
        f1 = mapFlavorToBaseFlavor(block_1.flavor);
      if (block_2.flavor == BF_WILD)
        return f1 == X::wildFlavor(block_2);
      else
        return f1 == mapFlavorToBaseFlavor(block_2.flavor);
    }
  }

  static inline bool isNormalFlavor ( int flavor )
  {
    return flavor <= BF_NUMBER_NORMAL;
  }

  static inline bool isBaseFlavor ( int flavor )
  {
    return flavor <= BF_GRAY;
  }

  static inline bool isColorlessFlavor ( int flavor )
  {
    return flavor >= BF_GRAY && flavor <= BF_FINAL_GRAY_SPECIAL;
  }

  static inline bool isSpecialFlavor ( int flavor )
  {
    return flavor > BF_GRAY;
  }

  static inline bool isSpecialColorFlavor ( int flavor )
  {
    return flavor >= BF_SPECIAL_COLOR_1;
  }

  static inline int mapFlavorToBaseFlavor ( int flavor )
  {
    if (isBaseFlavor(flavor))
      return flavor;
    if (isSpecialColorFlavor(flavor))
      return mapSpecialColorFlavorToColor(flavor);
    return BF_GRAY;
  }

  // The following handle block flavor codes.  Each special block flavor has
  // a code which is distinct from it's flavor number.  This code is used to
  // dereference special flavor arrays.

  static inline bool isColorlessCode ( int code )
  {
    return code <= mapSpecialFlavorToCode(BF_FINAL_GRAY_SPECIAL);
  }

  static inline int mapSpecialFlavorToCode ( int flavor )
  {
    return flavor - (BF_GRAY + 1);
  }

  static inline int mapSpecialColorFlavorToColor ( int flavor )
  {
    return flavor - BF_SPECIAL_COLOR_1;
  }

  static int block_count;
  static Block blockStore[GC_BLOCK_STORE_SIZE];
  static bool storeMap[GC_BLOCK_STORE_SIZE];

  static int last_row_c[GC_PLAY_WIDTH], second_to_last_row_c[GC_PLAY_WIDTH];

private:
  static void newCreepBlock ( int x );

  static inline int findFreeId (   )
  {
    int n;
    for (n = 0; storeMap[n]; n++);
    return n;
  }

  static inline void allocateId ( int id )
  {
    assert(!storeMap[id]);
    storeMap[id] = true;
    block_count++;
  }

  static inline void freeId ( int id )
  {
    assert(storeMap[id]);
    storeMap[id] = false;
    block_count--;
  }

  static int next_pop_direction;

  static int last_flavor_a, second_to_last_flavor_a;
  static int last_flavor_c, second_to_last_flavor_c;
  static int last_row_a[GC_PLAY_WIDTH], second_to_last_row_a[GC_PLAY_WIDTH];
  static int special_block_location;
};

#endif
