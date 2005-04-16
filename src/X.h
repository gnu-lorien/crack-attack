/*
 * X.h
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

#ifndef X_H
#define X_H

#include <GL/glut.h>
#include <cassert>

#include "glext.h"

using namespace std;

#include "Game.h"
#include "Random.h"
#include "Garbage.h"
#include "Block.h"

class Wild {
public:
  bool active;
  int flavor;
  int alarm;
  Block *block;
};

/* static */ class X {
public:
  static void gameStart (   );
  static void timeStep (   );
  static void modifyHeadlightColor ( GLfloat headlight_color[3] );
  static void notifyImpact ( Garbage &garbage );
  static void notifyShatter ( Garbage &garbage );

  static inline bool reverseControls (   )
  {
    return reverse_controls != 0;
  }

  static inline bool invisibleSwapper (   )
  {
    return swapper_alpha != GC_INVISIBLE_MAX_ALPHA;
  }

  static inline bool needDrawSwapper (   )
  {
    return swapper_alpha > 0;
  }

  static inline GLfloat swapperAlpha (   )
  {
    assert(needDrawSwapper());
    return swapper_alpha * (1.0f / (float) GC_INVISIBLE_MAX_ALPHA);
  }

  static inline bool crazyLights (   )
  {
    return light_mode != -1 && (light_mode & (1 << 0));
  }

  static inline bool wildActive (   )
  {
    return wild_count != 0;
  }

  static inline bool wildAllowed (   )
  {
    return wild_count != GC_MAX_WILD_NUMBER;
  }

  static inline void activateWild ( Block &block )
  {
    assert(wildAllowed());

    int n;
    for (n = 0; wilds[n].active; n++);

    block.X = n;

    wilds[n].active = true;
    wilds[n].block = &block;
    wilds[n].flavor = Random::number(BF_WILD + 1);
    wilds[n].alarm = 180;

    wild_count++;
  }

  static inline int wildFlavor ( Block &block )
  {
    assert(block.flavor == BF_WILD);

    if (wilds[block.X].alarm >= GC_WILD_POLYMORPH_PERIOD)
      return wilds[block.X].flavor;
    else
      return BF_WILD;
  }

  static inline Wild &wild ( Block &block )
  {
    assert(block.flavor == BF_WILD);

    return wilds[block.X];
  }

  static inline void deactivateWild ( Block &block )
  {
    assert(block.flavor == BF_WILD);

    wilds[block.X].active = false;
    wild_count--;
  }

  static inline bool specialColorActive (   )
  {
    return special_color_count != 0;
  }

  static inline bool specialColorAllowed (   )
  {
    return special_color_count != GC_MAX_SPECIAL_COLOR_NUMBER;
  }

  static inline void activateSpecialColor ( Block &block )
  {
    assert(specialColorAllowed());

    // sloppy but effective way of insuring each special color block's gleam
    // is distinct
    block.X = Random::number2((1 << 20));
    special_color_count++;
  }

  static inline void deactivateSpecialColor (   )
  {
    special_color_count--;
  }

  static int reverse_controls;
  static int invisible_swapper;
  static int crazy_lights;

  static int swapper_alpha;

  static int light_mode;
  static int light_alarm;
  static const GLfloat light_level_map[6][3];

  static int wild_count;
  static Wild wilds[GC_MAX_WILD_NUMBER];

  static int special_color_count;
};

#endif
