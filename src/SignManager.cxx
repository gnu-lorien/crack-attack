/*
 * SignManager.cxx
 * Daniel Nelson - 9/14/0
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
 * Handles all the little reward signs.
 */

using namespace std;

#include "Game.h"
#include "SignManager.h"
#include "Random.h"

int SignManager::sign_count;
Sign SignManager::signs[DC_MAX_SIGN_NUMBER];

// maximum levels for each sign type
const int maximum_levels[3]
 = { 8, 10, 8 };

// which sign texture contains which signs
const int texture_mapping[3][11]
 = { { 0, 0, 0, 0, 0, 0, 1, 1, 1 },
     { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
     { 1, 1, 1, 1, 1, 1, 1, 1, 1 } };

// where a sign's subtexture is located
const GLfloat subtexture_s_mapping[3][11]
 = { { 0.0f, 0.0f, DC_SIGN_SMALL_SUBTEX_COORD_S, DC_SIGN_SMALL_SUBTEX_COORD_S,
       2.0f * DC_SIGN_SMALL_SUBTEX_COORD_S, 2.0f * DC_SIGN_SMALL_SUBTEX_COORD_S,
       0.0f, 0.0f, 0.0f },
     { 0.0f, 1.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       DC_SIGN_LARGE_SUBTEX_COORD_S, DC_SIGN_LARGE_SUBTEX_COORD_S,
       DC_SIGN_LARGE_SUBTEX_COORD_S, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S },
     { 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       3.0f * DC_SIGN_LARGE_SUBTEX_COORD_S } };
const GLfloat subtexture_t_mapping[3][11]
 = { { 0.0f, DC_SIGN_SMALL_SUBTEX_COORD_T, 0.0f, DC_SIGN_SMALL_SUBTEX_COORD_T,
       0.0f, DC_SIGN_SMALL_SUBTEX_COORD_T, 0.0f, DC_SIGN_LARGE_SUBTEX_COORD_T,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_T },
     { 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_T, 0.0f, DC_SIGN_LARGE_SUBTEX_COORD_T,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_T, 3.0f * DC_SIGN_LARGE_SUBTEX_COORD_T,
       0.0f, DC_SIGN_LARGE_SUBTEX_COORD_T, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_T,
       3.0f * DC_SIGN_LARGE_SUBTEX_COORD_T, 0.0f,
       DC_SIGN_LARGE_SUBTEX_COORD_T },
     { 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S, 2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S,
       2.0f * DC_SIGN_LARGE_SUBTEX_COORD_S } };

void SignManager::initialize (   )
{
  sign_count = 0;
  for (int n = DC_MAX_SIGN_NUMBER; n--; )
    signs[n].active = false;
}

void SignManager::createSign ( int x, int y, int type, int level )
{
  if (sign_count == DC_MAX_SIGN_NUMBER) return;

  int n;
  for (n = 0; signs[n].active; n++);
  Sign &sign = signs[n];

  if (type == ST_MULTIPLIER) {
    sign.g_x = x + 1;
    sign.g_y = y - 1;
    if (!confirmSignLocation(signs[n]))
      if (sign.g_x > 1)
        sign.g_x--;
      else
        sign.g_y++;

  } else {
    sign.g_x = x;
    sign.g_y = y;
    if (!confirmSignLocation(sign))
      if (sign.g_x < GC_PLAY_WIDTH - 1)
        sign.g_x++;
      else
        sign.g_y++;
  }

  while (!confirmSignLocation(sign))
    sign.g_y++;

  sign.x = sign.g_x * DC_GRID_ELEMENT_LENGTH
   + (DC_PLAY_OFFSET_X + DC_SIGN_OFFSET_X - DC_SIGN_OFFSET_SPREAD)
   + (2 * DC_SIGN_OFFSET_SPREAD) * Random::number();
  sign.y = sign.g_y * DC_GRID_ELEMENT_LENGTH
   + Displayer::play_offset_y + (DC_SIGN_OFFSET_Y - DC_SIGN_OFFSET_SPREAD)
   + (2 * DC_SIGN_OFFSET_SPREAD) * Random::number();

  if (level > maximum_levels[type])
    level = maximum_levels[type];

  sign.texture = texture_mapping[type][level];
  sign.subtexture_s = subtexture_s_mapping[type][level];
  sign.subtexture_t = subtexture_t_mapping[type][level];

  if (type != ST_SPECIAL)
    sign.color = 0;
  else
    sign.color = level;

  sign.life_time = 0;

  sign_count++;
  sign.active = true;
}

void SignManager::timeStep_inline_split_ (   )
{
  int c = sign_count;
  for (int n = 0; c; n++)
    if (signs[n].active) {
      Sign &sign = signs[n];
      c--;

      if (++sign.life_time == DC_SIGN_LIFE_TIME) {
        sign_count--;
        sign.active = false;
      }

      if (sign.life_time < DC_SIGN_ACCELERATION_TIME)
        sign.y += DC_SIGN_ACCELERATION * sign.life_time;
       
      else
        sign.y += DC_SIGN_TERMINAL_VELOCITY;
    }
}
      
