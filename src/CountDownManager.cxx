/*
 * CountDownManager.cxx
 * Daniel Nelson - 10/27/0
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
 * Very simple, but I need somewhere to put this code.
 */

#include "Game.h"
#include "CountDownManager.h"
#include "MessageManager.h"
#include "Displayer.h"

using namespace std;

int CountDownManager::state;
int CountDownManager::start_pause_alarm;
int CountDownManager::message_switch_alarm;

void CountDownManager::gameStart (   )
{
  start_pause_alarm = GC_START_PAUSE_DELAY;
  message_switch_alarm = GC_START_PAUSE_DELAY / 3;
  state = 3;

  MessageManager::readyMessage(state);
}

void CountDownManager::cleanUp (   )
{
  if (state != -1) MessageManager::freeMessage();
}

void CountDownManager::timeStep_inline_split_ (   )
{
  if (start_pause_alarm)
    start_pause_alarm--;

  if (--message_switch_alarm == 0)
    if (--state == -1) {
      MessageManager::freeMessage();
      message_switch_alarm = 0;

    } else if (state == MS_COUNT_DOWN_GO) {
      MessageManager::freeMessage();
      MessageManager::readyMessage(MS_COUNT_DOWN_GO);
      message_switch_alarm = GC_START_PAUSE_DELAY / 3;

    } else {
      MessageManager::nextMessage(state);
      message_switch_alarm = GC_START_PAUSE_DELAY / 3;
    }
}

