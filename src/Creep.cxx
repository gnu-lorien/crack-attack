/*
 * Creep.cxx
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
 * Handles the upward creepiness of the game.
 *
 * How to we do a creep roll over?  Right now we do a big copy up.  We could use
 * a rolling data structure, but the question is, would the time saved here be
 * worth the extra dereference we'd have to do each time we access the grid?
 */

#include "Game.h"
#include "Creep.h"
#include "Grid.h"
#include "BlockManager.h"
#include "Controller.h"
#include "ComboManager.h"
#include "Displayer.h"
#include "LevelLights.h"
#include "LoseBar.h"

using namespace std;

int Creep::creep;
int Creep::creep_timer_step;
int Creep::creep_timer;
int Creep::increase_velocity_alarm;
int Creep::loss_alarm;
bool Creep::creep_freeze;
bool Creep::advance;

void Creep::gameStart (   )
{
  creep = 0;

  creep_timer_step = GC_CREEP_INITIAL_TIMER_STEP;
  creep_timer = 0;

  increase_velocity_alarm = Game::time_step + GC_CREEP_INCREMENT_DELAY;

  creep_freeze = false;

  advance = false;

  BlockManager::newCreepRow();
}

void Creep::timeStep (   )
{
  // if it's time to increase the creep speed
  if (increase_velocity_alarm == Game::time_step) {

    if (creep_timer_step == GC_CREEP_MAX_TIMER_STEP)
      increase_velocity_alarm = 0;
    else {
      increase_velocity_alarm = Game::time_step + GC_CREEP_INCREMENT_DELAY;
      creep_timer_step += GC_CREEP_TIMER_STEP_INCREMENT;
    }
  }

  // no creeping when awaking or dying; not a true creep freeze
  if (Game::awaking_count != 0 || Game::dying_count != 0) {
    // if we're in a creep freeze
    if (creep_freeze) {

      // you can't lose within a certain delay of your last elimination
      if (loss_alarm < GC_LOSS_DELAY_ELIMINATION) {
        loss_alarm = GC_LOSS_DELAY_ELIMINATION;
        LoseBar::highAlertReset();
      }

      // check to end it
      if (!Grid::checkSafeHeightViolation())
        creep_freeze = false;
    }

    return;
  }

  // if we're in a creep freeze
  if (creep_freeze) {

    // check to end it
    if (!Grid::checkSafeHeightViolation())
      creep_freeze = false;

    else {
      // check for game loss
      if (--loss_alarm == 0)
        Game::loss();

      // and don't creep
      return;
    }

  // check to see if we're pushing against GC_SAFE_HEIGHT
  } else if (Grid::checkSafeHeightViolation()) {
    // freeze the creep for one creap cycle
    creep_freeze = true;
    loss_alarm = GC_LOSS_DELAY;
    LevelLights::notifySafeHeightViolation();
  }

  if (advance || Controller::advanceCommand()) {
    if (creep_timer_step < GC_CREEP_ADVANCE_TIMER_STEP)
      creep_timer += GC_CREEP_ADVANCE_TIMER_STEP;
    else
      creep_timer += creep_timer_step;
    advance = true;
  } else
    creep_timer += creep_timer_step;

  // if it's time to creep
  while (creep_timer >= GC_CREEP_DELAY) {
    creep_timer -= GC_CREEP_DELAY;

    // otherwise, creep as normal
    if (!advance)
      creep++;

    // if we're going to creep fast
    else {
      creep += GC_CREEP_ADVANCE_VELOCITY;
      if (creep > GC_STEPS_PER_GRID)
        creep = GC_STEPS_PER_GRID;
    }

    // if we've completed a grid of creeping
    if (creep == GC_STEPS_PER_GRID) {
      creep = 0;

      // shift everything up one grid
      if (Grid::shiftGridUp()) {

        // create a new bottom row
        BlockManager::newCreepRow();

        // link the elimination requests
        ComboTabulator &combo = ComboManager::newComboTabulator();
        for (int x = GC_PLAY_WIDTH; x--; )
          Grid::requestEliminationCheck(Grid::blockAt(x, 1), &combo);

      // if we can't shift up, don't; but try again next time
      } else {
        creep_timer += GC_CREEP_DELAY;
        creep = GC_STEPS_PER_GRID - 1;
      }

      if (advance && !Controller::advanceCommand())
        advance = false;
    }
  }
}
