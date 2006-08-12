/*
 * ComputerPlayerAI.cxx
 * Andrew Sayman - 3/27/05
 *
 * Copyright (C) 2005  Andrew Sayman
 * Copyright (C) 2005  Kevin Webb
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
 */

#include "ComputerPlayerAI.h"

#include "Score.h"
#include "Garbage.h"
#include "GarbageGenerator.h"
#include "Game.h"

#include <cassert>

#define loop(v,m) for(int v = 0; v<(m); v++)
#define loopi(m) loop(i,m)

/* Begin ComputerPlayerAI */
ComputerPlayerAI::ComputerPlayerAI ()
{ 
  resetAlarm();
  last_shatter_height = 0;
  state = AI_WAITING;
  queue = new GarbageQueue();
}

int ComputerPlayerAI::baseSteps()
{
  return GC_STEPS_PER_SECOND;
}

int ComputerPlayerAI::stateSteps()
{
  if (state == AI_WAITING) {
    return (GC_CREEP_ADVANCE_VELOCITY * 5) // increase five lines
      + (GC_DYING_DELAY * 5); // five combos
  }
  if (state == AI_SHATTERING) {
    return garbageShatterDelay();
  }
	return GC_DYING_DELAY * 5;
}

int ComputerPlayerAI::lossHeight()
{
  return 4;
}

void ComputerPlayerAI::resetAlarm() 
{
  last_time = Game::time_step;
}

int ComputerPlayerAI::alarm()
{
  return last_time + baseSteps() + stateSteps();
}

int ComputerPlayerAI::garbageShatterDelay()
{
  int delay = GC_INITIAL_POP_DELAY + (last_shatter_height * GC_PLAY_WIDTH * GC_INTERNAL_POP_DELAY) + GC_FINAL_POP_DELAY;
  return delay;
}

GarbageQueue *ComputerPlayerAI::garbageQueue ()
{
  return queue;
}

GarbageQueue *ComputerPlayerAI::garbageAmount( )
{
  GarbageQueue *q = new GarbageQueue();
  int working_height = GC_SAFE_HEIGHT - 1 - garbageQueue()->height();
  int num_grays, num_normals;

  MESSAGE("Hard garbageAmount");

  if (working_height > 0) {
    num_grays = working_height % 3;
  } else {
    num_grays = 0;
  }

  num_normals = garbageQueue()->height() + working_height;

  LOG("garbageQueue height " << garbageQueue()->height());
  LOG("grays: " << num_grays << " normals: " << num_normals);
  loopi(num_grays)
    q->add(1, 6, GF_GRAY);

  int norm_div = num_normals / 3;
  int norm_mod = num_normals % 3;
  //int more_gray = norm_mod / 2;
  LOG("div: " << norm_div << " mod: " << norm_mod << " gray: " << more_gray);
  if (norm_div > 0) q->add(norm_div, 6, GF_NORMAL);
  loopi(norm_mod) q->add(1, 6, GF_NORMAL);
  //loopi(more_gray) q->add(1, 6, GF_GRAY);

  shatter();
  return q;
}

void ComputerPlayerAI::shatter()
{
  MESSAGE("Resetting garbageQueue " << garbageQueue()->height());
  if (garbageQueue()->height() > 0) {
    state = AI_SHATTERING; 
    //int gray_height = garbageQueue()->specialHeight();
    last_shatter_height = garbageQueue()->removeWithSpecials();
    MESSAGE(last_shatter_height << " shattered and " << garbageQueue()->height() << " remaining grays:" << gray_height);
    loopi(last_shatter_height) {
      if (Random::chanceIn(GC_GARBAGE_TO_GARBAGE_SHATTER)) {
        garbageQueue()->add(1, 6, GF_NORMAL);
        MESSAGE("Adding garbage on AI shatter " << garbageQueue()->height());
      }
    }
  } else {
    state = AI_WAITING;
    last_shatter_height = 0;
  }
}

bool ComputerPlayerAI::determineLoss()
{
  GarbageQueue *queue = garbageQueue();
  static int height = queue->height();
  int h = queue->height();
  if (h != height) {
    MESSAGE("Height change in determine loss old: " << height << " new: " << h);
    height = h;
  }
  return queue->height() > lossHeight();
}

/* End ComputerPlayerAI */

int EasyAI::baseSteps()
{
  //cout << "easy baseSteps" << endl;
  int a = ComputerPlayerAI::baseSteps() * 15;
  return a;
}

int EasyAI::lossHeight()
{
  return 4;
}

int MediumAI::baseSteps()
{
  return ComputerPlayerAI::baseSteps() * 10;
}

int MediumAI::lossHeight()
{
  return 10;
}

/* Begin HardAI */
int HardAI::baseSteps()
{
  return ComputerPlayerAI::baseSteps() * 5;
}

int HardAI::lossHeight()
{
  return 20;
}

/* End HardAI */
