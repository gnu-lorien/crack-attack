/*
 * CelebrationManager.cxx
 * Daniel Nelson - 10/30/0
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
 * Handles the celebration after a game.
 */

#include "Game.h"
#include "MetaState.h"
#include "Displayer.h"
#include "CelebrationManager.h"
#include "SparkleManager.h"
#include "MessageManager.h"
#include "WinRecord.h"
#include "Random.h"
#include "Score.h"

using namespace std;

bool CelebrationManager::draw_game;
GLfloat CelebrationManager::light_level;
int CelebrationManager::spark_rate[DC_CSPARK_SOURCE_NUMBER];
int CelebrationManager::spark_color[DC_CSPARK_SOURCE_NUMBER];

float CelebrationManager::win_alpha;
float CelebrationManager::win_scale;
int CelebrationManager::win_flash1;
int CelebrationManager::win_flash2;

float CelebrationManager::loss_height;
float CelebrationManager::loss_velocity;
bool CelebrationManager::loss_rescale;
int CelebrationManager::loss_bounce_count;

void CelebrationManager::gameFinish (   )
{
  draw_game = true;
  light_level = 1.0f;

  MessageManager::mode = MM_CELEBRATION;

  if (WinRecord::won) {
    win_alpha = 0.0f;
    win_scale = DC_STARTING_WIN_SCALE;
    MessageManager::readyMessage(MS_WINNER);

  } else {
    loss_height = DC_STARTING_LOSS_HEIGHT;
    loss_velocity = 0.0f;
    loss_bounce_count = DC_STARTING_BOUNCE_COUNT;

    if (WinRecord::isMatchFinished()) {
      MessageManager::readyMessage(MS_GAME_OVER);
      loss_rescale = true;

    } else {
      MessageManager::readyMessage(MS_LOSER);
      loss_rescale = false;
    }
  }
}

void CelebrationManager::celebrationFinish (   )
{
  draw_game = false;

  MessageManager::mode = MM_NORMAL;
  MessageManager::freeMessage();
}

void CelebrationManager::timeStep (   )
{
  if (WinRecord::current_game == -1) return;

  // fade out the game
  if (Game::time_step < DC_CELEBRATION_FADE_TIME)
    light_level = (DC_CELEBRATION_FADE_TIME
     - Game::time_step) * (1.0f / (float) DC_CELEBRATION_FADE_TIME);

  else if (Game::time_step == DC_CELEBRATION_FADE_TIME)
    light_level = 0.0f;

  // signal that the user is now allowed to end the celebration
  else if (Game::time_step == DC_CELEBRATION_TIME)
    MetaState::celebrationComplete();

  // draw the game until the fade out is complete and no light motes exist
  if (draw_game && !(Game::time_step < DC_CELEBRATION_FADE_TIME
   || SparkleManager::mote_count > 0))
    draw_game = false;

  // handle the message behavior

  // if we won
  if (WinRecord::won) {

    // fade in
    if (Game::time_step < DC_WIN_FADE_TIME) {
      win_alpha += 1.0f / (float) DC_WIN_FADE_TIME;
      win_scale -= (DC_STARTING_WIN_SCALE - 1.0f) / (float) DC_WIN_FADE_TIME;

    // reach static state
    } else if (Game::time_step == DC_WIN_FADE_TIME) {
      win_alpha = 1.0f;
      win_scale = 1.0f;
      win_flash1 = DC_WIN_FLASH_1_TIME;
      win_flash2 = DC_WIN_FLASH_2_TIME;
      
      spark_rate[DC_CSPARK_SOURCE_NUMBER - 1] = DC_CSPARK_FULL_RATE;
      spark_color[DC_CSPARK_SOURCE_NUMBER - 1]
       = Random::number(DC_CSPARK_COLOR_NUMBER);
      for (int n = DC_CSPARK_SOURCE_NUMBER - 1; n--; ) {
        spark_rate[n] = DC_CSPARK_STARTING_RATE;
        spark_color[n] = Random::number(DC_CSPARK_COLOR_NUMBER);
      }

    } else {

      // flash...

      if (win_flash1)
        win_flash1--;
      if (Random::chanceIn(DC_WIN_FLASH_1_CHANCE_IN))
        if (win_flash1) {
          if (win_flash1 < DC_WIN_FLASH_1_TIME / 2)
            win_flash1 = (DC_WIN_FLASH_1_TIME / 2) - win_flash1;
        } else
          win_flash1 = DC_WIN_FLASH_1_TIME;

      if (win_flash2)
        win_flash2--;
      if (Random::chanceIn(DC_WIN_FLASH_2_CHANCE_IN))
        if (win_flash2) {
          if (win_flash2 < DC_WIN_FLASH_2_TIME / 2)
            win_flash2 = (DC_WIN_FLASH_2_TIME / 2) - win_flash2;
        } else 
          win_flash2 = DC_WIN_FLASH_2_TIME;

      // and, if you've been really good, spark
      if (WinRecord::isMatchFinished()
       && (!(MetaState::mode & CM_SOLO) || Score::topRank())) {

        // this algorithm is supposed to cause the sparks to be created in
        // spurts, sort of like a sputtering firework

        for (int n = DC_CSPARK_SOURCE_NUMBER; n--; ) {
          if (spark_rate[n] > DC_CSPARK_LOW_RATE)
            spark_rate[n] -= DC_CSPARK_QUICK_RATE_DROP;
          else if (spark_rate[n] > 0)
            spark_rate[n]--;

          if (Random::chanceIn(DC_CSPARK_BOOST_CHANCE_IN)) {
            spark_rate[n] += DC_CSPARK_RATE_BOOST;
            if (Random::chanceIn(DC_CSPARK_COLOR_CHANGE_CHANCE_IN))
              spark_color[n] = Random::number(DC_CSPARK_COLOR_NUMBER);
          }

          if (Random::number(DC_CSPARK_FULL_RATE) < spark_rate[n])
            SparkleManager::createCelebrationSpark(n, spark_color[n]);
        }
      }
    }

  // if we lost
  } else {

    // drop the sign

    if (loss_bounce_count != DC_FINAL_BOUNCE_COUNT - 1) {

      loss_height += loss_velocity;
      loss_velocity += -DC_LOSS_GRAVITY - DC_LOSS_DRAG * loss_velocity;

      // if we've hit bottom
      if (loss_height < 0.0f) {

        // if it's the last bounce
        if (loss_bounce_count == DC_FINAL_BOUNCE_COUNT) {
          loss_bounce_count--;
          loss_velocity = 0.0f;
          loss_height = 0.0f;

        // otherwise, bounce
        } else {

          // near the final bounce, reduce the elasticity
          if (loss_velocity > -DC_LOSS_MIN_VELOCITY) {
            loss_bounce_count--;
            loss_velocity = -DC_LOSS_END_BOUNCE_ELASTICITY
             * loss_bounce_count * loss_velocity;
            loss_height = -loss_height;

          } else {
            loss_height = -loss_height;
            loss_velocity = -DC_LOSS_BOUNCE_ELASTICITY * loss_velocity;
          }
        }
      }
    }
  }
}
