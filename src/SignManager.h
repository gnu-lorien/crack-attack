/*
 * SignManager.h
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

#ifndef SIGNMANAGER_H
#define SIGNMANAGER_H

using namespace std;

#include "Displayer.h"

// sign types
#define ST_MAGNITUDE                      (0)
#define ST_MULTIPLIER                     (1)
#define ST_SPECIAL                        (2)

// sign textures
#define ST_SMALL_TEXTURE                  (0)
#define ST_LARGE_TEXTURE                  (1)

// content bases
#define SC_BASE_MAGNITUDE_CONTENT         (0)
#define SC_BASE_MULTIPLIER_CONTENT        (13)

class Sign {
public:
  bool active;
  float x, y;
  int texture;
  GLfloat subtexture_s;
  GLfloat subtexture_t;
  GLfloat size;
  int life_time;
  int g_x, g_y;
  int color;
};

/* static */ class SignManager {
public:
  static void initialize (   );

  static inline void timeStep (   )
  {
    if (sign_count > 0) timeStep_inline_split_();
  }

  static void createSign ( int x, int y, int type, int level );

  static int sign_count;
  static Sign signs[DC_MAX_SIGN_NUMBER];

private:
  static void timeStep_inline_split_ (   );

  static inline bool confirmSignLocation ( Sign &sign )
  {
    if (sign.g_y <= 0) return false;
    if (sign.g_x <= 0 || sign.g_x >= GC_PLAY_WIDTH) return false;

    // sign being created is still inactive
    int c = sign_count;
    for (int n = 0; c; n++)
      if (signs[n].active) {
        c--;
        if (sign.g_x == signs[n].g_x && sign.g_y == signs[n].g_y) return false;
      }

    return true;
  }
};

#endif
