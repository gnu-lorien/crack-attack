/*
 * LevelLights.h
 * Daniel Nelson - 8/24/0
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
 */

#ifndef LEVELLIGHTS_H
#define LEVELLIGHTS_H

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "Communicator.h"

#define LL_NUMBER_LEVEL_LIGHTS       (GC_SAFE_HEIGHT - 1)

// two light sets
#define LL_LOCAL_LIGHTS              (0)
#define LL_OPPONENT_LIGHTS           (1)

// light states
#define LS_RED                       (1 << 0)
#define LS_BLUE                      (1 << 1)
#define LS_FADE_TO_RED               (1 << 2)
#define LS_FADE_TO_BLUE              (1 << 3)
#define LS_IMPACT_FLASH              (1 << 4)

// communication buffer states
#define LC_BLUE                      (1 << 0)
#define LC_IMPACT                    (1 << 1)
#define LC_DEATH_FLASH_MASK          (1 << (2 * LL_NUMBER_LEVEL_LIGHTS))

class LevelLight {
public:
  int state;
  int fade_alarm;
  int flash_alarm;
};

/* static */ class LevelLights {
public:
  static void initialize (   );
  static void gameStart (   );
  static void timeStep (   );

  static inline void levelRaise ( int top_effective_row )
  {
    top_effective_row--;
    if (top_effective_row >= LL_NUMBER_LEVEL_LIGHTS)
      top_effective_row = LL_NUMBER_LEVEL_LIGHTS - 1;

    while (top_effective_row >= 0) {
      LevelLight &light = lights[LL_LOCAL_LIGHTS][top_effective_row];
      if (light.state & (LS_RED | LS_FADE_TO_RED)) break;
      setRed(light);
      top_effective_row--;
    }
  }

  static inline void levelLower ( int top_effective_row )
  {
    while (top_effective_row < LL_NUMBER_LEVEL_LIGHTS) {
      LevelLight &light = lights[LL_LOCAL_LIGHTS][top_effective_row];
      if (light.state & (LS_BLUE | LS_FADE_TO_BLUE)) break;
      setBlue(light);
      top_effective_row++;
    }
  }

  static inline void notifyImpact ( int y, int height )
  {
    if (y - 1 + height > LL_NUMBER_LEVEL_LIGHTS) {
      height = LL_NUMBER_LEVEL_LIGHTS - y + 1;
      if (height < 1) return;
    }

    while (height--) {
      LevelLight &light = lights[LL_LOCAL_LIGHTS][y - 1 + height];
      Communicator::setLevelLightSendBit(LC_IMPACT << (2 * (y - 1 + height)));
      setFlashing(light);
    }
  }

  static inline void notifySafeHeightViolation (   )
  {
    if (death_flash_alarm[LL_LOCAL_LIGHTS] == -1)
      death_flash_alarm[LL_LOCAL_LIGHTS] = DC_LEVEL_LIGHT_DEATH_FLASH_TIME;
  }
  
  static inline void readySendBuffer (   )
  {
    for (int n = LL_NUMBER_LEVEL_LIGHTS; n--; )
      if (lights[LL_LOCAL_LIGHTS][n].state & (LS_BLUE | LS_FADE_TO_BLUE))
        Communicator::setLevelLightSendBit(LC_BLUE << (2 * n));

    if (death_flash_alarm[LL_LOCAL_LIGHTS] != -1)
      Communicator::setLevelLightSendBit(LC_DEATH_FLASH_MASK);
  }

  static inline void handleRecvBuffer (   )
  {
    if (Communicator::checkLevelLightRecvBit(LC_DEATH_FLASH_MASK)
     && death_flash_alarm[LL_OPPONENT_LIGHTS] == -1)
      death_flash_alarm[LL_OPPONENT_LIGHTS] = DC_LEVEL_LIGHT_DEATH_FLASH_TIME;

    for (int n = LL_NUMBER_LEVEL_LIGHTS; n--; ) {
      if (lights[LL_OPPONENT_LIGHTS][n].state & (LS_RED | LS_FADE_TO_RED)) {
        if (Communicator::checkLevelLightRecvBit(LC_BLUE << (2 * n)))
          setBlue(lights[LL_OPPONENT_LIGHTS][n]);
      } else {
        if (!Communicator::checkLevelLightRecvBit(LC_BLUE << (2 * n)))
          setRed(lights[LL_OPPONENT_LIGHTS][n]);
      }

      if (Communicator::checkLevelLightRecvBit(LC_IMPACT << (2 * n)))
        setFlashing(lights[LL_OPPONENT_LIGHTS][n]);
    }
  }

  static void handleAI ();

  // level light n corresponds to row n + 1 in the grid
  static LevelLight lights[2][LL_NUMBER_LEVEL_LIGHTS];

  static int death_flash_alarm[2];

private:
  static inline void setBlue ( LevelLight &light )
  {
    if (light.state & LS_FADE_TO_RED)
      light.fade_alarm = DC_LEVEL_LIGHT_FADE_TIME - light.fade_alarm;
    else
      light.fade_alarm = DC_LEVEL_LIGHT_FADE_TIME;
    light.state &= ~(LS_RED | LS_FADE_TO_RED);
    light.state |= LS_FADE_TO_BLUE;
  }

  static inline void setRed ( LevelLight &light )
  {
    if (light.state & LS_FADE_TO_BLUE)
      light.fade_alarm = DC_LEVEL_LIGHT_FADE_TIME - light.fade_alarm;
    else
      light.fade_alarm = DC_LEVEL_LIGHT_FADE_TIME;
    light.state &= ~(LS_BLUE | LS_FADE_TO_BLUE);
    light.state |= LS_FADE_TO_RED;
  }

  static inline void setFlashing ( LevelLight &light )
  {
    if (light.state & LS_IMPACT_FLASH) {
      // if past flash inflection point, sync flash start
      if (light.flash_alarm < (int) (DC_LEVEL_LIGHT_FLASH_INFLECTION
       * DC_LEVEL_LIGHT_IMPACT_FLASH_TIME))
        light.flash_alarm = (int) (DC_LEVEL_LIGHT_FLASH_INFLECTION
         * DC_LEVEL_LIGHT_IMPACT_FLASH_TIME)
         + (int) (((1.0f - DC_LEVEL_LIGHT_FLASH_INFLECTION)
         * DC_LEVEL_LIGHT_IMPACT_FLASH_TIME) * (1.0f
         - light.flash_alarm * (1.0f / (DC_LEVEL_LIGHT_FLASH_INFLECTION
         * DC_LEVEL_LIGHT_IMPACT_FLASH_TIME))));

    } else {
      light.state |= LS_IMPACT_FLASH;
      light.flash_alarm = DC_LEVEL_LIGHT_IMPACT_FLASH_TIME;
    }
  }
};

#endif
