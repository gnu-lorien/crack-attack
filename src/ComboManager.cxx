/*
 * ComboManager.cxx
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
 *
 * Watches for finished combo tabulators and generates the appropriate garbage
 * blocks.  Handles the combo tabulator free store.
 */

#include "Game.h"
#include "ComboManager.h"
#include "ComboTabulator.h"
#include "GarbageGenerator.h"
#include "Score.h"

using namespace std;

ComboTabulator ComboManager::tabulatorStore[GC_COMBO_TABULATOR_STORE_SIZE];
bool ComboManager::storeMap[GC_COMBO_TABULATOR_STORE_SIZE];
int ComboManager::combo_count;

void ComboManager::gameStart (   )
{
  for (int n = GC_COMBO_TABULATOR_STORE_SIZE; n--; ) {
    storeMap[n] = false;
    tabulatorStore[n].id = n;
  }

  combo_count = 0;
}

void ComboManager::timeStep (   )
{
  // loop through the active combos
  int c = combo_count;
  for (int n = 0; c; n++) {
    if (!storeMap[n]) continue; 
    ComboTabulator &combo = tabulatorStore[n];
    c--;

    // if this combo is complete
    if (combo.involvement_count == 0) {

      // the garbage generator
      GarbageGenerator::comboComplete(combo);

      // delete the combo; circumvent deleteComboTabulator()
      freeId(n);

    // if an elimination in this combo just occured
    } else if (combo.time_stamp == Game::time_step) {

      // notify the score
      int score = Score::reportElimination(combo);
      combo.base_accumulated_score += score;
      combo.base_score_this_step += score;

      if (combo.n_multipliers_this_step != 0)
        Score::reportMultiplier(combo);
      combo.base_score_this_step = 0;

      // notify the garbage generator; removes the elimination magnitude
      GarbageGenerator::comboElimination(combo);
    }
  }
}
