/*
 * GarbageGenerator.h
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

#ifndef GARBAGEGENERATOR_H
#define GARBAGEGENERATOR_H

using namespace std;

#include "Game.h"
#include "MetaState.h"
#include "Communicator.h"
#include "Random.h"
#include "GarbageQueueElement.h"

class ComboTabulator;
class CommunicationBuffer;

/* static */ class GarbageGenerator {
public:
  static void gameStart (   );

  static void comboElimination ( ComboTabulator &combo );
  static void comboComplete ( ComboTabulator &combo );
  static void timeStep (   );
  static void addToQueue ( GarbageQueueElement &e );
  static void addToQueue ( CommunicationBuffer &buffer );
  static void addToQueue ( BufferElement garbage[], size_t size );
  static void addToQueue ( uint32 height, uint32 width, uint32 flavor, uint32 stamp );


private:
  static void sendGarbage ( int height, int width, int flavor );
  static void sendSpecialGarbage ( int flavor );

  static inline int determineDropTime ( int time_stamp )
  {
    return time_stamp
     + (GC_AVERAGE_GARBAGE_DROP_DELAY - GC_SPREAD_GARBAGE_DROP_DELAY / 2)
     + Random::number(GC_SPREAD_GARBAGE_DROP_DELAY);
  }

  static void dealLocalGarbage ( int height, int width, int flavor,
   int time_stamp );
  static void dealSpecialLocalGarbage ( int flavor, int time_stamp );

  static GarbageQueueElement garbage_queue[GC_GARBAGE_QUEUE_SIZE];
  static int waiting_count;
};

#endif
