/*
 * X.cxx
 * Daniel Nelson - 11/11/0
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
 * Holds the X-treme state.
 */

using namespace std;

#include "Game.h"
#include "Grid.h"
#include "Block.h"
#include "Swapper.h"
#include "X.h"

int X::reverse_controls;
int X::invisible_swapper;
int X::crazy_lights;
int X::swapper_alpha;
int X::light_mode;
int X::light_alarm;
int X::wild_count;
Wild X::wilds[GC_MAX_WILD_NUMBER];
int X::special_color_count;

const GLfloat X::light_level_map[6][3]
 = { { 0.0f, 1.0f, 1.0f },
     { 0.5f, 0.5f, 1.0f },
     { 1.0f, 0.0f, 1.0f },
     { 1.0f, 0.5f, 0.5f },
     { 1.0f, 1.0f, 0.0f },
     { 0.5f, 1.0f, 0.5f } };

void X::gameStart (   )
{
  reverse_controls = 0;

  invisible_swapper = 0;
  swapper_alpha = GC_INVISIBLE_MAX_ALPHA;

  crazy_lights = 0;
  light_mode = -1;

  wild_count = 0;
  for (int n = GC_MAX_WILD_NUMBER; n--; )
    wilds[n].active = false;

  special_color_count = 0;
}

void X::timeStep (   )
{
  // evolve the swapper's invisibleness
  if (invisible_swapper != 0 || swapper_alpha != GC_INVISIBLE_MAX_ALPHA)
    if (invisible_swapper != 0) {

      // drop the swapper's alpha
      if (swapper_alpha > 0)
        swapper_alpha -= GC_INVISIBLE_QUICK_DECAY_RATE;
      else if (swapper_alpha > GC_INVISIBLE_MIN_ALPHA)
        swapper_alpha--;

      // there is a chance the swapper's alpha will spike upward
      if (Random::chanceIn(GC_INVISIBLE_PULSE_CHANCE_IN)) {
        swapper_alpha += GC_INVISIBLE_PULSE_STRENGTH;
        if (swapper_alpha > GC_INVISIBLE_MAX_ALPHA)
          swapper_alpha = GC_INVISIBLE_MAX_ALPHA;
      }

    // raise the swapper's alpha back to normal
    } else
      swapper_alpha++;

  // evolve the wild blocks' state
  if (wild_count != 0) {
    int c = wild_count;
    for (int n = 0; c; n++)
      if (wilds[n].active) {
        c--;
        if (!--wilds[n].alarm) {
        
          // switch flavors
          wilds[n].alarm = GC_WILD_PERIOD;
          if (wilds[n].flavor == 0)
            wilds[n].flavor = BF_WILD;
          else {
            wilds[n].flavor--;
            if (wilds[n].block->y != 0)
              Grid::requestEliminationCheck(*wilds[n].block);
          }
        }
      }
  }

  // evolve the crazy lights
  if (crazy_lights != 0 || light_mode != -1) {

    // switch light colors
    if (!--light_alarm) {

      // if craziness continues
      if (crazy_lights != 0) {
        if (light_mode != 11)
          light_mode++;
        else
          light_mode = 0;

        // odd modes - non-white modes - are longer
        if (light_mode & (1 << 0))
          light_alarm = GC_CRAZY_LONG_MODE_PERIOD;
        else
          light_alarm = GC_CRAZY_SHORT_MODE_PERIOD;

      // end craziness
      } else
        light_mode = -1;
    }
  }
}

void X::notifyImpact ( Garbage &garbage )
/*
 * Garbage has impacted.  We must check to see if it's flavor has a special
 * effect.
 */
{
  // controls are reversed
  if (garbage.flavor == GF_REVERSE_CONTROLS) {
    reverse_controls++;
    if (invisible_swapper == 0)
      Swapper::color = 1;
    else
      Swapper::color = 3;

  // swapper becomes invisible
  } else if (garbage.flavor == GF_INVISIBLE_SWAPPER) {
    invisible_swapper++;
    if (reverse_controls == 0)
      Swapper::color = 2;
    else
      Swapper::color = 3;

  // lights go crazy
  } else if (garbage.flavor == GF_CRAZY_LIGHTS) {
    crazy_lights++;
    if (light_mode == -1) {
      light_mode = (Random::number(6) << 1) + 1;
      light_alarm = 150;
    }
  }
}

void X::notifyShatter ( Garbage &garbage )
/*
 * Garbage has shattered.  We must check to see if this effects the special
 * effects which are occuring.
 */
{
  if (garbage.flavor == GF_REVERSE_CONTROLS) {
    reverse_controls--;
    if (reverse_controls == 0)
      if (invisible_swapper == 0)
        Swapper::color = 0;
      else
        Swapper::color = 2;

  } else if (garbage.flavor == GF_INVISIBLE_SWAPPER) {
    invisible_swapper--;
    if (invisible_swapper == 0)
      if (reverse_controls == 0)
        Swapper::color = 0;
      else
        Swapper::color = 1;

  } else if (garbage.flavor == GF_CRAZY_LIGHTS)
    crazy_lights--;
}

void X::modifyHeadlightColor ( GLfloat headlight_color[3] )
/*
 * Called by LightManager to allow us the opportunity to modify the headlight
 * color.
 */
{
  assert(crazyLights());

  GLfloat fade = light_alarm * (2.0f / (float) GC_CRAZY_LONG_MODE_PERIOD);
  if (fade > 1.0f)
    fade = -1.0f + fade;
  else
    fade = 1.0f - fade;
  fade *= fade;

  headlight_color[0] = light_level_map[light_mode >> 1][0]
   + fade * (1.0f - light_level_map[light_mode >> 1][0]);
  headlight_color[1] = light_level_map[light_mode >> 1][1]
   + fade * (1.0f - light_level_map[light_mode >> 1][1]);
  headlight_color[2] = light_level_map[light_mode >> 1][2]
   + fade * (1.0f - light_level_map[light_mode >> 1][2]);
}
