/*
 * LoseBar.cxx
 * Daniel Nelson - 3/1/2
 *
 * Copyright (C) 2002  Daniel Nelson
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
 * Handles the losebar.
 */

#include <GL/glut.h>
#include <cassert>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Creep.h"
#include "Displayer.h"
#include "LoseBar.h"

GLfloat LoseBar::bar;
int LoseBar::fade_timer;
int LoseBar::state;

void LoseBar::initialize (   )
{
  bar = 0.0f;
  fade_timer = 0;
  state = LB_INACTIVE;

}

void LoseBar::gameStart (   )
{
  // at the start of the game, fade to inactive
  if (state == LB_LOW_ALERT)
    enterLowToInactiveFade();
  else if (state == LB_HIGH_ALERT)
    enterHighToInactiveFade();
}

void LoseBar::timeStep (   )
{
  // update state

  // if we're not on alert
  if (state
   & (LB_INACTIVE | LB_FADE_LOW_TO_INACTIVE | LB_FADE_HIGH_TO_INACTIVE)) {

    // if we're done fading
    if (state & (LB_FADE_LOW_TO_INACTIVE | LB_FADE_HIGH_TO_INACTIVE)
     && --fade_timer == 0)
      state = LB_INACTIVE;

    // if we should be on alert
    if (Creep::creep_freeze) {
      // no check for high alert is needed
      state = LB_LOW_ALERT;
      fade_timer = 0;
    }

  // if we are on low alert
  } else if (state == LB_LOW_ALERT) {

    // if we no longer should be on alert
    if (!Creep::creep_freeze)
      enterLowToInactiveFade();

    // if we should enter high alert
    else if (Creep::loss_alarm <= GC_LOSS_DELAY_ELIMINATION)
      state = LB_HIGH_ALERT;

  // if we are on high alert
  } else if (state == LB_HIGH_ALERT) {

    // if we no longer should be on alert
    if (!Creep::creep_freeze)
      enterHighToInactiveFade();

  // if we are fading because of a high alert reset
  } else if (state == LB_FADE_RESET_HIGH) {
    if (--fade_timer == 0)
      state = LB_HIGH_ALERT;
  }

  // calculate the bar value

  if (state & (LB_LOW_ALERT | LB_HIGH_ALERT))
    if (state & LB_LOW_ALERT)
      bar = (GC_LOSS_DELAY - Creep::loss_alarm)
       * (1.0f / (GLfloat) (GC_LOSS_DELAY - GC_LOSS_DELAY_ELIMINATION));
    else
      bar = (GC_LOSS_DELAY_ELIMINATION - Creep::loss_alarm)
       * (1.0f / (GLfloat) GC_LOSS_DELAY_ELIMINATION);
}
