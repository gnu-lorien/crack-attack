/*
 * DrawLevelLights.cxx
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
 * Draws the cute level lights.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "LevelLights.h"
#include "MetaState.h"

#define CS_UNDEFINED         (0)
#define CS_RED               (1)
#define CS_BLUE              (2)

const GLfloat ambient[]
 = { 0.0f, 0.0f, 0.0f, 1.0f };

const GLfloat diffuse[]
 = { 0.0f, 0.0f, 0.0f, 1.0f };

const GLfloat specular[]
 = { 0.8f, 0.8f, 0.8f, 1.0f };

void Displayer::drawLevelLights (   )
{
  int color_state;
  int next_color_state;
  GLfloat death_flash = 0.0f;
  GLfloat color[3];

  glColorMaterial(GL_FRONT, GL_EMISSION);

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, 2.0f);

  for (int set = 2; set--; ) {

    glPushMatrix();

      if (set == LL_LOCAL_LIGHTS)
        glTranslatef(DC_LEVEL_LIGHT_LOCAL_OFFSET_X,
         DC_PLAY_OFFSET_Y + (DC_GRID_ELEMENT_LENGTH / 2.0f), 0.0f);
      else {
        glTranslatef(DC_LEVEL_LIGHT_OPPONENT_OFFSET_X,
         DC_PLAY_OFFSET_Y + (DC_GRID_ELEMENT_LENGTH / 2.0f), 0.0f);
        glScalef(-1.0f, 1.0f, 1.0f);
      }

      static int hold_set = 0;
      if (MetaState::mode & CM_SOLO) {
        hold_set = set;
        set = LL_LOCAL_LIGHTS;
      }

      color_state = CS_UNDEFINED;

      if (LevelLights::death_flash_alarm[set] != -1) {
        death_flash = LevelLights::death_flash_alarm[set]
         * (2.0f / (GLfloat) DC_LEVEL_LIGHT_DEATH_FLASH_TIME);

        if (death_flash > 1.0f) death_flash = 2.0f - death_flash;
      }

      for (int n = 0; n < LL_NUMBER_LEVEL_LIGHTS; n++) {
        LevelLight &light = LevelLights::lights[set][n];
        color[0] = color[1] = color[2] = 0.0f;

        if (light.state & LS_RED) {
          color[0] = DC_LEVEL_LIGHT_RED;
          next_color_state = CS_RED;

        } else if (light.state & LS_BLUE) {
          color[2] = DC_LEVEL_LIGHT_BLUE;
          next_color_state = CS_BLUE;

        } else {
          GLfloat fade = light.fade_alarm
           * (1.0f / (GLfloat) DC_LEVEL_LIGHT_FADE_TIME);

          if (light.state & LS_FADE_TO_RED) {
            color[0] = DC_LEVEL_LIGHT_RED * Game::sqrt(1.0f - fade);
            color[2] = DC_LEVEL_LIGHT_BLUE * Game::sqrt(fade);

          } else {
            color[0] = DC_LEVEL_LIGHT_RED * Game::sqrt(fade);
            color[2] = DC_LEVEL_LIGHT_BLUE * Game::sqrt(1.0f - fade);
          }

          next_color_state = CS_UNDEFINED;
        }

        if (light.state & LS_IMPACT_FLASH) {
          GLfloat flash = light.flash_alarm
           * (1.0f / (GLfloat) DC_LEVEL_LIGHT_IMPACT_FLASH_TIME);

          if (flash > DC_LEVEL_LIGHT_FLASH_INFLECTION)
           flash = (1.0f - flash)
            * (1.0f / (1.0f - DC_LEVEL_LIGHT_FLASH_INFLECTION));
          else
           flash *= 1.0f / DC_LEVEL_LIGHT_FLASH_INFLECTION;
          flash *= flash;

          color[0] += (1.0f - color[0]) * flash;
          color[1] = flash;
          color[2] += (1.0f - color[2]) * flash;

          next_color_state = CS_UNDEFINED;
        }

        if (LevelLights::death_flash_alarm[set] != -1) {
          color[0] += (1.0f - color[0]) * death_flash;
          color[1] += (1.0f - color[1]) * death_flash;
          color[2] += (1.0f - color[2]) * death_flash;
        }

        if (next_color_state != color_state || color_state == CS_UNDEFINED) {
          glColor3fv(color);
          color_state = next_color_state;
        }

        glTranslatef(0.0f, DC_GRID_ELEMENT_LENGTH, 0.0f);

        glCallList(level_light_list);
      }

      if (MetaState::mode & CM_SOLO)
        set = hold_set;

    glPopMatrix();

  }
}
