/*
 * MetaState.cxx
 * Daniel Nelson - 10/22/0
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
 * Handles the program's state and transfers between them.
 */

#include <GL/glut.h>
#include <cstring>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "MetaState.h"
#include "Displayer.h"
#include "CelebrationManager.h"
#include "Controller.h"
#include "Communicator.h"
#include "WinRecord.h"
#include "MessageManager.h"

int MetaState::state;
int MetaState::mode = 0;
int MetaState::final_time_step;
char MetaState::player_name[GC_PLAYER_NAME_LENGTH];

void MetaState::programStart ( int _mode,
 char _player_name[GC_PLAYER_NAME_LENGTH] )
{
  state = MS_BOTH_KEY_WAIT;
  mode |= _mode;
  strncpy(player_name, _player_name, GC_PLAYER_NAME_LENGTH);
  
  Game::initialize();
  Displayer::initialize();

  MessageManager::mode = MM_NORMAL;
  MessageManager::readyMessage(MS_ANYKEY);

  glutKeyboardFunc(Controller::keyboardMeta);
  glutSpecialFunc(Controller::specialMeta);
  glutKeyboardUpFunc(null);
  glutSpecialUpFunc(null);
  glutEntryFunc(Controller::entry);
  glutDisplayFunc(Displayer::displayMeta);
  glutReshapeFunc(Displayer::reshape);
  glutIdleFunc(Game::idleMeta);

  if (!(mode & CM_SOLO))
    Communicator::barrier();

  atexit(programEnd);

  Game::go();
}

void MetaState::programEnd (   )
{
  Game::cleanUp();
  Displayer::cleanUp();
  Communicator::cleanUp();
}

void MetaState::gameStart (   )
{
  state = MS_GAME_PLAY;

  MessageManager::freeMessage();

  if (!(mode & CM_SOLO))
    Communicator::gameStart();
  Game::gameStart();
  Displayer::gameStart();
  
  glutKeyboardFunc(Controller::keyboardPlay);
  glutSpecialFunc(Controller::specialPlay);
  glutKeyboardUpFunc(Controller::keyboardUpPlay);
  glutSpecialUpFunc(Controller::specialUpPlay);
  glutDisplayFunc(Displayer::displayPlay);
  glutIdleFunc(Game::idlePlay);

  if (!(mode & CM_SOLO))
    Communicator::barrier();

  Game::go();
}

void MetaState::gameWon (   )
{
  WinRecord::gameWon();
  gameFinish();
}

void MetaState::gameLoss (   )
{
  WinRecord::gameLoss();
  gameFinish();
}

void MetaState::gameFinish (   )
{
  final_time_step = Game::time_step;

  if (state & MS_CONCESSION)
    WinRecord::matchConceded();

  if (!(mode & CM_SOLO))
    Communicator::gameFinish();
  Game::gameFinish();
  Displayer::gameFinish();

  if (WinRecord::isMatchFinished()) {
    if (!(mode & CM_SOLO))
      Communicator::cleanUp();
    state = MS_GAME_OVER_KEY_WAIT;

  } else 
    state = MS_CELEBRATION_WAIT | MS_BOTH_KEY_WAIT;

  glutKeyboardFunc(Controller::keyboardMeta);
  glutSpecialFunc(Controller::specialMeta);
  glutDisplayFunc(Displayer::displayMeta);
  glutIdleFunc(Game::idleMeta);

  // GLUT 3.7 is seg faulting if glutKeyboardUpFunc is unset and
  // glutSpecialUpFunc is set.  This is the only use of
  // Controller::keyboardUpMeta() .
  // glutKeyboardUpFunc(null); // removed
  if (mode & CM_SOLO) {
    glutSpecialUpFunc(Controller::specialUpMeta);
    glutKeyboardUpFunc(Controller::keyboardUpMeta); // workaround
  } else {
    glutSpecialUpFunc(null);
    glutKeyboardUpFunc(null); // workaround
  }

  if (!(mode & CM_SOLO) && !WinRecord::isMatchFinished())
    Communicator::barrier();

  Game::go();
}

void MetaState::celebrationComplete (   )
{
  // allow the player to end the celebration
  state &= ~MS_CELEBRATION_WAIT;
}

void MetaState::localKeyPressed ( bool esc )
{
  if (state & MS_CELEBRATION_WAIT) return;

  switch (state) {
  case MS_BOTH_KEY_WAIT:
    if (WinRecord::current_game == -1)
      MessageManager::freeMessage();
    else
      CelebrationManager::celebrationFinish();
    MessageManager::readyMessage(MS_WAITING);
    if (!(mode & CM_SOLO))
      state = MS_REMOTE_KEY_WAIT;
    else {
      state = MS_READY_GAME_START;
      remoteReady();
    }
    return;

  case MS_LOCAL_KEY_WAIT:
    if (WinRecord::current_game == -1)
      MessageManager::freeMessage();
    else
      CelebrationManager::celebrationFinish();
    MessageManager::readyMessage(MS_WAITING);
    state = MS_READY_GAME_START;
    return;

  case MS_GAME_OVER_KEY_WAIT:
    if (!esc) return;
    exit(0);
    return;
  }
}

void MetaState::remoteKeyPressed (   )
{
  switch (state) {
  case MS_BOTH_KEY_WAIT:
    state = MS_LOCAL_KEY_WAIT;
    return;

  case MS_REMOTE_KEY_WAIT:
    state = MS_READY_GAME_START;
    return;
  }
}

void MetaState::remoteReady (   )
{
  switch (state) {
  case MS_READY_GAME_START:
    gameStart();
    return;

  case MS_REMOTE_KEY_WAIT:
    state = MS_READY_GAME_START;
    return;

  case MS_BOTH_KEY_WAIT:
    state = MS_LOCAL_KEY_WAIT;
    return;
  }
}
