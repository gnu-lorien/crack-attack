/*
 * swapper.c_xx
 * Daniel Nelson 9/22/0
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
 * Generates the swapper's displa_y list.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"

// swapper's geometric constants
#define SG_A                          (0.1f)
#define SG_B                          (2.0f * SG_A / 2.414214f)
#define SG_C                          (0.8f)
#define SG_D                          (0.1f)
#define SG_E                          (1.0f)
#define SG_F                          (0.4f)
#define SG_H                          (1.0f)
#define SG_I                          (0.1f)
#define SG_J                          (0.16f)
#define SG_S                          (1.0f)

GLuint Displayer::swapper_list;

void Displayer::drawTriangle ( float a_x, float a_y, float a_z,
 float b_x, float b_y, float b_z, float c_x, float c_y, float c_z )
{
  float n_x = (c_y - a_y) * (a_z - b_z) - (a_y - b_y) * (c_z - a_z);
  float n_y = (c_z - a_z) * (a_x - b_x) - (a_z - b_z) * (c_x - a_x);
  float n_z = (c_x - a_x) * (a_y - b_y) - (a_x - b_x) * (c_y - a_y);

  float norm = sqrt(n_x * n_x + n_y * n_y + n_z * n_z);
  n_x /= norm;
  n_y /= norm;
  n_z /= norm;

  glNormal3f(n_x, n_y, n_z);

  glVertex3f(a_x, a_y, a_z);
  glVertex3f(b_x, b_y, b_z);
  glVertex3f(c_x, c_y, c_z);
}

void Displayer::generateSwapperList (   )
{
  swapper_list = glGenLists(1);

  glNewList(swapper_list, GL_COMPILE);

    glBegin(GL_TRIANGLES);

      // not the fastest method, but this is at start up and may change
      // someday; plus, if you have a super cool compiler, it could do it at
      // compile time; plus, if you're going to complain, why don't you buy me
      // a copy of 3D Studio Max

      drawTriangle(SG_S + SG_E - SG_C, -SG_E, SG_H,
        SG_S + SG_E - SG_C + SG_J, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_C + SG_D, -SG_E + SG_A, SG_H);

      drawTriangle(SG_S + SG_E - SG_C + SG_D, -SG_E + SG_A, SG_H,
        SG_S + SG_E - SG_C + SG_J, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_F, -SG_E + SG_A, SG_H);

      drawTriangle(SG_S + SG_E - SG_F, -SG_E + SG_A, SG_H,
        SG_S + SG_E - SG_C + SG_J, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_F + SG_B / 2.0f, -SG_E, SG_H + SG_I);

      drawTriangle(SG_S + SG_E - SG_F, -SG_E + SG_A, SG_H,
        SG_S + SG_E - SG_F + SG_B / 2.0f, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_A, -SG_E + SG_F, SG_H);

      drawTriangle(SG_S + SG_E - SG_A, -SG_E + SG_F, SG_H,
        SG_S + SG_E - SG_F + SG_B / 2.0f, -SG_E, SG_H + SG_I,
        SG_S + SG_E, -SG_E + SG_F - SG_B / 2.0f, SG_H + SG_I);

      drawTriangle(SG_S + SG_E - SG_A, -SG_E + SG_F, SG_H,
        SG_S + SG_E, -SG_E + SG_F - SG_B / 2.0f, SG_H + SG_I,
        SG_S + SG_E - SG_A, -SG_E + SG_C - SG_D, SG_H);

      drawTriangle(SG_S + SG_E - SG_A, -SG_E + SG_C - SG_D, SG_H,
        SG_S + SG_E, -SG_E + SG_F - SG_B / 2.0f, SG_H + SG_I,
        SG_S + SG_E, -SG_E + SG_C - SG_J, SG_H + SG_I);

      drawTriangle(SG_S + SG_E - SG_A, -SG_E + SG_C - SG_D, SG_H,
        SG_S + SG_E, -SG_E + SG_C - SG_J, SG_H + SG_I,
        SG_S + SG_E, -SG_E + SG_C, SG_H);

      drawTriangle(SG_S + SG_E - SG_C, -SG_E, SG_H,
        SG_S + SG_E - SG_C + SG_D, -SG_E - SG_A, SG_H,
        SG_S + SG_E - SG_C + SG_J, -SG_E, SG_H + SG_I);

      drawTriangle(SG_S + SG_E - SG_C + SG_J, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_C + SG_D, -SG_E - SG_A, SG_H,
        SG_S + SG_E - SG_F + SG_B / 2.0f, -SG_E, SG_H + SG_I);

      drawTriangle(SG_S + SG_E - SG_F + SG_B / 2.0f, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_C + SG_D, -SG_E - SG_A, SG_H,
        SG_S + SG_E - SG_F + SG_B, -SG_E - SG_A, SG_H);

      drawTriangle(SG_S + SG_E - SG_F + SG_B / 2.0f, -SG_E, SG_H + SG_I,
        SG_S + SG_E - SG_F + SG_B, -SG_E - SG_A, SG_H,
        SG_S + SG_E, -SG_E + SG_F - SG_B / 2.0f, SG_H + SG_I);

      drawTriangle(SG_S + SG_E, -SG_E + SG_F - SG_B / 2.0f, SG_H + SG_I,
        SG_S + SG_E - SG_F + SG_B, -SG_E - SG_A, SG_H,
        SG_S + SG_E + SG_A, -SG_E + SG_F - SG_B, SG_H);

      drawTriangle(SG_S + SG_E, -SG_E + SG_F - SG_B / 2.0f, SG_H + SG_I,
        SG_S + SG_E + SG_A, -SG_E + SG_F - SG_B, SG_H,
        SG_S + SG_E, -SG_E + SG_C - SG_J, SG_H + SG_I);

      drawTriangle(SG_S + SG_E, -SG_E + SG_C - SG_J, SG_H + SG_I,
        SG_S + SG_E + SG_A, -SG_E + SG_F - SG_B, SG_H,
        SG_S + SG_E + SG_A, -SG_E + SG_C - SG_D, SG_H);

      drawTriangle(SG_S + SG_E, -SG_E + SG_C, SG_H,
        SG_S+ SG_E, -SG_E + SG_C - SG_J, SG_H + SG_I,
        SG_S + SG_E + SG_A, -SG_E + SG_C - SG_D, SG_H);
        
    glEnd();

  glEndList();
}
