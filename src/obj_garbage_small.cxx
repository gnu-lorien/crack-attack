/*
 * garbage_small.cxx
 * Daniel Nelson - 11/12/0
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
 * Generates a display list for the cute li'l purple garbage blocks.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"

GLuint Displayer::garbage_small_list;

void Displayer::generateGarbageSmallList (   )
{
  garbage_small_list = glGenLists(1);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glNewList(garbage_small_list, GL_COMPILE);

    GLfloat vertices_0[3 * 18] = {
      -0.8f, -0.8f, -1.0f,
      0.8f, -0.8f, -1.0f,
      -0.8f, -1.0f, -0.8f,
      0.8f, -1.0f, -0.8f,
      -0.8f, -1.0f, 0.8f,
      0.8f, -1.0f, 0.8f,
      -0.8f, -0.8f, 1.0f,
      0.8f, -0.8f, 1.0f,
      -0.8f, 0.8f, 1.0f,
      0.8f, 0.8f, 1.0f,
      -0.8f, 1.0f, 0.8f,
      0.8f, 1.0f, 0.8f,
      -0.8f, 1.0f, -0.8f,
      0.8f, 1.0f, -0.8f,
      -0.8f, 0.8f, -1.0f,
      0.8f, 0.8f, -1.0f,
      -0.8f, -0.8f, -1.0f,
      0.8f, -0.8f, -1.0f
    };

    GLfloat vertices_1[3 * 8] = {
      -0.8f, -0.8f, -1.0f,
      -1.0f, -0.8f, -0.8f,
      -0.8f, 0.8f, -1.0f,
      -1.0f, 0.8f, -0.8f,
      -0.8f, 1.0f, -0.8f,
      -1.0f, 0.8f, 0.8f,
      -0.8f, 1.0f, 0.8f,
      -0.8f, 0.8f, 1.0f
    };

    GLfloat vertices_2[3 * 8] = {
      -0.8f, 0.8f, 1.0f,
      -1.0f, 0.8f, 0.8f,
      -0.8f, -0.8f, 1.0f,
      -1.0f, -0.8f, 0.8f,
      -0.8f, -1.0f, 0.8f,
      -1.0f, -0.8f, -0.8f,
      -0.8f, -1.0f, -0.8f,
      -0.8f, -0.8f, -1.0f
    };

    GLfloat vertices_3[3 * 4] = {
      -1.0f, 0.8f, 0.8f,
      -1.0f, 0.8f, -0.8f,
      -1.0f, -0.8f, 0.8f,
      -1.0f, -0.8f, -0.8f
    };

    GLfloat vertices_4[3 * 4] = {
      1.0f, 0.8f, 0.8f,
      1.0f, -0.8f, 0.8f,
      1.0f, 0.8f, -0.8f,
      1.0f, -0.8f, -0.8f
    };

    GLfloat vertices_5[3 * 8] = {
      0.8f, 1.0f, 0.8f,
      1.0f, 0.8f, 0.8f,
      0.8f, 1.0f, -0.8f,
      1.0f, 0.8f, -0.8f,
      0.8f, 0.8f, -1.0f,
      1.0f, -0.8f, -0.8f,
      0.8f, -0.8f, -1.0f,
      0.8f, -1.0f, -0.8f,
    };

    GLfloat vertices_6[3 * 8] = {
      0.8f, -1.0f, -0.8f,
      1.0f, -0.8f, -0.8f,
      0.8f, -1.0f, 0.8f,
      1.0f, -0.8f, 0.8f,
      0.8f, -0.8f, 1.0f,
      1.0f, 0.8f, 0.8f,
      0.8f, 0.8f, 1.0f,
      0.8f, 1.0f, 0.8f,
    };

    GLfloat normals_0[3 * 18] = {
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f, 
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f
    };

    GLfloat normals_1[3 * 8] = {
      0.0f, 0.0f, -1.0f,
      -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      -1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
    };

    GLfloat normals_2[3 * 8] = {
      0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 0.0f, -1.0f
    };

    GLfloat normals_3[3 * 4] = {
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f
    };

    GLfloat normals_4[3 * 4] = {
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f
    };

    GLfloat normals_5[3 * 8] = {
      0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      1.0f, 0.0f, 0.0f,
    };

    GLfloat normals_6[3 * 8] = {
      0.0f, -1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f,
    };

    GLfloat tex_coords_0[3 * 18];
    for (int n = 3 * 18; n--; )
      tex_coords_0[n] = vertices_0[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    GLfloat tex_coords_1[3 * 8];
    for (int n = 3 * 8; n--; )
      tex_coords_1[n] = vertices_1[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    GLfloat tex_coords_2[3 * 8];
    for (int n = 3 * 8; n--; )
      tex_coords_2[n] = vertices_2[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    GLfloat tex_coords_3[3 * 4];
    for (int n = 3 * 4; n--; )
      tex_coords_3[n] = vertices_3[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    GLfloat tex_coords_4[3 * 4];
    for (int n = 3 * 4; n--; )
      tex_coords_4[n] = vertices_4[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    GLfloat tex_coords_5[3 * 8];
    for (int n = 3 * 8; n--; )
      tex_coords_5[n] = vertices_5[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    GLfloat tex_coords_6[3 * 8];
    for (int n = 3 * 8; n--; )
      tex_coords_6[n] = vertices_6[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    glVertexPointer(3, GL_FLOAT, 0, vertices_0);
    glNormalPointer(GL_FLOAT, 0, normals_0);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);

    glVertexPointer(3, GL_FLOAT, 0, vertices_1);
    glNormalPointer(GL_FLOAT, 0, normals_1);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

    glVertexPointer(3, GL_FLOAT, 0, vertices_2);
    glNormalPointer(GL_FLOAT, 0, normals_2);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

    glVertexPointer(3, GL_FLOAT, 0, vertices_3);
    glNormalPointer(GL_FLOAT, 0, normals_3);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glVertexPointer(3, GL_FLOAT, 0, vertices_4);
    glNormalPointer(GL_FLOAT, 0, normals_4);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_4);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glVertexPointer(3, GL_FLOAT, 0, vertices_5);
    glNormalPointer(GL_FLOAT, 0, normals_5);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_5);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

    glVertexPointer(3, GL_FLOAT, 0, vertices_6);
    glNormalPointer(GL_FLOAT, 0, normals_6);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_6);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

  glEndList();

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
};
