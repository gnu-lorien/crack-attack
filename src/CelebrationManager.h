/*
 * CelebrationManager.h
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

#ifndef CELEBRATIONMANAGER_H
#define CELEBRATIONMANAGER_H

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Displayer.h"

/* static */ class CelebrationManager {
public:
  static inline void initialize (   )
  {
    draw_game = false;
    light_level = 1.0f;
  }

  static void gameFinish (   );
  static void celebrationFinish (   );

  static void timeStep (   );

  static bool draw_game;
  static GLfloat light_level;

  // celebration display variables

  static int spark_rate[DC_CSPARK_SOURCE_NUMBER];
  static int spark_color[DC_CSPARK_SOURCE_NUMBER];

  static float win_alpha;
  static float win_scale;
  static int win_flash1;
  static int win_flash2;

  static float loss_height;
  static float loss_velocity;
  static bool loss_rescale;
  static int loss_bounce_count;
};

#endif
