/*
 * Garbage.cxx
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
 * The garbage blocks is what this details.
 */

#include "Game.h"
#include "Grid.h"
#include "Garbage.h"
#include "BlockManager.h"
#include "GarbageManager.h"
#include "Displayer.h"
#include "Spring.h"
#include "Random.h"
#include "X.h"

using namespace std;

void Garbage::initializeStatic ( int _x, int _y, int _height, int _width,
 int _flavor )
{
  x = _x;
  y = _y;
  height = _height;
  width = _width;
  flavor = _flavor;
  f_y = 0;

  state = GS_STATIC;
  alarm = 0;
  pop_alarm = 0;
  sections_popped = 0;
  initial_fall = false;
  awaking_combo = null;

  // add ourselves to the grid
  for (int h = height; h--; )
    for (int w = width; w--; )
      Grid::addGarbage(x + w, y + h, this, GR_GARBAGE);
}

void Garbage::initializeFalling ( int _x, int _y, int _height, int _width,
 int _flavor )
{
  x = _x;
  y = _y;
  height = _height;
  width = _width;
  flavor = _flavor;
  f_y = 0;

  state = GS_FALLING;
  alarm = Game::time_step + GC_HANG_DELAY;
  pop_alarm = 0;
  sections_popped = 0;
  initial_fall = true;
  awaking_combo = null;

  // add ourselves to the grid
  for (int h = height; h--; )
    for (int w = width; w--; )
      Grid::addGarbage(x + w, y + h, this, GR_FALLING);
}

void Garbage::initializeAwaking ( int _x, int _y, int _height, int pop_delay,
 int awake_delay, ComboTabulator *combo, int _pop_color )
{
  x = _x;
  y = _y;
  height = _height;
  width = GC_PLAY_WIDTH;
  flavor = GF_NORMAL;
  f_y = 0;

  state = GS_AWAKING;
  alarm = Game::time_step + awake_delay;
  pop_alarm = Game::time_step + pop_delay;
  sections_popped = 0;
  pop_direction = BlockManager::generatePopDirection(height * width);
  pop_color = _pop_color;
  initial_fall = false;

  // Although garbage does not participate in combos as such, this is needed
  // so that the garbage can pass the combo on to blocks above it if it falls
  // when it awakes.  Note that this never happens because garbage currently
  // only awakes as the second row of a three row or taller shattering
  // garbage.  Thus, all blocks above it will be BS_AWAKING when the garbage
  // calls their startFalling().
  awaking_combo = combo;

  // change the game state
  Game::awaking_count++;

  // add ourselves to the grid
  for (int h = height; h--; )
    for (int w = width; w--; )
      Grid::addGarbage(x + w, y + h, this, GR_IMMUTABLE);
}

void Garbage::timeStep ( int &l_x, int &l_y )
{
  // We must advance l_x and l_y based on our size.

  // normal garbage dimensions
  if (height == 1 || width == GC_PLAY_WIDTH) {
    l_y += height - 1;
    l_x += width - 1;

  // special garbage dimensions
  } else {
    l_x += width - 1;
    // if it's not our top row, don't time step
    if (l_y != y + height - 1) return;
  }

  // First, the states that may change do to falling.

  if (state & GS_STATIC) {
    // We may have to fall.

    bool flag = true;
    for (int w = width; w--; )
      if (!(Grid::stateAt(x + w, y - 1) & GR_EMPTY)) {
        flag = false;
        break;
      }
    if (flag)
      startFalling();

  } else if (state & GS_AWAKING) {
    // The alarm has been set to go off when we're done awaking.  When the pop
    // alarm goes off, we have to pop one more of our sections.  If that's the
    // last section, we don't reset the pop timer.  In about a million places,
    // we assume that awaking garbage is as wide as the grid.

    if (sections_popped < width * height)
      if (pop_alarm == Game::time_step) {
        sections_popped++;
        if (sections_popped < width * height) {
          if (pop_direction & (1 << 3))
            pop_direction = (1 << 0);
          else
            pop_direction <<= 1;
          pop_alarm = Game::time_step + GC_INTERNAL_POP_DELAY;
        }
      }

    if (alarm == Game::time_step) {

      // change the game state
      Game::awaking_count--;
 
      // if we're going to fall
      bool flag = true;
      for (int w = width; w--; )
        if (!(Grid::stateAt(x + w, y - 1) & GR_EMPTY)) {
          flag = false;
          break;
        }
      if (flag)

        startFalling(awaking_combo, true, true);

      else {

        // change our state
        state = GS_STATIC;

        // update the grid
        for (int h = height; h--; )
          for (int w = width; w--; )
            Grid::changeState(x + w, y + h, this, GR_GARBAGE);
      }
    }
  }

  // Deal with all other states.

  if (state & GS_FALLING) {
    // We are assured that the timeStep() of any blocks below us has already
    // been called.  Note that to start a fall, all we have to do is set our
    // state to GS_FALLING.  This code will deal with the rest.

    if (alarm == Game::time_step)
      // hang alarm goes off
      alarm = 0;

    // if the hang alarm has gone off
    if (alarm == 0) {
    
      // if we're at the bottom of a grid element
      if (f_y == 0) {

        // if we're still going to fall
        bool flag = true;
        for (int w = width; w--; )
          if (!(Grid::stateAt(x + w, y - 1) & GR_EMPTY)) {
            flag = false;
            break;
          }
        if (flag) {

          // shift our grid position down to the next row
          y--;
          f_y = GC_STEPS_PER_GRID;

          // update the grid
          for (int h = height; h--; )
            for (int w = width; w--; )
              Grid::remove(x + w, y + h + 1, this);
          for (int h = height; h--; )
            for (int w = width; w--; )
              Grid::addGarbage(x + w, y + h, this, GR_FALLING);

        // if we've landed
        } else {

          // change our state
          state = BS_STATIC;

          // if this is the end of our initial fall
          if (initial_fall) {
            initial_fall = false;
            Spring::notifyImpact(height, width);
            Grid::notifyImpact(y, height);

            X::notifyImpact(*this);
          }

          // update the grid
          for (int h = height; h--; )
            for (int w = width; w--; )
              Grid::changeState(x + w, y + h, this, GR_GARBAGE);
        }
      }

      // if we still are, fall
      if (state & GS_FALLING)
        f_y -= GC_FALL_VELOCITY;
    }
  }
}

