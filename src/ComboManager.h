/*
 * ComboManager.h
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

#ifndef COMBOMANAGER_H
#define COMBOMANAGER_H

#include <cassert>

using namespace std;

#include "ComboTabulator.h"
#include "BlockManager.h"
#include "Block.h"

class Block;

/* static */ class ComboManager {
public:
  static void gameStart (   );
  static void timeStep (   );

  static inline ComboTabulator &newComboTabulator (   )
  {
    int id = findFreeId();
    allocateId(id);
    ComboTabulator &combo = tabulatorStore[id];

    combo.initialize();
    return combo;
  }

  static inline void deleteComboTabulator ( ComboTabulator &combo )
  {
    freeId(combo.id);
  }

  static inline void specialBlockTally ( ComboTabulator &combo, Block &block )
  {
    if (BlockManager::isBaseFlavor(block.flavor)) return;
    combo.special[BlockManager::mapSpecialFlavorToCode(block.flavor)]++;
  }


private:
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
    combo_count++;
  }

  static inline void freeId ( int id )
  {
    assert(storeMap[id]);
    storeMap[id] = false;
    combo_count--;
  }

  static ComboTabulator tabulatorStore[GC_COMBO_TABULATOR_STORE_SIZE];
  static bool storeMap[GC_COMBO_TABULATOR_STORE_SIZE];
  static int combo_count;
};

#endif
