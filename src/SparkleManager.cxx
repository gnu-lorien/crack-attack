/*
 * SparkleManager.cxx
 * Daniel Nelson - 9/4/0
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
 * Handles all the pretty sparkles!
 */

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "MetaState.h"
#include "Random.h"
#include "SparkleManager.h"

int SparkleManager::spark_count;
Spark SparkleManager::sparks[DC_MAX_SPARK_NUMBER];

int SparkleManager::mote_count;
Mote SparkleManager::motes[DC_MAX_MOTE_NUMBER];

const int SparkleManager::mote_colors[DC_NUMBER_MOTE_LEVELS]
 = { 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11,
     0, 0, 0, 1, 2, 3, 3, 3, 3, 3, 3 };

const int SparkleManager::mote_light_colors[DC_NUMBER_MOTE_LEVELS]
 = { 0, 0, 0, 0, 1, 0, 2, 3, 4, 5, 6,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const int SparkleManager::mote_types[DC_NUMBER_MOTE_LEVELS]
 = { MT_FOUR_POINTED_STAR, MT_FIVE_POINTED_STAR,
     MT_SIX_POINTED_STAR, MT_SPECIAL_STAR, MT_SPECIAL_STAR, MT_SPECIAL_STAR,
     MT_SPECIAL_STAR, MT_SPECIAL_STAR, MT_SPECIAL_STAR, MT_SPECIAL_STAR,
     MT_SPECIAL_STAR, MT_MULTIPLIER_ONE_STAR, MT_MULTIPLIER_TWO_STAR,
     MT_MULTIPLIER_THREE_STAR, MT_MULTIPLIER_THREE_STAR,
     MT_MULTIPLIER_THREE_STAR, MT_MULTIPLIER_THREE_STAR,
     MT_MULTIPLIER_THREE_STAR, MT_MULTIPLIER_THREE_STAR,
     MT_MULTIPLIER_THREE_STAR, MT_MULTIPLIER_THREE_STAR,
     MT_MULTIPLIER_THREE_STAR };

const GLfloat SparkleManager::mote_sizes[DC_NUMBER_MOTE_LEVELS]
 = { 2.0f, 2.8f, 2.8f, 3.4f, 3.4f, 3.4f, 3.4f, 3.4f, 3.4f, 3.4f, 3.4f,
     4.0f, 2.6f, 3.5f, 3.7f, 3.9f, 4.1f, 4.3f, 4.5f, 4.7f, 4.9f, 5.1f };

const float SparkleManager::mote_inverse_masses[DC_NUMBER_MOTE_LEVELS]
 = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f / 1.4f, 1.0f / 1.8f, 1.0f / 2.2f,
     1.0f / 2.6f, 1.0f / 3.0f, 1.0f / 3.4f, 1.0f / 3.8f, 1.0f / 4.2f };

void SparkleManager::initialize (   )
{
  spark_count = 0;
  for (int n = DC_MAX_SPARK_NUMBER; n--; )
    sparks[n].active = false;

  mote_count = 0;
  for (int n = DC_MAX_MOTE_NUMBER; n--; )
    motes[n].active = false;
}

void SparkleManager::createBlockDeathSpark ( int x, int y, int color, int n )
{
  while (n--) {
    if (spark_count == DC_MAX_SPARK_NUMBER) return;

    int n;
    for (n = 0; sparks[n].active; n++);
    Spark &spark = sparks[n];

    spark_count++;
    spark.active = true;
    spark.x = x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X;
    spark.y = y * DC_GRID_ELEMENT_LENGTH + Displayer::play_offset_y;

    float v = DC_MIN_SPARK_VELOCITY + (Random::number() + Random::number())
     * (0.5f * (DC_MAX_SPARK_VELOCITY - DC_MIN_SPARK_VELOCITY));
    Random::deathSparkAngle(spark.v_x, spark.v_y);
    spark.v_x *= v;
    spark.v_y *= v;

    spark.a = Random::number(360);
    spark.v_a = DC_MIN_SPARK_ANGULAR_VELOCITY
     + (Random::number() + Random::number())
     * (0.5f * (DC_MAX_SPARK_ANGULAR_VELOCITY - DC_MIN_SPARK_ANGULAR_VELOCITY));
    if (Random::chanceIn2(2))
      spark.v_a = -spark.v_a;

    switch (Random::number2(4)) {
    case 0:
      spark.size = DC_MIN_SPARK_SIZE;
      break;
    case 1:
      spark.size = DC_MIN_SPARK_SIZE + Random::number()
       * (1.0f - DC_MIN_SPARK_SIZE);
      break;
    default:
      spark.size = 1.0f;
      break;
    }

    spark.color = color;

    if (Random::chanceIn(DC_CHANCE_LONG_SPARK_LIFE_TIME))
      spark.life_time = Random::number(10 * DC_SPREAD_SPARK_LIFE_TIME)
       + Random::number(10 * DC_SPREAD_SPARK_LIFE_TIME)
       + 10 * (DC_MEDIUM_SPARK_LIFE_TIME - DC_SPREAD_SPARK_LIFE_TIME);
    else
      spark.life_time = Random::number(DC_SPREAD_SPARK_LIFE_TIME)
       + Random::number(DC_SPREAD_SPARK_LIFE_TIME)
       + (DC_MEDIUM_SPARK_LIFE_TIME - DC_SPREAD_SPARK_LIFE_TIME);
  }
}

