/*
 * Random.cxx
 * Daniel Nelson - 11/10/0
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
 * Handles all the random number tables.
 */

#include <ctime>

using namespace std;

#include "Game.h"
#include "Random.h"

Angle Random::angle_table[GC_SIZE_RANDOM_ANGLE_TABLE];
Angle Random::angle_death_spark_table[GC_SIZE_RANDOM_ANGLE_TABLE];
Angle Random::angle_celebration_spark_1_table[GC_SIZE_RANDOM_ANGLE_TABLE];
Angle Random::angle_celebration_spark_2_table[GC_SIZE_RANDOM_ANGLE_TABLE];

void Random::seed ( unsigned int seed )
/*
 * Called by Communicator to seed the random numbers.
 */
{
  srand(seed);
}

unsigned int Random::generateSeed (   )
/*
 * Called by Communicator if we're the server.  The result is used to seed our
 * random numbers as well as sent to the client for theirs.
 */
{
  return (unsigned int) time(null);
}

void Random::initialize (   )
/*
 * Attack calls seed() before this.
 */
{
  for (int n = GC_SIZE_RANDOM_ANGLE_TABLE; n--; ) {
    float angle = (2.0f * PI) * Random::number();
    angle_table[n].x = cos(angle);
    angle_table[n].y = sin(angle);
  }

  for (int n = GC_SIZE_RANDOM_ANGLE_TABLE; n--; ) {
    float angle = (PI / 4.0f) + (PI / 2.0f) * Random::number();
    angle_death_spark_table[n].x = cos(angle);
    angle_death_spark_table[n].y = sin(angle);
  }

  for (int n = GC_SIZE_RANDOM_ANGLE_TABLE; n--; ) {
    float angle = (3.0f * PI / 16.0f) + (PI / 8.0f) * Random::number();
    angle_celebration_spark_1_table[n].x = cos(angle);
    angle_celebration_spark_1_table[n].y = sin(angle);
  }

  for (int n = GC_SIZE_RANDOM_ANGLE_TABLE; n--; ) {
    float angle = (7.0f * PI / 16.0f) + (PI / 8.0f) * Random::number();
    angle_celebration_spark_2_table[n].x = cos(angle);
    angle_celebration_spark_2_table[n].y = sin(angle);
  }
}
