/*
 * level_lights.cxx
 * Daniel Nelson - 10/13/0
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
 * Generates the list for the level lights.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"

// level lights' geometric constants
#define LG_A                                (0.96f)
#define LG_B                                (0.36f)
#define LG_C                                (0.3f)
#define LG_D                                (0.72f)

GLuint Displayer::level_light_list;

void Displayer::generateLevelLights (   )
{
  level_light_list = glGenLists(1);

  glNewList(level_light_list, GL_COMPILE);
    glBegin(GL_TRIANGLES);

      drawTriangle(0.0f, 0.0f, 0.0f + DC_EXTERNAL_OFFSET_Z,
       LG_D, 0.0f, LG_C + DC_EXTERNAL_OFFSET_Z,
       LG_A, LG_B, 0.0f + DC_EXTERNAL_OFFSET_Z);

      drawTriangle(LG_A, LG_B, 0.0f + DC_EXTERNAL_OFFSET_Z,
       LG_D, 0.0f, LG_C + DC_EXTERNAL_OFFSET_Z,
       LG_A, -LG_B, 0.0f + DC_EXTERNAL_OFFSET_Z);

      drawTriangle(0.0f, 0.0f, 0.0f + DC_EXTERNAL_OFFSET_Z,
       LG_A, -LG_B, 0.0f + DC_EXTERNAL_OFFSET_Z,
       LG_D, 0.0f, LG_C + DC_EXTERNAL_OFFSET_Z);

    glEnd();
  glEndList();
}
