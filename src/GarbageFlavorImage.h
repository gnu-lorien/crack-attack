/*
 * GarbageFlavorImage.h
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

#ifndef GARBAGEFLAVORIMAGE_H
#define GARBAGEFLAVORIMAGE_H

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Displayer.h"
#include "Garbage.h"
#include "Random.h"

/* static */ class GarbageFlavorImage {
public:
  static void initialize (   );

  static bool personalGarbageFlavorImageExists (   );
  static GLubyte *loadPersonalGarbageFlavorImage (   );
  static void handleNetworkGarbageFlavorImage ( GLubyte *texture );

  static void buildGarbageTextureFileName ( char file_name[256],
   const char *dir_name, int n );
  static void buildGarbageTextureFileName ( char file_name[256], int n );
  static void buildOriginalGarbageTextureFileName ( char file_name[256],
   int n );

  static void requestGarbageFlavorImage_inline_split_ ( Garbage &garbage );

  static inline void requestGarbageFlavorImage ( Garbage &garbage )
  {
    if (garbage.height < DC_MIN_FLAVOR_GARBAGE_LENGTH) return;
    if (associated_garbage_id != -1) return;
    if (!Random::number(DC_CHANCE_NO_GARBAGE_FLAVOR)) return;

    requestGarbageFlavorImage_inline_split_(garbage);
  }

  static inline void notifyGarbageDestruction ( Garbage &garbage )
  {
    if (garbage.id == associated_garbage_id)
      associated_garbage_id = -1;
  }

  static int current_texture;
  static int associated_garbage_id;
  static int x, y;
  static bool network_texture;
};

#endif
