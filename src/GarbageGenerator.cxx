/*
 * GarbageGenerator.cxx
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
 * Displays and sends garbage to the opponent.
 */

#include <iostream>

using namespace std;

#include "Game.h"
#include "GarbageGenerator.h"
#include "Block.h"
#include "BlockManager.h"
#include "Garbage.h"
#include "GarbageManager.h"
#include "ComboTabulator.h"
#include "Communicator.h"
#include "SparkleManager.h"
#include "SignManager.h"

GarbageQueueElement GarbageGenerator::garbage_queue[GC_GARBAGE_QUEUE_SIZE];
int GarbageGenerator::waiting_count;

void GarbageGenerator::gameStart (   )
{
  waiting_count = 0;
  for (int n = GC_GARBAGE_QUEUE_SIZE; n--; )
    garbage_queue[n].active = false;
}

void GarbageGenerator::comboElimination ( ComboTabulator &combo )
{
  int sibling = (combo.multiplier > 1 ? 1 : 0);

  // used by blocks to determine death spark number
  combo.latest_magnitude = combo.special_magnitude + combo.magnitude
   + combo.multiplier - 1;

  // send the special garbage

  bool special_sign = true;
  for (int n = BF_NUMBER_SPECIAL; n--; ) {
    if (combo.special[n]) {
      special_sign = false;
      SignManager::createSign(combo.x, combo.y, ST_SPECIAL, n + 1);

      if (BlockManager::isColorlessCode(n))
        combo.special_magnitude -= combo.special[n];

      while (combo.special[n]--) {
        sendSpecialGarbage(GarbageManager::mapBlockCodeToGarbageFlavor(n));
        SparkleManager::createRewardMote(combo.x, combo.y, n + 4, sibling++);
      }
      combo.special[n] = 0;
    }
  }

  // send the gray garbage

  if (combo.special_magnitude >= GC_MIN_PATTERN_LENGTH) {
    if (special_sign)
      SignManager::createSign(combo.x, combo.y, ST_SPECIAL, 0);

    combo.special_magnitude -= GC_MIN_PATTERN_LENGTH - 2;
    while (--combo.special_magnitude) {

      sendSpecialGarbage(GF_GRAY);
      SparkleManager::createRewardMote(combo.x, combo.y, 3, sibling++);
    }

  } else
    combo.special_magnitude = 0;

  // send the normal garbage

  if (combo.magnitude > GC_MIN_PATTERN_LENGTH) {
    SignManager::createSign(combo.x, combo.y, ST_MAGNITUDE,
     combo.magnitude - 4);

    if (combo.magnitude <= GC_PLAY_WIDTH) {

      sendGarbage(1, combo.magnitude - 1, GF_NORMAL);
      SparkleManager::createRewardMote(combo.x, combo.y,
       combo.magnitude - 4, sibling++);

    } else if (combo.magnitude < 2 * GC_PLAY_WIDTH - 1) {

      sendGarbage(1, combo.magnitude - (combo.magnitude >> 1),
       GF_NORMAL);
      SparkleManager::createRewardMote(combo.x, combo.y,
       combo.magnitude - (combo.magnitude >> 1) - 3, sibling++);

      sendGarbage(1, combo.magnitude >> 1, GF_NORMAL);
      SparkleManager::createRewardMote(combo.x, combo.y,
       (combo.magnitude >> 1) - 3, sibling++);

    } else {
      combo.magnitude += GC_MIN_PATTERN_LENGTH;
      while (combo.magnitude > GC_PLAY_WIDTH - 1) {

        sendGarbage(1, GC_PLAY_WIDTH - 1, GF_NORMAL);
        SparkleManager::createRewardMote(combo.x, combo.y,
         GC_PLAY_WIDTH - 4, sibling++);
        
        combo.magnitude -= GC_PLAY_WIDTH - 1;
      }

      if (combo.magnitude >= GC_MIN_PATTERN_LENGTH) {

        sendGarbage(1, combo.magnitude, GF_NORMAL);
        SparkleManager::createRewardMote(combo.x, combo.y,
         combo.magnitude - 3, sibling);
      }
    }
  }

  combo.magnitude = 0;
}

void GarbageGenerator::comboComplete ( ComboTabulator &combo )
{
  // send the multiplier garbage
  if (combo.multiplier > 1)
    sendGarbage(combo.multiplier - 1, GC_PLAY_WIDTH, GF_NORMAL);
}

void GarbageGenerator::addToQueue ( CommunicationBuffer &buffer )
{
  for (unsigned int n = 0; n < buffer.count; n++) {
    BufferElement &e = buffer.garbage[n];

    if (!GarbageManager::isSpecialFlavor(e.flavor))
      dealLocalGarbage(e.height, e.width, e.flavor, e.time_stamp);
    else
      dealSpecialLocalGarbage(e.flavor, e.time_stamp);
  }
}

void GarbageGenerator::dealLocalGarbage ( int height, int width, int flavor,
 int time_stamp )
/*
 * Used for solo games.
 */
{
  if (waiting_count == GC_GARBAGE_QUEUE_SIZE) return;

  int i = 0;
  while (garbage_queue[i].active) i++;

  garbage_queue[i].active = true;
  garbage_queue[i].height = height;
  garbage_queue[i].width = width;
  garbage_queue[i].flavor = flavor;
  garbage_queue[i].alarm = determineDropTime(time_stamp);

  waiting_count++;
}

void GarbageGenerator::dealSpecialLocalGarbage ( int flavor, int time_stamp )
{
  switch (flavor) {
  case GF_GRAY: case GF_WHITE: case GF_COLOR_2:
    dealLocalGarbage(1, GC_PLAY_WIDTH, flavor, time_stamp);
    break;

  case GF_BLACK:
    dealLocalGarbage(1, 2, GF_BLACK, time_stamp);
    break;

  case GF_COLOR_1:
    if (Random::chanceIn2(4)) {
      dealLocalGarbage(2, 2, GF_COLOR_1, time_stamp);
      for (int n = 1 + Random::number(3); n--; )
        dealLocalGarbage(1, 1, GF_COLOR_1, time_stamp);
    } else
      for (int n = 5 + Random::number(3); n--; )
        dealLocalGarbage(1, 1, GF_COLOR_1, time_stamp);
    break;

  case GF_COLOR_3:
    dealLocalGarbage(1, 4, GF_COLOR_3, time_stamp);
    break;

  case GF_COLOR_4:
    dealLocalGarbage(1, 3, GF_COLOR_4, time_stamp);
    break;

  case GF_COLOR_5:
    dealLocalGarbage(3, 2, GF_COLOR_5, time_stamp);
    break;
  }
} 

void GarbageGenerator::timeStep (   )
{
  int c = waiting_count;

  for (int n = 0; c; n++)
    if (garbage_queue[n].active) {
      GarbageQueueElement &e = garbage_queue[n];
      c--;

      // if this garbage's ready, let's try to drop it
      if (e.alarm < Game::time_step) {

        // if we successfully drop it, take it away
        if (GarbageManager::newFallingGarbage(e.height, e.width, e.flavor)) {
          waiting_count--;
          e.active = false;

        // otherwise, reset the alarm
        } else
          e.alarm = Game::time_step + GC_AVERAGE_GARBAGE_DROP_DELAY;
      }
    }
}
