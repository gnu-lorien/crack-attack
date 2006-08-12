/*
 * block.cxx
 * Daniel Nelson - 8/31/0
 *
 * Copyright (C) 2006  Bjørn Lindeijer
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
 * Generates the block's two display lists.
 */

#include <GL/glut.h>

#include "glext.h"

#include <cstring>

#include "Game.h"
#include "Displayer.h"
#include "OBJModel.h"

GLuint Displayer::block_list;
GLuint Displayer::small_block_list;
GLuint Displayer::special_block_list;

GLuint Displayer::special_block_lightmap;

void Displayer::generateBlockDisplayList (   )
{
  OBJModel *block_model = NULL;
  OBJModel *block_model_tex = NULL;

  if (MetaState::mode & CM_LOW_GRAPHICS) {
    block_model = new OBJModel(GC_MODEL_DIRECTORY("crackattackcubemedres.obj"));
    block_model_tex = new OBJModel(GC_MODEL_DIRECTORY("crackattackcubemedres_tex.obj"));
  } else {
    block_model = new OBJModel(GC_MODEL_DIRECTORY("crackattackcubehires.obj"));
    block_model_tex = new OBJModel(GC_MODEL_DIRECTORY("crackattackcubehires_tex.obj"));
    //block_model = new OBJModel(GC_MODEL_DIRECTORY("cube.obj"));
    //block_model_tex = new OBJModel(GC_MODEL_DIRECTORY("cube.obj"));
  }
  block_list = glGenLists(1);
  glNewList(block_list, GL_COMPILE);
    block_model->render();
  glEndList();

  // Now let's make one for pulsing blocks (has texture coordinates)

  special_block_list = glGenLists(1);
  glNewList(special_block_list, GL_COMPILE);
    block_model_tex->render();
  glEndList();

  delete block_model;
  delete block_model_tex;

  // Now let's make one at half that size and rotated

  small_block_list = glGenLists(1);
  glNewList(small_block_list, GL_COMPILE);
    glPushMatrix();
      glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
      glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
      glScalef(0.5f, 0.5f, 0.5f);
      glEnable(GL_NORMALIZE);
      glCallList(block_list);
      glDisable(GL_NORMALIZE);
    glPopMatrix();
  glEndList();

  // Now we build the special color blocks' gleam texture.  A texture value of
  // 0.6f seems to cause the special color blocks to match their standard
  // siblings, given the specials' double color values.

  GLubyte texture[32];
  for (int n = 32; n--; )
    texture[n]
     = (GLubyte) (255.0f * (0.8f - 0.2f * cos((2.0f * PI / 32.0f) * n)));

  glGenTextures(1, &special_block_lightmap);

  glBindTexture(GL_TEXTURE_1D, special_block_lightmap);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  const GLubyte* renderer = glGetString(GL_RENDERER);
  if (strstr((char*) renderer, "DRI 20020221 Voodoo3")) {
    std::cerr << "**********\nWARNING:"
      "disabling call to 1d texturing on DRI 20020221 Voodoo3 renderer "
      "since it segfaults\n**********" << std::endl;
    return;
  }
  if (strstr((char*) renderer, "865G 20021115")) {
    std::cerr << "**********\nWARNING:"
      "disabling call to 1d texturing on 865G 20021115 renderer "
      "since it segfaults\n**********" << std::endl;
    return;
  }

  glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE, 32, GL_FALSE, GL_LUMINANCE,
   GL_UNSIGNED_BYTE, texture);
}
