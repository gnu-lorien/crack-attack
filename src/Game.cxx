/*
 * Game.cxx
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
 * The core loop plus extras.
 * Remove dying_count_2.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "BlockManager.h"
#include "CelebrationManager.h"
#include "Clock.h"
#include "Controller.h"
#include "Communicator.h"
#include "ComboManager.h"
#include "CountDownManager.h"
#include "Creep.h"
#include "GarbageGenerator.h"
#include "GarbageManager.h"
#include "Grid.h"
#include "LevelLights.h"
#include "LoseBar.h"
#include "MessageManager.h"
#include "MetaState.h"
#include "Random.h"
#include "Score.h"
#include "ScoreRecordManager.h"
#include "SignManager.h"
#include "Sine.h"
#include "SparkleManager.h"
#include "Spring.h"
#include "Swapper.h"
#include "WinRecord.h"
#include "X.h"

int Game::time_step;
int Game::state;
int Game::awaking_count;
int Game::dying_count;
int Game::dying_count_2;
int Game::previous_time;
int Game::remaining_time;
bool Game::button_down_pause;
bool Game::step_play;
int Game::sync_wait;

void Game::initialize (   )
{
  Random::initialize();
  WinRecord::initialize();
  LevelLights::initialize();
  Score::initialize();
  Sine::initialize();
  LoseBar::initialize();

  gameFinish();
}

void Game::gameStart (   )
{
  state = GS_NORMAL;

  awaking_count = 0;
  dying_count = 0;

  WinRecord::gameStart();

  BlockManager::gameStart();
  GarbageManager::gameStart();
  ComboManager::gameStart();
  GarbageGenerator::gameStart();
  X::gameStart();
  Grid::gameStart();
  Controller::gameStart();
  Creep::gameStart();
  Swapper::gameStart();
  Spring::gameStart();
  LevelLights::gameStart();
  CountDownManager::gameStart();
  Clock::gameStart();
  LoseBar::gameStart();

  button_down_pause = false;

  step_play = true;

  remaining_time = 0;
  time_step = 1;
}

void Game::gameFinish (   )
{
  remaining_time = 0;
  time_step = 1;
}

void Game::cleanUp (   )
{
  CountDownManager::cleanUp();
  Score::cleanUp();
}

void Game::loss (   )
/*
 * Called when creep detects a game loss condition; opponent may have already
 * lost, so me must wait for confirmation.
 */
{
  if (!(MetaState::mode & CM_SOLO)) {
    if (!(state & (GS_MAY_HAVE_LOST | GS_WON))) {
      state = GS_MAY_HAVE_LOST;
      Communicator::setLossTimeStep();
    }
  } else
    state |= GS_END_PLAY;
}

void Game::lossConfirmation (   )
/*
 * Called by Communicator when opponent confirms game loss.  Now we must only
 * wait for the level lights to end play.
 */
{
  state = GS_LOST | GS_END_PLAY;
}

void Game::won (   )
/*
 * Called by Communicator when opponent signals a game loss that predates
 * ours or signals a reverse of our game loss.  Now we must wait to confirm
 * our opponent's loss and for the level lights to end play.
 */
{
  if (!(state & GS_WON))
    state = GS_WON | GS_MUST_CONFIRM_LOSS;
}

void Game::concession (   )
/*
 * Called by the controller upon player concession.
 */
{
  if (CountDownManager::start_pause_alarm != 0) return;

  loss();
  MetaState::state |= MS_CONCESSION;
}

void Game::buttonPause (   )
// run time step during pause (for Communicator); then reset the clock to the
// value at the pause time
{
  // unpause
  if ((state & (GS_PAUSED | GS_SYNC_WAIT)) == GS_PAUSED) {
    state = GS_NORMAL;

    MessageManager::freeMessage();
    if (CountDownManager::state != -1)
      MessageManager::readyMessage(CountDownManager::state);

    if (!(MetaState::mode & CM_SOLO)) {
      Communicator::unpauseSyncCheck();
      Communicator::signalUnpaused();
    }

  // pause
  } else {
    // to simplify things, you can't pause if you're about to lose
    if (!(state & GS_NORMAL)) return;

    state = GS_PAUSED;

    if (CountDownManager::state != -1)
      MessageManager::freeMessage();
    MessageManager::readyMessage(MS_PAUSED);

    if (!(MetaState::mode & CM_SOLO))
      Communicator::signalPaused();
  }
}

void Game::netSignalPause (   )
{
  if (!(state & GS_NORMAL)) return;

  state = GS_PAUSED;

  if (CountDownManager::state != -1)
    MessageManager::freeMessage();
  MessageManager::readyMessage(MS_PAUSED);
}

void Game::netSignalUnpause (   )
{
  if ((state & (GS_PAUSED | GS_SYNC_WAIT)) != GS_PAUSED) return;

  state = GS_NORMAL;

  MessageManager::freeMessage();
  if (CountDownManager::state != -1)
    MessageManager::readyMessage(CountDownManager::state);
}

void Game::syncPause ( int delay )
{
  assert(delay > 0);

  state = GS_PAUSED | GS_SYNC_WAIT;
  sync_wait = delay;

  if (CountDownManager::state != -1)
    MessageManager::freeMessage();
  MessageManager::readyMessage(MS_WAITING);
}

