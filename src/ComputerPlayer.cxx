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
std::vector< PathPortion > ComputerPlayer::path;
int ComputerPlayer::target_x = -1;
int ComputerPlayer::target_y = -1;

static std::vector< PathPortion > path_between(int start_x, int start_y, int end_x, int end_y, int move_delay = GC_MOVE_DELAY)
{
  std::vector< PathPortion > ret_path;
  int x_move = 0, y_move = 0;
  int dir, inc;
  int bound_end_x = end_x;
  int current_x, current_y;

  if (bound_end_x > GC_PLAY_WIDTH - 2) {
    bound_end_x = GC_PLAY_WIDTH - 2;
  }

  x_move = start_x - bound_end_x;
  y_move = start_y - end_y;

  char lame[255];
  snprintf(lame, 255, "Move from (%d,%d) to (%d,%d)",
      start_x, start_y,
      bound_end_x, end_y);
  MESSAGE(lame);

  current_x = start_x; current_y = start_y;
  if (!(0 == x_move)) {
    if (x_move < 0) {
      dir = GC_RIGHT_KEY;
      inc = 1;
    } else {
      dir = GC_LEFT_KEY;
      inc = -1;
    }
    for (; x_move != 0; x_move += inc) {
      PathPortion p;
      p.alarm = move_delay;
      p.key_action = dir;
      p.target_x = bound_end_x;
      p.target_y = end_y;
      ret_path.push_back(p);
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
      PathPortion p;
      p.alarm = move_delay;
      p.key_action = dir;
      p.target_x = bound_end_x;
      p.target_y = end_y;
      ret_path.push_back(p);
    }
  }


  return ret_path;
}

static void path_all_for_flavor(std::vector< PathPortion > &my_path, int hunting_for_flavor)
{
  int move_delay = GC_MOVE_DELAY;
  int swap_x = Swapper::x, swap_y = Swapper::y;

  if (!my_path.empty())
  {
    PathPortion p = my_path[my_path.size() - 1];
    swap_x = p.target_x;
    swap_y = p.target_y;
  }

  for (int x = 0; x < GC_PLAY_WIDTH; ++x) {
    for (int y = 1; y < (Grid::top_effective_row + 1); ++y) {
      if (GR_BLOCK == Grid::residentTypeAt(x, y)) {
        if (Grid::flavorAt(x, y) == hunting_for_flavor) {
          std::vector< PathPortion > additional_path = path_between(
              swap_x, swap_y,
              x, y);
          if (!additional_path.empty())
            my_path.insert(my_path.end(), additional_path.begin(), additional_path.end());
          swap_x = x; swap_y = y;
          if (swap_x > GC_PLAY_WIDTH - 2) {
            swap_x = GC_PLAY_WIDTH - 2;
          }
        }
      }
    }
  }
}

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
  for (int i = 0; i < 5; ++i) {
    path_all_for_flavor(path, i);
  }

  assert(!path.empty());
  alarm = start_time + path[0].alarm;
}

int ComputerPlayer::gameFinish()
{
  return ai->determineLoss() ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  static bool first_time = true;
  static bool need_key_up = false;
  /*
  if (!(MetaState::mode & CM_AI))
    return;
  if (!ai) {
    return;
  }
  */

  
  if (Game::time_step >= alarm && alarm != -1) {
    char lame[255];
    if (!need_key_up) {
      if (!path.empty()) {
        snprintf(lame, 255, "Executing %d on alarm %d at step %d (%d,%d)",
            path[0].key_action,
            alarm,
            Game::time_step,
            Swapper::x,
            Swapper::y);
        MESSAGE(lame);
        Controller::keyboardPlay(path[0].key_action, 0, 0);
        target_x = path[0].target_x;
        target_y = path[0].target_y;
        snprintf(lame, 255, "Target (%d,%d)", target_x, target_y);
        MESSAGE(lame);
        alarm = Game::time_step + 1;
        need_key_up = true;
      } else {
        MESSAGE("Trying to activate an empty path!");
      }
    } else {
      if (!path.empty()) {
        Controller::keyboardUpPlay(path[0].key_action, 0, 0);
        need_key_up = false;
      }
      path.erase(path.begin());
      if (!path.empty()) {
        alarm = alarm + path[0].alarm;
      } else {
        snprintf(lame, 255, "Path is empty. No new alarm to set (%d,%d)",
            Swapper::x,
            Swapper::y);
        MESSAGE(lame);
        alarm = -1;
        target_x = -1;
        target_y = -1;
      }
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