void Garbage::startFalling ( ComboTabulator *combo, bool no_hang,
 bool self_call )
/*
 * While garbage doesn't have a current combo and doesn't have to deal with
 * such things, it does need to pass combo falls along to it's upward neighbors.
 */
{
  // if we're calling our own startFalling() this has already been checked
  if (!self_call) {

    if (!(state & BS_STATIC)) return;

    // if we're not going to fall
    for (int w = width; w--; )
      if (!(Grid::stateAt(x + w, y - 1) & (GR_EMPTY | GR_FALLING)))
        return;
  }

  // change our state
  state = GS_FALLING;

  // set the hang alarm and update the grid
  if (no_hang) {
    alarm = 0;
    for (int h = height; h--; )
      for (int w = width; w--; )
        Grid::changeState(x + w, y + h, this, GR_FALLING);

  } else {
    alarm = Game::time_step + GC_HANG_DELAY;
    for (int h = height; h--; )
      for (int w = width; w--; )
        Grid::changeState(x + w, y + h, this, GR_HANGING | GR_FALLING);
  }

  // tell our upward neighbors to start a combo fall
  if (y + height < GC_PLAY_HEIGHT) {
    for (int w = width; w--; ) {
      if (Grid::stateAt(x + w, y + height) & GR_BLOCK)
        Grid::blockAt(x + w, y + height).startFalling(combo, no_hang);
      else if (Grid::stateAt(x + w, y + height) & GR_GARBAGE)
        Grid::garbageAt(x + w, y + height).startFalling(combo, no_hang);
    }
  }
}

void Garbage::startShattering ( int &s_x, int s_y, int &pop_delay,
 int awake_delay, ComboTabulator *combo )
/*
 * This is called for each row we occupy, with s_x equal to our left most
 * position and s_y indicating which row the call is for.  We must convert
 * ourselves to blocks or new garbage along that row.  Additionally, we must
 * advance s_x to our right side as well as pop_delay and delete ourselves if
 * this is our top row.
 *
 * Note that we may want to change the block/garbage conversion behavior.
 */
{
#ifndef NDEBUG
  // otherwise assert will bite us
  for (int w = 0; w < width; w++)
    Grid::remove(s_x + w, s_y, this);
#endif

  // if it's an even row, perhaps shatter into new garbage
  if ((width == GC_PLAY_WIDTH && ((s_y - y) & (1 << 0))
   && Random::chanceIn(GC_GARBAGE_TO_GARBAGE_SHATTER))
   || flavor == GF_SHATTER_TO_NORMAL_GARBAGE) {
    GarbageManager::newAwakingGarbage(s_x, s_y, 1, pop_delay, awake_delay,
     combo, flavor);
    s_x += GC_PLAY_WIDTH;
    pop_delay += GC_PLAY_WIDTH * GC_INTERNAL_POP_DELAY;

  // otherwise, shatter into blocks
  } else
    for (int w = 0; w < width; w++) {
      BlockManager::newAwakingBlock(s_x, s_y, pop_delay, awake_delay, combo,
       flavor);
      s_x++;
      pop_delay += GC_INTERNAL_POP_DELAY;
    }

  // If it's our top row, enter shatter state; we are no longer on the grid
  // but we stay around a bit to animate our shattering; since we're not in the
  // grid, our time step will never be called; that's OK!  We'll be deleted by
  // the display code, sloppy but fastest.
  if (s_y + 1 == y + height) {

    // change our state
    state = GS_SHATTERING;

    // set the deletion alarm
    alarm = Game::time_step + DC_SHATTER_TIME;

    // notify extreme effects of our demise
    X::notifyShatter(*this);
  }
}
