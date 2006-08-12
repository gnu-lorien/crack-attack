/*
 * GarbageManager.h
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

#ifndef GARBAGEMANAGER_H
#define GARBAGEMANAGER_H

#include <cassert>


#include "Garbage.h"
#include "GarbageFlavorImage.h"

class ComboTabulator;

/* static */ class GarbageManager {
public:
  static void gameStart (   );
  static bool newFallingGarbage ( int height, int width, int flavor );

  static inline void newAwakingGarbage ( int x, int y, int height,
   int pop_delay, int awake_delay, ComboTabulator *combo, int pop_color )
  {
    if (garbage_count == GC_GARBAGE_STORE_SIZE) return;

    int id = findFreeId();
    allocateId(id);
    Garbage *garbage = garbageStore + id;

    garbage->initializeAwaking(x, y, height, pop_delay, awake_delay, combo,
     pop_color);
  }

  static inline void newFallingGarbage ( int x, int y, int height, int width,
   int flavor )
  {
    if (garbage_count == GC_GARBAGE_STORE_SIZE) return;

    int id = findFreeId();
    allocateId(id);
    Garbage *garbage = garbageStore + id;

    garbage->initializeFalling(x, y, height, width, flavor);

    GarbageFlavorImage::requestGarbageFlavorImage(*garbage);
  }

  static inline void deleteGarbage ( Garbage *garbage )
  {
    GarbageFlavorImage::notifyGarbageDestruction(*garbage);
    freeId(garbage->id);
  }

  static inline void shiftUp (   )
  {
    int c = garbage_count;
    for (int n = 0; c; n++)
      if (storeMap[n]) {
        c--;
        garbageStore[n].y++;
      }
  }

  static inline bool isSpecialFlavor ( int flavor )
  {
    return flavor;
  }

  static inline int mapBlockCodeToGarbageFlavor ( int code )
  {
    return code + (GF_GRAY + 1);
  }

  static int garbage_count;
  static Garbage garbageStore[GC_GARBAGE_STORE_SIZE];
  static bool storeMap[GC_GARBAGE_STORE_SIZE];

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
    garbage_count++;
  }

  static inline void freeId ( int id )
  {
    assert(storeMap[id]);
    storeMap[id] = false;
    garbage_count--;
  }
};

#endif
