/*
 * WinRecord.cxx
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
 * Handles the win/loss record.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "WinRecord.h"
#include "Displayer.h"
#include "Random.h"

int WinRecord::current_game;
bool WinRecord::won;
bool WinRecord::concession;
int WinRecord::games_won;
int WinRecord::games_lost;
int WinRecord::record[GC_GAMES_PER_MATCH];

Star WinRecord::stars[GC_GAMES_PER_MATCH];
int WinRecord::dynamic_star;
int WinRecord::displaced_star;
GLfloat WinRecord::win_star_x;
GLfloat WinRecord::win_star_y;
GLfloat WinRecord::win_star_v_x;
GLfloat WinRecord::win_star_v_y;
GLfloat WinRecord::old_star_a;
GLfloat WinRecord::old_star_size;
bool WinRecord::draw_old_star; 

void WinRecord::initialize (   )
{
  for (int n = GC_GAMES_PER_MATCH; n--; )
    record[n] = GR_NOT_PLAYED;
  current_game = -1;
  concession = false;
  games_won = 0;
  games_lost = 0;

  dynamic_star = -1;
  displaced_star = -1;
  for (int n = GC_GAMES_PER_MATCH; n--; )
    stars[n].a = 0.0f;
}

void WinRecord::gameStart (   )
{
  if (MetaState::mode & CM_SOLO)
    current_game = 1;
  else
    current_game++;

  record[current_game] = GR_BEING_PLAYED;
}

void WinRecord::gameWon (   )
{
  won = true;
  games_won++;
  record[current_game] = GR_WON;

  // The star will disappear and pop into existance in the play area.  After a
  // few moments, it will fly to it's original location.

  // record star's old location; for a time, two stars will be drawn, as one
  // pops in and the other pops away
  draw_old_star = true;
  old_star_a = stars[current_game].a;
  old_star_size = 5.0f;

  // displace the star into the play area and set it's starting parameters
  displaced_star = current_game;
  stars[current_game].a = DC_STAR_WIN_MIN_ANGULAR_DEVIATION
   + DC_STAR_WIN_SPREAD_ANGULAR_DEVIATION * Random::number();
  if (Random::chanceIn2(2))
    stars[current_game].a = -stars[current_game].a;
  stars[current_game].v_a = 0.0f;
  stars[current_game].size = 0.0f;
  stars[current_game].v_size = 0.0f;
  win_star_x = (-DC_LEFT_EXTERNAL_CENTER + DC_STAR_DISPLACEMENT
   * (GC_GAMES_PER_MATCH - 1) / 2.0f + DC_STAR_WIN_OFFSET_X)
   - DC_STAR_DISPLACEMENT * current_game;

  if (MetaState::mode & CM_SOLO)
    win_star_y = -DC_STAR_OFFSET_Y + DC_STAR_WIN_SOLO_OFFSET_Y;
  else
    win_star_y = -DC_STAR_OFFSET_Y + DC_STAR_WIN_OFFSET_Y;

  // set the kick velocity; choice of two aesthetically pleasing preset values
  // or, for variety, a random 270 degree arc
  switch (Random::number(3)) {
  case 0:
    win_star_v_x = DC_STAR_WIN_PRESET_1_VELOCITY_X;
    win_star_v_y = DC_STAR_WIN_PRESET_1_VELOCITY_Y;
    break;
  case 1:
    win_star_v_x = DC_STAR_WIN_PRESET_2_VELOCITY_X;
    win_star_v_y = DC_STAR_WIN_PRESET_2_VELOCITY_Y;
    break;
  default:
    // too infrequent to warrent a random direction table
    float v = DC_STAR_WIN_MIN_VELOCITY
     + DC_STAR_WIN_SPREAD_VELOCITY * Random::number();
    float angle = Random::number() * (3.0f * PI / 2.0f) - (PI / 2.0f);
    win_star_v_x = v * cos(angle);
    win_star_v_y = v * sin(angle);
    break;
  }
}

void WinRecord::gameLoss (   )
{
  won = false;
  games_lost++;
  record[current_game] = GR_LOST;
}

void WinRecord::timeStep (   )
{
  for (int n = GC_GAMES_PER_MATCH; n--; ) {
    Star &star = stars[n];

    switch (record[n]) {

    // game being played
    case GR_BEING_PLAYED:
      if (Game::time_step >= GC_START_PAUSE_DELAY)
        star.a += DC_STAR_PLAY_ANGULAR_VELOCITY;
      else
        star.a += Game::time_step
         * (DC_STAR_PLAY_ANGULAR_VELOCITY / (float) GC_START_PAUSE_DELAY);
      break;

    // game has been lost
    case GR_LOST:
      if (current_game == n && Game::time_step < DC_CELEBRATION_TIME)
        star.a += (DC_CELEBRATION_TIME - Game::time_step)
         * (DC_STAR_PLAY_ANGULAR_VELOCITY / (float) DC_CELEBRATION_TIME);
      break;

    // game has been won
    case GR_WON:
      // wait for win message to hit before dynamics
      if (current_game != n || Game::time_step > DC_WIN_FADE_TIME) {

        star.size += star.v_size;
        if (star.size < 0.0f) star.size = 0.0f;
        star.v_size += -DC_STAR_WIN_SIZE_DRAG * star.v_size
         - DC_STAR_WIN_SIZE_SPRING * (star.size - DC_STAR_SIZE_EQUILIBRIUM);

        if (fabs(star.v_size) < DC_STAR_WIN_SIZE_EPSILON
         && Random::chanceIn(DC_STAR_WIN_SIZE_PULSE_CHANCE_IN))
          star.v_size += DC_STAR_WIN_SIZE_PULSE_VELOCITY * Random::number();

        star.v_a += -DC_STAR_WIN_ANGULAR_SPRING * star.a;

        star.a += star.v_a;

        // begin motion
        if (Game::time_step == DC_WIN_FADE_TIME + DC_STAR_WIN_KICK_DELAY
         && current_game == n) {
          displaced_star = -1;
          dynamic_star = n;
        }

        // if we're dynamic;
        if (dynamic_star == n) {
          win_star_x += win_star_v_x;
          win_star_y += win_star_v_y;

          win_star_v_x += -DC_STAR_WIN_SPRING * win_star_x
           - DC_STAR_WIN_DRAG * win_star_v_x;
          win_star_v_y += -DC_STAR_WIN_SPRING * win_star_y
           - DC_STAR_WIN_DRAG * win_star_v_y;

          // if we're there, stop being dynamic
          if (fabs(win_star_v_x) < DC_STAR_WIN_VELOCITY_EPSILON
           && fabs(win_star_v_y) < DC_STAR_WIN_VELOCITY_EPSILON
           && fabs(win_star_x) < DC_STAR_WIN_EPSILON
           && fabs(win_star_y) < DC_STAR_WIN_EPSILON)
            dynamic_star = -1;
        }
      }

      // shrink the old star as the new one apears
      if (draw_old_star && current_game == n) {
        if (star.size < DC_STAR_SIZE_EQUILIBRIUM) {
          old_star_size = DC_STAR_SIZE_EQUILIBRIUM - star.size;
          old_star_a += DC_STAR_PLAY_ANGULAR_VELOCITY;
        } else
          draw_old_star = false;
      }

      break;
    }
  }
}
