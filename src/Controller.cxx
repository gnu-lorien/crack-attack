/*
 * Controller.cxx
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
 * Keeps track of the button pressing.
 */

#include <GL/glut.h>
#include <cassert>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

#include "Game.h"
#include "Controller.h"
#include "MetaState.h"
#include "Grid.h"

#ifdef DEVELOPMENT
#include "Displayer.h"
#endif

using namespace std;

int Controller::state;

void Controller::gameStart (   )
{
  state = 0;
}

void Controller::keyboardPlay ( unsigned char key, int x, int y )
{
  switch (key) {
  case GC_LEFT_KEY: case (GC_LEFT_KEY - 32):
    state |= CC_LEFT;
    break;
  case GC_RIGHT_KEY: case (GC_RIGHT_KEY - 32):
    state |= CC_RIGHT;
    break;
  case GC_UP_KEY: case (GC_UP_KEY - 32):
    state |= CC_UP;
    break;
  case GC_DOWN_KEY: case (GC_DOWN_KEY - 32):
    state |= CC_DOWN;
    break;
  case GC_SWAP_KEY: case (GC_SWAP_KEY - 32): case ' ':
    state |= CC_SWAP;
    break;
  case GC_ADVANCE_KEY: case (GC_ADVANCE_KEY - 32): case '\r':
    state |= CC_ADVANCE;
    break;
  case GC_PAUSE_KEY: case (GC_PAUSE_KEY - 32):
    state |= CC_PAUSE;
    break;
#ifndef NDEBUG
  case 'z':
    Grid::dump();
    break;
#endif
#ifdef DEVELOPMENT
  case '.':
    Displayer::screenShot();
    break;
#endif
  case 27:
    Game::concession();
    break;
  }
}

void Controller::keyboardUpPlay ( unsigned char key, int x, int y )
{
  switch (key) {
  case GC_LEFT_KEY: case (GC_LEFT_KEY - 32):
    state &= ~CC_LEFT;
    break;
  case GC_RIGHT_KEY: case (GC_RIGHT_KEY - 32):
    state &= ~CC_RIGHT;
    break;
  case GC_UP_KEY: case (GC_UP_KEY - 32):
    state &= ~CC_UP;
    break;
  case GC_DOWN_KEY: case (GC_DOWN_KEY - 32):
    state &= ~CC_DOWN;
    break;
  case GC_SWAP_KEY: case (GC_SWAP_KEY - 32): case ' ':
    state &= ~CC_SWAP;
    break;
  case GC_ADVANCE_KEY: case (GC_ADVANCE_KEY - 32): case '\r':
    state &= ~CC_ADVANCE;
    break;
  case GC_PAUSE_KEY: case (GC_PAUSE_KEY - 32):
    state &= ~CC_PAUSE;
    break;
  }
}

void Controller::specialPlay ( int key, int x, int y )
{
  switch (key) {
  case GLUT_KEY_LEFT:
    state |= CC_LEFT;
    break;
  case GLUT_KEY_RIGHT:
    state |= CC_RIGHT;
    break;
  case GLUT_KEY_UP:
    state |= CC_UP;
    break;
  case GLUT_KEY_DOWN:
    state |= CC_DOWN;
    break;
  }
}

void Controller::specialUpPlay ( int key, int x, int y )
{
  switch (key) {
  case GLUT_KEY_LEFT:
    state &= ~CC_LEFT;
    break;
  case GLUT_KEY_RIGHT:
    state &= ~CC_RIGHT;
    break;
  case GLUT_KEY_UP:
    state &= ~CC_UP;
    break;
  case GLUT_KEY_DOWN:
    state &= ~CC_DOWN;
    break;
  }
}

void Controller::keyboardMeta ( unsigned char key, int x, int y )
{
#ifdef DEVELOPMENT
  if (key == '.') Displayer::screenShot();
#endif
  MetaState::localKeyPressed(key == 27);
}

void Controller::keyboardUpMeta ( unsigned char key, int x, int y )
{
}

void Controller::specialMeta ( int key, int x, int y )
{
  if (MetaState::mode * CM_SOLO)
    switch (key) {
    case GLUT_KEY_UP:
      state |= CC_UP;
      break;
    case GLUT_KEY_DOWN:
      state |= CC_DOWN;
      break;
    default:
      MetaState::localKeyPressed(false);
      break;
    }
  else
    MetaState::localKeyPressed(false);
}

void Controller::specialUpMeta ( int key, int x, int y )
{
  assert(MetaState::mode & CM_SOLO);

  switch (key) {
  case GLUT_KEY_UP:
    state &= ~CC_UP;
    break;
  case GLUT_KEY_DOWN:
    state &= ~CC_DOWN;
    break;
  }
}

void Controller::entry ( int mouse_state )
{
  if (mouse_state == GLUT_LEFT)
    state = 0;
}
