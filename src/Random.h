/*
 * Random.h
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

#ifndef RANDOM_H
#define RANDOM_H


#include "Game.h"

class Angle {
public:
  float x;
  float y;
};

/* static */ class Random {
public:
  static void seed ( unsigned int seed );
  static unsigned int generateSeed (   );
  static void initialize (   );

  static inline bool chanceIn ( int chance )
    { return rand() % chance == 0; }

  // use then chance is a power of 2
  static inline bool chanceIn2 ( int chance )
    { return (rand() & (chance - 1)) == 0; }

  static inline int number ( int maximum )
    { return rand() % maximum; }

  // use then maximum is a power of 2
  static inline int number2 ( int maximum )
    { return rand() & (maximum - 1); }

  static inline float number (   )
    { return rand() * (1.0 / (float) RAND_MAX); }

  static inline void angle ( float &x, float &y )
  {
    int n = number2(GC_SIZE_RANDOM_ANGLE_TABLE);
    x = angle_table[n].x;
    y = angle_table[n].y;
  }

  static inline void deathSparkAngle ( float &x, float &y )
  {
    int n = number2(GC_SIZE_RANDOM_ANGLE_TABLE);
    x = angle_death_spark_table[n].x;
    y = angle_death_spark_table[n].y;
  }

  static inline void celebrationSpark1Angle ( float &x, float &y )
  {
    int n = number2(GC_SIZE_RANDOM_ANGLE_TABLE);
    x = angle_celebration_spark_1_table[n].x;
    y = angle_celebration_spark_1_table[n].y;
  }

  static inline void celebrationSpark2Angle ( float &x, float &y )
  {
    int n = number2(GC_SIZE_RANDOM_ANGLE_TABLE);
    x = angle_celebration_spark_2_table[n].x;
    y = angle_celebration_spark_2_table[n].y;
  }

private:
  static Angle angle_table[GC_SIZE_RANDOM_ANGLE_TABLE];
  static Angle angle_death_spark_table[GC_SIZE_RANDOM_ANGLE_TABLE];
  static Angle angle_celebration_spark_1_table[GC_SIZE_RANDOM_ANGLE_TABLE];
  static Angle angle_celebration_spark_2_table[GC_SIZE_RANDOM_ANGLE_TABLE];
};

#endif
