/*
 * ComputerPlayer.cxx
 * Andrew Sayman - 3/27/05
 *
 * Copyright (C) 2005  Andrew Sayman
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

#include "ComputerPlayer.h"

#include <cassert>
#include "LevelLights.h"
#include "Score.h"


//#define WAIT_TIME ( GC_STEPS_PER_SECOND * 10 )

bool ComputerPlayer::lost;
bool ComputerPlayer::_impact;
ComputerPlayerAI *ComputerPlayer::ai;

void ComputerPlayer::gameStart()
{
  if (!(MetaState::mode & CM_AI))
    return;

  if ((MetaState::mode & CM_AI_EASY))
    ai = new EasyAI();
  if ((MetaState::mode & CM_AI_MEDIUM))
    ai = new MediumAI();
  if ((MetaState::mode & CM_AI_HARD))
    ai = new HardAI();

  assert(ai != NULL);
  
  lost = false;
}

int ComputerPlayer::gameFinish()
{
  return ai->determineLoss() ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  static bool first_time = true;
  if (!(MetaState::mode & CM_AI))
    return;
  if (!ai) {
    return;
  }
  
  // handle the lights
  LevelLights::handleAI();

  ComputerPlayerAI &localAi = *ai;
  if (first_time) {
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    first_time = false;
  }
  if (Game::time_step >= localAi.alarm()) {
    localAi.garbageAmount()->sendToGenerator();
#ifndef NDEBUG
    cout << "init pop: " << GC_INITIAL_POP_DELAY << endl;
    cout << "steps per second: " << GC_STEPS_PER_SECOND << endl;
    cout << "Height: " << ai->garbageQueue()->height() << endl;
#endif
    localAi.resetAlarm();
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
  }
  if(localAi.determineLoss()) {
    Game::aiPlayerLoss();
  }
}

void ComputerPlayer::addGarbage ( int height, int width, int flavor ) {
  assert(ai != NULL);
  MESSAGE("Adding garbage to queue");
  ai->garbageQueue()->add(height, width, flavor);
  _impact = true;
}

bool ComputerPlayer::checkLevelLightDying()
{
  int height = ai->garbageQueue()->height();
  int ninety = (int)(ai->lossHeight() * .9);
  if (ninety == ai->lossHeight())
    ninety = ai->lossHeight() - 1;
  if (height >= ninety)
    return true;
  return false;
}

double ComputerPlayer::lightPartition (int n)
{
  static double max = LL_NUMBER_LEVEL_LIGHTS;
  double lh = ai->lossHeight();
  double partition = lh / max;
  double colorh = n * partition;
  return colorh;
}

bool ComputerPlayer::checkLevelLightBlue(int n)
{
  if (lightPartition(n) >= ai->garbageQueue()->height())
    return true;
  else
    return false;
}

int ComputerPlayer::findTopRed()
{
  for (int i=0; i < LL_NUMBER_LEVEL_LIGHTS; ++i) {
    if (lightPartition(i) >= ai->garbageQueue()->height())
      return i;
  }
	return 0;
}

int ComputerPlayer::levelLightImpact (  )
{
  if (impact(true))
    return findTopRed();

	return 0;
}

bool ComputerPlayer::impact (bool reset)
{
  bool ret = _impact;
  if (reset) _impact = false;
  return ret;
}
