/*
 * Block.cxx
 * Daniel Nelson - 8/21/0
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
 * The block object, of which each block is one.
 */

#include "Game.h"
#include "Block.h"
#include "BlockManager.h"
#include "ComboTabulator.h"
#include "Garbage.h"
#include "Grid.h"
#include "Random.h"
#include "Swapper.h"
#include "SparkleManager.h"
#include "X.h"

using namespace std;

void Block::initializeStatic ( int _x, int _y, int _flavor )
{
  x = _x;
  y = _y;
  flavor = _flavor;
  f_y = 0;

  state = BS_STATIC;
  alarm = 0;
  // pop_alarm = 0;
  current_combo = null;

  // add ourselves to the grid
  Grid::addBlock(x, y, this, GR_BLOCK);

  // if we're wild, the extreme code needs to know
  if (flavor == BF_WILD)
    X::activateWild(*this);
  else if (BlockManager::isSpecialColorFlavor(flavor))
    X::activateSpecialColor(*this);
}

void Block::initializeAwaking ( int _x, int _y, int _flavor, int pop_delay,
 int awake_delay, ComboTabulator *combo, int _pop_color )
{
  x = _x;
  y = _y;
  flavor = _flavor;
  f_y = 0;

  state = BS_AWAKING;
  alarm = Game::time_step + awake_delay;
  pop_alarm = Game::time_step + pop_delay;
  pop_direction = BlockManager::generatePopDirection();
  pop_color = _pop_color;
  current_combo = combo;

  // let the combo know we're involved
  current_combo->incrementInvolvement();

  // change the game state
  Game::awaking_count++;

  // add ourselves to the grid
  Grid::addBlock(x, y, this, GR_IMMUTABLE);
}

void Block::timeStep (   )
{
  if (state & BS_STATIC) {
    // We may have to fall.

    if (Grid::stateAt(x, y - 1) & GR_EMPTY)
      startFalling();
    else
      return;

  } else if (state & BS_AWAKING) {
    // The alarm has been set to go off when we're done awaking.  When the
    // pop alarm goes off, we only switch our appearence.
    if (pop_alarm == Game::time_step)
      pop_alarm = 0;

    if (alarm == Game::time_step) {

      // change the game state
      Game::awaking_count--;

      // change our state; startFalling() and eliminations check for
      // BS_STATIC state
      state = BS_STATIC;

      // if we're going to fall
      if (Grid::stateAt(x, y - 1) & GR_EMPTY)
        startFalling(current_combo, true);

      else {
        // update the grid
        Grid::changeState(x, y, this, GR_BLOCK);

        // register for elimination checking
        Grid::requestEliminationCheck(*this, current_combo);
      }

    } else
      return;
  }

  // Deal with all other states.

  if (state & BS_FALLING) {
    // We are assured that the timeStep() of any blocks below us has already
    // been called.  Note that to start a fall, all we have to do is set our
    // state to BS_FALLING.  This code will deal with the rest.

    if (alarm == Game::time_step)
      // hang alarm goes off
      alarm = 0;

    // if the hang alarm has gone off
    if (alarm == 0) {

      // if we're at the bottom of a grid element
      if (f_y == 0) {

        // if we're still going to fall
        if (Grid::stateAt(x, y - 1) & GR_EMPTY) {

          // shift our grid position down to the next row
          y--;
          f_y = GC_STEPS_PER_GRID;

          // update the grid
          Grid::remove(x, y + 1, this);
          Grid::addBlock(x, y, this, GR_FALLING);

        // if we've landed
        } else {

          // change our state
          state = BS_STATIC;

          // update the grid
          Grid::changeState(x, y, this, GR_BLOCK);

          // register for elimination checking
          Grid::requestEliminationCheck(*this, current_combo);

          // if the block below us is swapping, we may have to switch it's combo
          if (current_combo)
            Swapper::notifyLanding(x, y, *this, current_combo);
        }
      }

      // if we still are, fall
      if (state & BS_FALLING)
        f_y -= GC_FALL_VELOCITY;
    }

  } else if (state & BS_DYING) {
    // The alarm has been set to go off when we're done dying.

    if (--alarm == 0) {

      // change the game state
      Game::dying_count--;
      Game::dying_count_2--;

      // update the grid
      Grid::remove(x, y, this);

      // tell our upward neighbor to start a combo fall
      if (y < GC_PLAY_HEIGHT - 1) {
        if (Grid::stateAt(x, y + 1) & GR_BLOCK)
          Grid::blockAt(x, y + 1).startFalling(current_combo);
        else if (Grid::stateAt(x, y + 1) & GR_GARBAGE)
          Grid::garbageAt(x, y + 1).startFalling(current_combo);
      }

      // let the combo know we're out
      current_combo->decrementInvolvement();

      // generate some sparkles; pop_alarm stores the number
      if (flavor != BF_WILD)
        SparkleManager::createBlockDeathSpark(x, y, flavor, pop_alarm);
      else
        SparkleManager::createBlockDeathSpark(x, y, X::wildFlavor(*this),
         pop_alarm);

      // if we're wild, the extreme code needs to know
      if (flavor == BF_WILD)
        X::deactivateWild(*this);
      else if (BlockManager::isSpecialColorFlavor(flavor))
        X::deactivateSpecialColor();

      // delete ourselves
      BlockManager::deleteBlock(this);

    // if we just started dying
    } else if (alarm == GC_DYING_DELAY - 1)
      // grab the elimination magnitude from our combo
      pop_alarm = current_combo->latest_magnitude;
  }
}

void Block::startFalling ( ComboTabulator *combo, bool no_hang )
/*
 * Although blocks will fall on their own, this must be called to syncronize
 * and connect that falling with a elimination combo.
 */
{
  if (!(state & BS_STATIC)) return;

  // change our state
  state = BS_FALLING;

  // set the hang alarm and update the grid
  if (no_hang) {
    alarm = 0;
    Grid::changeState(x, y, this, GR_FALLING);

  } else {
    alarm = Game::time_step + GC_HANG_DELAY;
    Grid::changeState(x, y, this, GR_HANGING | GR_FALLING);
  }

  // let the combo know we're involved
  if (combo)
    beginComboInvolvement(combo);

  // tell our upward neighbor to start a combo fall
  if (y < GC_PLAY_HEIGHT - 1) {
    if (Grid::stateAt(x, y + 1) & GR_BLOCK)
      Grid::blockAt(x, y + 1).startFalling(current_combo, no_hang);
    else if (Grid::stateAt(x, y + 1) & GR_GARBAGE)
      Grid::garbageAt(x, y + 1).startFalling(current_combo, no_hang);
  }
}

void Block::startDying ( ComboTabulator *combo, int spark_number )
{
  // change the game state
  Game::dying_count++;
  Game::dying_count_2++;

  // let the combo know we're in
  beginComboInvolvement(combo);

  // change our state
  state = BS_DYING;

  // set the alarm; the alarm works this way due to display needs
  alarm = GC_DYING_DELAY;

  // update the grid
  Grid::changeState(x, y, this, GR_IMMUTABLE);

  // generate a random rotation axis
  Random::angle(axis_x, axis_y);
}

void Block::startSwapping ( int direction )
{
  // change our state and swap direction
  state = BS_SWAPPING | (direction & SA_RIGHT ? BS_SWAP_DIRECTION_MASK : 0);

  // update the grid
  Grid::changeState(x, y, this, GR_IMMUTABLE);
}

void Block::finishSwapping ( int s_x )
{
  // change our state
  state = BS_STATIC;
  x = s_x;

  // update the grid
  Grid::addBlock(x, y, this, GR_BLOCK);
}