void Game::syncUnpause (   )
{
  state = GS_NORMAL;

  MessageManager::freeMessage();
  if (CountDownManager::state != -1)
    MessageManager::readyMessage(CountDownManager::state);
}

void Game::idleMeta (   )
{
  int modified_and_complete = false;

  do {

    int time = glutGet((GLenum) GLUT_ELAPSED_TIME);
    remaining_time += time - previous_time;
    previous_time = time;

    // no time step yet
    if (remaining_time < GC_TIME_STEP_PERIOD) break;

    remaining_time -= GC_TIME_STEP_PERIOD;
    time_step++;

    if (remaining_time < GC_TIME_STEP_PERIOD)
      modified_and_complete = true;
    else
      DOT(3);

    // communicate state and other stuff
    if (!(MetaState::state & MS_GAME_OVER_KEY_WAIT) &&
     !(MetaState::mode & CM_SOLO))
      Communicator::timeStepMeta();

    // advance celebration
    CelebrationManager::timeStep();

    // advance the score record
    ScoreRecordManager::timeStep();

    // update message pulse
    MessageManager::timeStep();

    // update the pretty stuff too
    SparkleManager::timeStep();
    SignManager::timeStep();
    WinRecord::timeStep();
    LevelLights::timeStep();

    // advance the clock through its final tick fade
    Clock::timeStepMeta();

    // advance the score through its final increment fade
    Score::timeStepMeta();

  } while (false);

  if (modified_and_complete) {
    glFinish();
    glutPostRedisplay();
  }
}

void Game::idlePlay (   )
{
  int modified_and_complete = false;

  do {

    int time = glutGet((GLenum) GLUT_ELAPSED_TIME);
    remaining_time += time - previous_time;
    previous_time = time;

    // no time step yet
    if (remaining_time < GC_TIME_STEP_PERIOD) break;

    remaining_time -= GC_TIME_STEP_PERIOD;

    if (remaining_time < GC_TIME_STEP_PERIOD)
      modified_and_complete = true;
    else
      DOT(2);

    if (!(state & GS_PAUSED)) {
      if (step_play) {

        time_step++;

        // check for and continue swaps and moves
        Swapper::timeStep();

        // update the level lights
        LevelLights::timeStep();

        // update message pulse
        MessageManager::timeStep();

        // move the win record stars
        WinRecord::timeStep();

        // update the starting count down
        CountDownManager::timeStep();
        if (CountDownManager::start_pause_alarm != 0) {
          // some objects require a time step during start pause

          if (!(MetaState::mode & CM_SOLO))
            Communicator::timeStepPlay();

          // reset the clock
          Clock::timeStepMeta();

          continue;
        }

        // loop over the grid, bottom to top; garbage will advance x and y
        for (int y = 1; y < GC_PLAY_HEIGHT; y++)
          for (int x = 0; x < GC_PLAY_WIDTH; x++) {
            if (Grid::residentTypeAt(x, y) & GR_EMPTY) continue;

            if (Grid::residentTypeAt(x, y) & GR_BLOCK)
              Grid::blockAt(x, y).timeStep();
            else
              Grid::garbageAt(x, y).timeStep(x, y);
          }

        // perhaps creep upward
        Creep::timeStep();

        // process elimination check requests and update top_occupied_row
        Grid::timeStep();

        // handle new and finished combos
        ComboManager::timeStep();
      }

      // communicate garbage and other stuff
      if (!(MetaState::mode & CM_SOLO))
        Communicator::timeStepPlay();

      if (step_play) {

        // drop queued garbage
        GarbageGenerator::timeStep();

        // move the sparkles and signs
        SparkleManager::timeStep();
        SignManager::timeStep();

        // update the impact spring
        Spring::timeStep();

        // update the clock
        Clock::timeStepPlay();

        // update the score
        Score::timeStepPlay();

        // update the losebar
        LoseBar::timeStep();

        // advance extreme effects
        if (MetaState::mode & CM_X)
          X::timeStep();
      }

    } else {

      // if this is a sync pause, count down the waiting period
      if (state & GS_SYNC_WAIT && !--sync_wait)
        syncUnpause();

      // communicate when paused
      if (!(MetaState::mode & CM_SOLO))
        Communicator::timeStepPlay();

      // update message pulse
      MessageManager::timeStep();
    }

    if (state & (GS_MAY_HAVE_LOST | GS_MUST_CONFIRM_LOSS))
      step_play = false;

    if (state & GS_END_PLAY) {
      if (state & GS_PAUSED)
        MessageManager::freeMessage();

      if (MetaState::mode & CM_SOLO)
        state = Score::gameFinish();

      if (state & GS_LOST)
        MetaState::gameLoss();
      else
        MetaState::gameWon();

      break;
    }

  } while (false);

  if (!button_down_pause) {
    if (Controller::pauseCommand()) {
      button_down_pause = true;
      buttonPause();
    }
  } else
    if (!Controller::pauseCommand())
      button_down_pause = false;

  if (modified_and_complete) {
    glFinish();
    glutPostRedisplay();
  }
}