void SparkleManager::createCelebrationSpark ( int source, int color )
{
  if (spark_count == DC_MAX_SPARK_NUMBER) return;

  int n;
  for (n = 0; sparks[n].active; n++);
  Spark &spark = sparks[n];

  spark_count++;
  spark.active = true;

  float v;
  if (source != 4) {
    Random::celebrationSpark1Angle(spark.v_x, spark.v_y);
    v = DC_MIN_CSPARK_VELOCITY + DC_SPREAD_CSPARK_VELOCITY * Random::number();
  } else {
    Random::celebrationSpark2Angle(spark.v_x, spark.v_y);
    v = (2.0f * DC_MIN_CSPARK_VELOCITY)
     + (2.0f * DC_SPREAD_CSPARK_VELOCITY) * Random::number();
  }
  spark.v_x *= v;
  spark.v_y *= v;

  switch (source) {
  case 0:
    if (MetaState::mode & CM_SOLO) {
      spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_1_SOLO_OFFSET_X;
      spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_1_SOLO_OFFSET_Y;
    } else {
      spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_1_OFFSET_X;
      spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_1_OFFSET_Y;
    }
    break;
  case 1:
    spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_2_OFFSET_X;
    spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_2_OFFSET_Y;
    break;
  case 2:
    if (MetaState::mode & CM_SOLO) {
      spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_3_SOLO_OFFSET_X;
      spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_3_SOLO_OFFSET_Y;
    } else {
      spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_3_OFFSET_X;
      spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_3_OFFSET_Y;
    }
    spark.v_x = -spark.v_x;
    break;
  case 3:
    spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_4_OFFSET_X;
    spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_4_OFFSET_Y;
    spark.v_x = -spark.v_x;
    break;
  case 4:
    if (MetaState::mode & CM_SOLO) {
      spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_5_SOLO_OFFSET_X;
      spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_5_SOLO_OFFSET_Y;
    } else {
      spark.x = DC_PLAY_OFFSET_X + DC_SOURCE_5_OFFSET_X;
      spark.y = DC_PLAY_OFFSET_Y + DC_SOURCE_5_OFFSET_Y;
    }
    break;
  }

  spark.a = Random::number(360);
  spark.v_a = DC_MIN_SPARK_ANGULAR_VELOCITY
   + (Random::number() + Random::number())
   * (0.5f * (DC_MAX_SPARK_ANGULAR_VELOCITY - DC_MIN_SPARK_ANGULAR_VELOCITY));
  if (Random::chanceIn2(2))
    spark.v_a = -spark.v_a;

  if (Random::chanceIn(3))
    spark.size = DC_MIN_SPARK_SIZE
     + Random::number() * (1.0f - DC_MIN_SPARK_SIZE);
  else
    spark.size = DC_MIN_SPARK_SIZE;

  spark.color = color;

  if (Random::chanceIn(DC_CHANCE_LONG_SPARK_LIFE_TIME))
    spark.life_time = Random::number(10 * DC_SPREAD_SPARK_LIFE_TIME)
     + Random::number(10 * DC_SPREAD_SPARK_LIFE_TIME)
     + 10 * (DC_MEDIUM_SPARK_LIFE_TIME - DC_SPREAD_SPARK_LIFE_TIME);
  else
    spark.life_time = Random::number(DC_SPREAD_SPARK_LIFE_TIME)
     + Random::number(DC_SPREAD_SPARK_LIFE_TIME)
     + (DC_MEDIUM_SPARK_LIFE_TIME - DC_SPREAD_SPARK_LIFE_TIME);
}

