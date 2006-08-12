/*
 * SparkleManager.h
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

#ifndef SPARKMANAGER_H
#define SPARKMANAGER_H


#include "Displayer.h"

class Spark {
public:
  bool active;
  float x, y;
  float v_x, v_y;
  float a;
  float v_a;
  float size;
  int color;
  int life_time;
};

class Mote {
public:
  bool active;
  float x, y;
  float v_x, v_y;
  float a;
  float initial_a;
  float v_a;
  int color;
  int type;
  float size;
  float inverse_mass;
  float brightness;
  int life_time;
  int sibling_delay;
  int associated_light;
  int light_color;
};

/* static */ class SparkleManager {
public:
  static void initialize (   );
  static void timeStep (   );

  static void createBlockDeathSpark ( int x, int y, int color, int n );
  static void createCelebrationSpark ( int source, int color );
  static void createRewardMote ( int x, int y, int level,
   int sibling_number = 0 );

  // the block death sparkles
  static int spark_count;
  static Spark sparks[DC_MAX_SPARK_NUMBER];

  // the combo reward sparkles
  static int mote_count;
  static Mote motes[DC_MAX_MOTE_NUMBER];

private:
  static const int mote_colors[DC_NUMBER_MOTE_LEVELS];
  static const int mote_light_colors[DC_NUMBER_MOTE_LEVELS];
  static const int mote_types[DC_NUMBER_MOTE_LEVELS];
  static const GLfloat mote_sizes[DC_NUMBER_MOTE_LEVELS];
  static const float mote_inverse_masses[DC_NUMBER_MOTE_LEVELS];
};

#endif
