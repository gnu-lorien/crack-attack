/*
 * Swapper.cxx
 * Daniel Nelson - 8/22/0
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
 * The thingy you swap blocks with.
 */

using namespace std;

#include "Game.h"
#include "Swapper.h"
#include "Grid.h"
#include "Block.h"
#include "Controller.h"
#include "ComboTabulator.h"
#include "ComboManager.h"
#include "CountDownManager.h"
#include "X.h"

int Swapper::x, Swapper::y;
int Swapper::state;
int Swapper::swap;
int Swapper::swap_alarm;
int Swapper::move_pause_alarm;
bool Swapper::button_down_swap;
int Swapper::button_down_move;
Block *Swapper::left_block, *Swapper::right_block;
int Swapper::queued_move;
bool Swapper::queued_swap;
float Swapper::swap_factor;
int Swapper::color;

void Swapper::gameStart (   )
{
  x = GC_INITIAL_SWAPPER_LOCATION_X;
  y = GC_INITIAL_SWAPPER_LOCATION_Y;

  state = 0;

  swap_alarm = 0;
  move_pause_alarm = 0;

  button_down_swap = false;
  button_down_move = 0;

  queued_move = 0;
  queued_swap = false;

  color = 0;
}

void Swapper::timeStep (   )
/*
 * If we're swapping, check to see if we're done.  Check the keyboard for any
 * commands which we must react to.
 */
{
  if (!Controller::moveCommand())
    button_down_move = 0;
  if (!Controller::swapCommand())
    button_down_swap = false;

  if (state & SS_MOVE_PAUSE)
    // if it's time, unset the move pause state
    if (move_pause_alarm == Game::time_step)
      state &= ~SS_MOVE_PAUSE;
      
    // otherwise, listen to queue a swap
    else if (!button_down_swap && Controller::swapCommand()) {
      if (queued_move) queued_move = 0;
      queued_swap = true;
      button_down_swap = true;

    // otherwise, listen to queue a move
    } else if (Controller::moveCommand()
     && button_down_move != Controller::moveCommand() && !queued_swap)
      queued_move = button_down_move = Controller::moveCommand();

  if (state & SS_SWAPPING)
    if (swap_alarm == Game::time_step) {

      // unset the swap state
      state &= ~SS_SWAPPING;

      // swap the blocks

      if (swap & SA_LEFT)
        Grid::remove(x, y, left_block);
      if (swap & SA_RIGHT)
        Grid::remove(x + 1, y, right_block);

#ifndef NDEBUG
      // otherwise the assert bites us
      if (!(swap & SA_LEFT))
        Grid::changeState(x, y, null, GR_EMPTY);
      if (!(swap & SA_RIGHT))
        Grid::changeState(x + 1, y, null, GR_EMPTY);
#endif

      if (swap & SA_LEFT)
        left_block->finishSwapping(x + 1);
      if (swap & SA_RIGHT)
        right_block->finishSwapping(x);

      // register blocks for elimination checking

      // if two blocks moved, we need a combo tabulator to link potential
      // pattern matches
      ComboTabulator *combo = (swap == (SA_LEFT | SA_RIGHT)
       ? &ComboManager::newComboTabulator() : null);

      if (swap & SA_LEFT)
        Grid::requestEliminationCheck(*left_block, combo);
      if (swap & SA_RIGHT)
        Grid::requestEliminationCheck(*right_block, combo);

      // listen to queue a move
      if (!button_down_move && Controller::moveCommand())
        queued_move = button_down_move = Controller::moveCommand();

      // no commands when we're swapping
      return;

    } else
      swap_factor = (Game::time_step - Swapper::swap_alarm)
       * (1.0f / (float) GC_SWAP_DELAY);

  if (state & SS_SWAPPING) {
    // listen to queue a move
    if (!button_down_move && Controller::moveCommand())
      queued_move = button_down_move = Controller::moveCommand();

    // no commands when we're swapping
    return;
  }

  // if we're not in a move pause or swap
  if (!(state & (SS_MOVE_PAUSE | SS_SWAPPING)) && (queued_move
   || (Controller::moveCommand()
   && button_down_move != Controller::moveCommand()))) {
    switch (queued_move ? queued_move : Controller::moveCommand()) {
    case CC_LEFT:
      if (!X::reverseControls()) {
        if (x > 0) {
          x--;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_LEFT | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_LEFT;
        }
      } else {
        if (x < GC_PLAY_WIDTH - 2) {
          x++;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_RIGHT | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_LEFT;
        }
      }
      break;
    case CC_RIGHT:
      if (!X::reverseControls()) {
        if (x < GC_PLAY_WIDTH - 2) {
          x++;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_RIGHT | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_RIGHT;
        }
      } else {
        if (x > 0) {
          x--;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_LEFT | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_RIGHT;
        }
      }
      break;
    case CC_UP:
      if (!X::reverseControls()) {
        if (y < GC_SAFE_HEIGHT - 1) {
          y++;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_UP | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_UP;
        }
      } else {
        if (y > 1) {
          y--;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_DOWN | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_UP;
        }
      }
      break;
    case CC_DOWN:
      if (!X::reverseControls()) {
        if (y > 1) {
          y--;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_DOWN | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_DOWN;
        }
      } else {
        if (y < GC_SAFE_HEIGHT - 1) {
          y++;
          state = (state & ~SS_MOVE_MASK) | (SS_MOVE_UP | SS_MOVE_PAUSE);
          move_pause_alarm = Game::time_step + GC_MOVE_DELAY;
          button_down_move = CC_DOWN;
        }
      }
      break;
    }
    queued_move = 0;
  }

  if (!(state & (SS_MOVE_PAUSE | SS_SWAPPING)) && (queued_swap
   || (Controller::swapCommand() && !button_down_swap))
   && CountDownManager::start_pause_alarm == 0) {

    button_down_swap = true;
    queued_swap = false;

    swap = 0;
    if (Grid::stateAt(x, y) & GR_BLOCK) {
      left_block = &Grid::blockAt(x, y);
      swap |= SA_LEFT;
    } else if (!(Grid::stateAt(x, y) & GR_EMPTY)
     || Grid::stateAt(x, y - 1) & GR_FALLING
     || Grid::stateAt(x, y + 1) & GR_HANGING)
      swap |= SA_DISALLOWED;

    if (Grid::stateAt(x + 1, y) & GR_BLOCK) {
      right_block = &Grid::blockAt(x + 1, y);
      swap |= SA_RIGHT;
    } else if (!(Grid::stateAt(x + 1, y) & GR_EMPTY)
     || Grid::stateAt(x + 1, y - 1) & GR_FALLING
     || Grid::stateAt(x + 1, y + 1) & GR_HANGING)
      swap |= SA_DISALLOWED;

    // if swap is allowed
    if (!(swap & SA_DISALLOWED) && swap != 0) {

      // change our state
      state |= SS_SWAPPING;

      // set the alarm
      swap_alarm = Game::time_step + GC_SWAP_DELAY;

      // start the swapping
      if (swap & SA_LEFT)
        left_block->startSwapping(SA_RIGHT);
      else
        Grid::changeState(x, y, null, GR_IMMUTABLE);

      if (swap & SA_RIGHT)
        right_block->startSwapping(SA_LEFT);
      else
        Grid::changeState(x + 1, y, null, GR_IMMUTABLE);

      // initialize the swap factor
      swap_factor = -1.0f;
    }
  }
}
