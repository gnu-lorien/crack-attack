/*
 * LevelLights.cxx
 * Daniel Nelson - 10/13/0
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
 * Handles the level lights' states.
 */

using namespace std;

#include "Game.h"
#include "LevelLights.h"
#include "Grid.h"
#include "Communicator.h"
#include "MetaState.h"

int LevelLights::death_flash_alarm[2];
LevelLight LevelLights::lights[2][LL_NUMBER_LEVEL_LIGHTS];

void LevelLights::initialize (   )
{
  for (int n = 0; n < LL_NUMBER_LEVEL_LIGHTS; n++) {
    lights[LL_LOCAL_LIGHTS][n].state = LS_BLUE;
    lights[LL_OPPONENT_LIGHTS][n].state = LS_BLUE;
  }

  death_flash_alarm[LL_LOCAL_LIGHTS] = -1;
  death_flash_alarm[LL_OPPONENT_LIGHTS] = -1;
}

void LevelLights::gameStart (   )
{
  int n;
  for (n = 0; n < Grid::top_effective_row; n++) {
    setRed(lights[LL_LOCAL_LIGHTS][n]);
    setRed(lights[LL_OPPONENT_LIGHTS][n]);
  }
  for ( ; n < LL_NUMBER_LEVEL_LIGHTS; n++) {
    setBlue(lights[LL_LOCAL_LIGHTS][n]);
    setBlue(lights[LL_OPPONENT_LIGHTS][n]);
  }
}

void LevelLights::timeStep (   )
{
  // loop through the level light sets
  for (int set = 2; set--; ) {

    // loop through the set's level lights
    for (int n = LL_NUMBER_LEVEL_LIGHTS; n--; ) {
      LevelLight &light = lights[set][n];

      // see if we're finished fading
      if (light.state & (LS_FADE_TO_RED | LS_FADE_TO_BLUE))
        if (!light.fade_alarm--) {
          light.state |= ((light.state & LS_FADE_TO_RED) ? LS_RED : LS_BLUE);
          light.state &= ~(LS_FADE_TO_RED | LS_FADE_TO_BLUE);
        }

      // see if we're finished impact flashing
      if (light.state & LS_IMPACT_FLASH)
        if (!light.flash_alarm--)
          light.state &= ~LS_IMPACT_FLASH;
    }
  }

  // if we're death flashing and done with a flash and should continue
  if (death_flash_alarm[LL_LOCAL_LIGHTS] != -1
   && !death_flash_alarm[LL_LOCAL_LIGHTS]--
   && (MetaState::state & MS_GAME_PLAY)
   && Grid::checkSafeHeightViolation())
    death_flash_alarm[LL_LOCAL_LIGHTS] = DC_LEVEL_LIGHT_DEATH_FLASH_TIME;

  // if we're death flashing and done with a flash and should continue
  if (death_flash_alarm[LL_OPPONENT_LIGHTS] != -1
   && !death_flash_alarm[LL_OPPONENT_LIGHTS]--
   && (MetaState::state & MS_GAME_PLAY)
   && Communicator::checkLevelLightRecvBit(LC_DEATH_FLASH_MASK))
    death_flash_alarm[LL_OPPONENT_LIGHTS] = DC_LEVEL_LIGHT_DEATH_FLASH_TIME;
}
