/*
 * String.h
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

#ifndef STRING_H
#define STRING_H

using namespace std;

#include "Displayer.h"

/* static */ class String {
public:
  static int mapCharToCode ( char c );
  static int stringWidth ( const char *string, int max_width );
  static void readyLetterTextures (   );
  static void freeLetterTextures (   );
  static void fillStringTexture ( const char *string, GLubyte *texture,
   int width, bool use_alpha = false, int texture_width = 0 );
  
private:
  static const char *letter_texture_files[DC_FONT_NUMBER][DC_LETTER_NUMBER];
  static const char letter_mapping[DC_LETTER_NUMBER];
  static const int letter_widths[DC_FONT_NUMBER][DC_LETTER_NUMBER];
  static const float colors[DC_FONT_COLOR_NUMBER][3];
  static GLubyte *letter_textures[DC_FONT_NUMBER][DC_LETTER_NUMBER];
};

#endif
