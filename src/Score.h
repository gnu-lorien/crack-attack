/*
 * Score.h
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

#ifndef SCORE_H
#define SCORE_H

using namespace std;

#include "Game.h"
#include "ComboTabulator.h"
#include "MetaState.h"

class Rank {
public:
  char name[GC_PLAYER_NAME_LENGTH];
  int score;
};

/* static */ class Score {
public:
  static void initialize (   );
  static void cleanUp (   );
  static int gameFinish (   );

  static bool readScoreRecord (   );
  static void writeScoreRecord (   );
  static void setupDefaultScoreRecord (   );

  static inline void timeStepMeta (   )
  {
    if (!(MetaState::mode & CM_SOLO)) return;

    if (fade_timer == 0) return;
    fade_timer--;
  }

  static inline void timeStepPlay (   )
  {   
    if (!(MetaState::mode & CM_SOLO)) return;

    if (fade_timer == 0) {
      if (backlog > 0) {
        backlog--;
        score++;
        incrementDigits();
        fade_timer = GC_MAX_SCORE_INCREMENT_DELAY
         - (GC_SCORE_DELAY_SLOPE * backlog);
        if (fade_timer < GC_MIN_SCORE_INCREMENT_DELAY) {
          fade_timer = GC_MIN_SCORE_INCREMENT_DELAY;
          inverse_timer_start = 1.0f / (GLfloat) GC_MIN_SCORE_INCREMENT_DELAY;
        } else
          inverse_timer_start = 1.0f / (GLfloat) fade_timer;
      }
    } else
      fade_timer--;
  }

  static inline void incrementDigits (   )
  {
    for (int i = GC_NUMBER_DIGITS; i--; )
      previous_digits[i] = digits[i];

    int i;
    for (i = 0; i < GC_NUMBER_DIGITS; i++) {
      digits[i]++;
      if (digits[i] != 10) break;
      digits[i] = 0;
    }
    if (++i > n_digits_displayed && i <= GC_NUMBER_DIGITS)
      n_digits_displayed = i;
  }

  static inline void reportMultiplier ( ComboTabulator &combo )
  {
    if (!(MetaState::mode & CM_SOLO)) return;

    // multiply this step's score
    backlog += combo.base_score_this_step
     * (combo.multiplier - combo.n_multipliers_this_step - 1);

    // give another helping of the accumulated score
    backlog += combo.base_accumulated_score
     * combo.n_multipliers_this_step;

    combo.n_multipliers_this_step = 0;
  }

  static inline int reportElimination ( ComboTabulator &combo )
  {
    if (!(MetaState::mode & CM_SOLO)) return 0;

    int points = 0;

    // gray elimination score
    if (combo.special_magnitude > 0)
      points
       += GC_GRAY_SCORE * (combo.special_magnitude == GC_MIN_PATTERN_LENGTH
       ? GC_MIN_PATTERN_SCORE
       : combo.special_magnitude);

    // colored elimination score
    else
      points += (combo.magnitude == GC_MIN_PATTERN_LENGTH
       ? GC_MIN_PATTERN_SCORE
       : combo.magnitude);

    // special block bonuses
    for (int n = BF_NUMBER_SPECIAL; n--; )
      points += combo.special[n] * special_block_scores[n];

    backlog += points;

    return points;
  }

  static inline bool Score::topRank (   )
  {
    return player_rank == GC_SCORE_REC_LENGTH - 1;
  }

  static int score;
  static int backlog;
  static short digits[GC_NUMBER_DIGITS];
  static short previous_digits[GC_NUMBER_DIGITS];
  static int fade_timer;
  static GLfloat inverse_timer_start;
  static int n_digits_displayed;

  static int player_rank;
  static Rank record[GC_SCORE_REC_LENGTH];

  static int special_block_scores[BF_NUMBER_SPECIAL];
};

#endif
