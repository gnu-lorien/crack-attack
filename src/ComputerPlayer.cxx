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
#include "Controller.h"
#include "BlockManager.h"
#include "Grid.h"
#include "Swapper.h"

//#define WAIT_TIME ( GC_STEPS_PER_SECOND * 10 )

bool ComputerPlayer::lost;
bool ComputerPlayer::_impact;
ComputerPlayerAI *ComputerPlayer::ai;
int ComputerPlayer::start_time = 0;
int ComputerPlayer::alarm = 0;
std::vector< std::pair< int, int > > ComputerPlayer::path;

void ComputerPlayer::gameStart()
{
  /*
  if (!(MetaState::mode & CM_AI))
    return;
  */

  /*
  if ((MetaState::mode & CM_AI_EASY))
    ai = new EasyAI();
  if ((MetaState::mode & CM_AI_MEDIUM))
    ai = new MediumAI();
  if ((MetaState::mode & CM_AI_HARD))
    ai = new HardAI();
  */
  ai = new EasyAI();

  assert(ai != NULL);
  
  start_time = Game::time_step;
  alarm = start_time + GC_MOVE_DELAY;
  lost = false;
  //path.push_back(std::make_pair(50, GC_LEFT_KEY));
  static int hunting_for_flavor = 1;
  for (int x = 0; x < GC_PLAY_WIDTH; ++x) {
    for (int y = 0; y < GC_PLAY_HEIGHT; ++y) {
      if (GR_BLOCK == Grid::residentTypeAt(x, y)) {
        if (Grid::flavorAt(x, y) == hunting_for_flavor) {
          // Path to this one and swap it!
          int x_move = 0, y_move = 0;
          int dir, inc;
          x_move = Swapper::x - x;
          y_move = Swapper::y - y;
          char lame[255];
          snprintf(lame, 255, "Move from (%d,%d) to (%d,%d)",
              Swapper::x, Swapper::y,
              x, y);
          MESSAGE(lame);
          if (!(0 == x_move)) {
            if (x_move < 0) {
              dir = GC_RIGHT_KEY;
              inc = 1;
            } else {
              dir = GC_LEFT_KEY;
              inc = -1;
            }
            for (; x_move != 0; x_move += inc) {
              path.push_back(std::make_pair(GC_MOVE_DELAY + 2, dir));
            }
          }
          if (!(0 == y_move)) {
            if (y_move < 0) {
              dir = GC_UP_KEY;
              inc = 1;
            } else {
              dir = GC_DOWN_KEY;
              inc = -1;
            }
            for (; y_move != 0; y_move += inc) {
              path.push_back(std::make_pair(GC_MOVE_DELAY + 2, dir));
            }
          }
          goto cunt;
        }
      }
    }
  }
cunt:
  alarm = start_time + path[0].first;
}

int ComputerPlayer::gameFinish()
{
  return ai->determineLoss() ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  static bool first_time = true;
  static int alternating = 0;
  /*
  if (!(MetaState::mode & CM_AI))
    return;
  if (!ai) {
    return;
  }
  */

  
  if (Game::time_step >= alarm && alarm != -1) {
    char lame[255];
    Controller::entry(GLUT_LEFT);
    if (!path.empty()) {
      snprintf(lame, 255, "Executing %d on alarm %d at step %d (%d,%d)",
          path[0].second,
          alarm,
          Game::time_step,
          Swapper::x,
          Swapper::y);
      MESSAGE(lame);
      Controller::keyboardPlay(path[0].second, 0, 0);
    }
    path.erase(path.begin());
    if (!path.empty()) {
      alarm = Game::time_step + path[0].first;
    } else {
      snprintf(lame, 255, "Path is empty. No new alarm to set (%d,%d)",
          Swapper::x,
          Swapper::y);
      MESSAGE(lame);
      alarm = -1;
    }
  }
  // handle the lights
  LevelLights::handleAI();

  /*
  ComputerPlayerAI &localAi = *ai;
  if (first_time) {
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    first_time = false;
  }
  if (Game::time_step >= localAi.alarm()) {
    localAi.garbageAmount()->sendToGenerator();
#ifndef NDEBUG
    std::cout << "init pop: " << GC_INITIAL_POP_DELAY << std::endl;
    std::cout << "steps per second: " << GC_STEPS_PER_SECOND << std::endl;
    std::cout << "Height: " << ai->garbageQueue()->height() << std::endl;
#endif
    localAi.resetAlarm();
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
  }
  if(localAi.determineLoss()) {
    Game::aiPlayerLoss();
  }
  */
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