void SparkleManager::createRewardMote ( int x, int y, int level,
 int sibling_number )
{
  if (mote_count == DC_MAX_MOTE_NUMBER) return;

  int n;
  for (n = 0; motes[n].active; n++);
  Mote &mote = motes[n];

  if (n == 0)
    glEnable(GL_LIGHT1);

  mote_count++;
  mote.active = true;

  if (level >= DC_NUMBER_MOTE_LEVELS)
    level = DC_NUMBER_MOTE_LEVELS - 1;
  mote.color = mote_colors[level];
  mote.light_color = mote_light_colors[level];
  mote.type = mote_types[level];
  mote.size = mote_sizes[level];
  mote.inverse_mass = mote_inverse_masses[level];

  mote.x = x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X
   - (DC_GRID_ELEMENT_LENGTH / 2.0f)
   + (float) Random::number(20) * ((float) DC_GRID_ELEMENT_LENGTH / 20.0f);
  mote.y = y * DC_GRID_ELEMENT_LENGTH + Displayer::play_offset_y
   - (DC_GRID_ELEMENT_LENGTH / 2.0f)
   + (float) Random::number(20) * ((float) DC_GRID_ELEMENT_LENGTH / 20.0f);

  // slow down big ones
  float v = (DC_MEDIUM_MOTE_VELOCITY - DC_SPREAD_MOTE_VELOCITY
   + Random::number() * (2.0f * DC_SPREAD_MOTE_VELOCITY)) * mote.inverse_mass;
  if (x < GC_PLAY_WIDTH / 2)
   mote.v_x = -0.707107 * v;
  else
   mote.v_x = 0.707107 * v;
  mote.v_y = -0.707107 * v;

  mote.a = mote.initial_a = Random::number(360);
  mote.v_a = (DC_MEDIUM_MOTE_ANGULAR_VELOCITY
   - DC_SPREAD_MOTE_ANGULAR_VELOCITY + Random::number()
   * (2.0f * DC_SPREAD_MOTE_ANGULAR_VELOCITY)) * mote.inverse_mass;
  if (Random::chanceIn2(2))
    mote.v_a = -mote.v_a;

  mote.life_time = 0;
  mote.sibling_delay = sibling_number * DC_MULTI_MOTE_FIRE_DELAY;

  mote.associated_light = -1;
}

void SparkleManager::timeStep (   )
{
  int c = spark_count;
  for (int n = 0; c; n++)
    if (sparks[n].active) {
      Spark &spark = sparks[n];
      c--;

      if (--spark.life_time == 0) {
        spark.active = false;
        spark_count--;

      } else {
        spark.x += spark.v_x;
        spark.y += spark.v_y;

        spark.a += spark.v_a;

        spark.v_y -= DC_SPARK_GRAVITY + DC_SPARK_DRAG * spark.v_y;
        spark.v_x -= DC_SPARK_DRAG * spark.v_x;
      }
    }

  c = mote_count;
  for (int n = 0; c; n++)
    if (motes[n].active) {
      Mote &mote = motes[n];
      c--;

      if (mote.life_time >= 0)
        if (++mote.life_time - mote.sibling_delay < GC_DYING_DELAY) {
          mote.a += mote.v_a;
          if (mote.life_time <= GC_DYING_DELAY)
            mote.brightness = DC_MAX_MOTE_LIGHT_BRIGHTNESS
             * mote.life_time * (1.0f / (float) GC_DYING_DELAY);
          continue;
        } else
          mote.life_time = -1;
      else if (mote.color > 0 && mote.color < DC_FIRST_SPECIAL_MOTE_COLOR)
        mote.life_time--;

      mote.y += mote.v_y;
      if (mote.y > DC_PLAY_HEIGHT + mote.size * (DC_SPARKLE_LENGTH / 2.0f)) {
        mote.active = false;
        mote_count--;
        continue;
      }

      mote.x += mote.v_x;

      mote.a += mote.v_a;

      mote.v_y += mote.inverse_mass * DC_MOTE_UPWARD_FORCE
       - DC_MOTE_DRAG * mote.v_y;
      mote.v_x -= mote.inverse_mass * DC_MOTE_CENTER_SPRING * mote.x
       + DC_MOTE_DRAG * mote.v_x;

      mote.v_a -= mote.inverse_mass
       * DC_MOTE_TWIST_SPRING * (mote.a - mote.initial_a);
    }
}
