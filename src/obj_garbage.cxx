/*
 * garbage.cxx
 * Daniel Nelson - 10/1/0
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
 * Generates the lightmap and flavor textures for the garbage.
 */

#define GL_GLEXT_LEGACY

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "TextureLoader.h"
#include "GarbageFlavorImage.h"
#include "MetaState.h"
#include "Random.h"

const char *garbage_lightmap_files[DC_NUMBER_GARBAGE_LIGHTMAPS]
 = { GC_DATA_DIRECTORY "garbage_lightmap_0.tga",
     GC_DATA_DIRECTORY "garbage_lightmap_1.tga",
     GC_DATA_DIRECTORY "garbage_lightmap_2.tga",
     GC_DATA_DIRECTORY "garbage_lightmap_3.tga",
     GC_DATA_DIRECTORY "garbage_lightmap_4.tga",
     GC_DATA_DIRECTORY "garbage_lightmap_5.tga" };

GLuint Displayer::garbage_lightmap;
GLuint Displayer::garbage_texture;

GLuint Displayer::garbage_flavor_list;

GLubyte *Displayer::garbage_texture_data[DC_NUMBER_USE_GARBAGE_TEX];

void Displayer::generateGarbageExtras (   )
{
  char file_name[256];

  glGenTextures(1, &garbage_lightmap);

  glBindTexture(GL_TEXTURE_2D, garbage_lightmap);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // choose a random lightmap for this game
  GLubyte *texture = TextureLoader::loadAlphaTGA(
   garbage_lightmap_files[Random::number(DC_NUMBER_GARBAGE_LIGHTMAPS)],
   DC_GARBAGE_LIGHTMAP_LENGTH, DC_GARBAGE_LIGHTMAP_LENGTH);

  for (int s = DC_GARBAGE_LIGHTMAP_LENGTH; s--; )
    for (int t = DC_GARBAGE_LIGHTMAP_LENGTH; t--; )
      texture[s * DC_GARBAGE_LIGHTMAP_LENGTH + t]
       = (GLubyte) (255.0f * (DC_GARBAGE_LIGHTMAP_MIN_LUMINANCE
       + ((1.0f - DC_GARBAGE_LIGHTMAP_MIN_LUMINANCE) / 255.0f)
       * (float) texture[s * DC_GARBAGE_LIGHTMAP_LENGTH + t]));

  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, DC_GARBAGE_LIGHTMAP_LENGTH,
   DC_GARBAGE_LIGHTMAP_LENGTH, GL_FALSE, GL_LUMINANCE, GL_UNSIGNED_BYTE,
   texture);

  if (texture != null) {
    delete [] texture;
    texture = null;
  }

  glGenTextures(1, &garbage_texture);

  glBindTexture(GL_TEXTURE_2D, garbage_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // identify which texture files exist
  int texture_file_count = 0;
  bool texture_file_flag[GC_GARBAGE_TEX_MAX_NUMBER];
  for (int n = GC_GARBAGE_TEX_MAX_NUMBER; n--; ) {
    GarbageFlavorImage::buildGarbageTextureFileName(file_name, n);

    if (TextureLoader::fileExists(file_name)) {
      texture_file_count++;
      texture_file_flag[n] = true;
    } else
      texture_file_flag[n] = false;
  }

  // if for some reason we don't have enough textures
  if (texture_file_count < DC_NUMBER_USE_GARBAGE_TEX) {
    GarbageFlavorImage::buildGarbageTextureFileName(file_name, 0);

    for (int n = 0; n < DC_NUMBER_USE_GARBAGE_TEX; n++) {
      garbage_texture_data[n] = TextureLoader::loadTGA(file_name,
       DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);
    }

  // otherwise, we have plenty
  } else {
    int use_textures[DC_NUMBER_USE_GARBAGE_TEX];
    for (int n = 0; n < DC_NUMBER_USE_GARBAGE_TEX; n++) {

      // always put the net tex in zero, if there is one
      if (n == 0 && (MetaState::mode & CM_SOLO)
       && GarbageFlavorImage::personalGarbageFlavorImageExists())
        GarbageFlavorImage::handleNetworkGarbageFlavorImage(
         garbage_texture_data[0]
         = GarbageFlavorImage::loadPersonalGarbageFlavorImage());

      else if (n == 0 && !(MetaState::mode & CM_SOLO)
       && GarbageFlavorImage::network_texture) {
        char net_tex_file_name[256];
        TextureLoader::buildLocalDataFileName(GC_GARBAGE_NET_TEX_FILE_NAME,
         net_tex_file_name);

        garbage_texture_data[0] = TextureLoader::loadTGA(net_tex_file_name,
         DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);

      } else {
        bool flag;
        do {
          flag = false;

          int i;
          int choice = Random::number(texture_file_count);
          for (i = 0; choice--; i++)
            while (!texture_file_flag[i])
              i++;
          use_textures[n] = i;

          for (int m = 0; m < n; m++)
            if (use_textures[n] == use_textures[m]) {
              flag = true;
              break;
            }
        } while (flag);

        GarbageFlavorImage::buildGarbageTextureFileName(file_name,
         use_textures[n]);

        garbage_texture_data[n] = TextureLoader::loadTGA(file_name,
         DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);
      }
    }
  }

  // the logo may replace the second texture
  if (Random::chanceIn(DC_CHANCE_IN_NOT_USE_LOGO_FLAVOR_TEX)) {
    if (garbage_texture_data[1] != null) {
      delete [] garbage_texture_data[1];
      garbage_texture_data[1] = null;
    }

    if (MetaState::mode & CM_X)
      garbage_texture_data[1]
       = TextureLoader::loadTGA(GC_GARBAGE_X_LOGO_TEX_FILE_NAME,
       DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);
    else
      garbage_texture_data[1]
       = TextureLoader::loadTGA(GC_GARBAGE_LOGO_TEX_FILE_NAME,
       DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DC_GARBAGE_TEX_LENGTH,
   DC_GARBAGE_TEX_LENGTH, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE,
   garbage_texture_data[GarbageFlavorImage::current_texture]);

  GLfloat vertices_0[3 * 6] = {
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 3.0f, 1.0f,
    1.0f, 3.0f, 1.0f
  };

  GLfloat normals_0[3 * 6] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
  };

  GLfloat tex_coords_flavor[2 * 6] = {
    0.0f, 1.0f,
    0.5f, 1.0f,
    0.0f, 0.5f,
    0.5f, 0.5f,
    0.0f, 0.0f,
    0.5f, 0.0f
  };

#ifndef NO_MULTITEXTURING
  GLfloat tex_coords_lightmap[2 * 6] = {
    -DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, -DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
    DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, -DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
    -DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
    DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
    -DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
     3.0f * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
    DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
     3.0f * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
  };
#endif

  garbage_flavor_list = glGenLists(1);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

#ifndef NO_MULTITEXTURING
  // if multitexturing
  if (Displayer::state & DS_MULTITEXTURING) {

    glActiveTextureARB(GL_TEXTURE1_ARB);
    
    glBindTexture(GL_TEXTURE_2D, garbage_lightmap);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glActiveTextureARB(GL_TEXTURE0_ARB);

    glNewList(garbage_flavor_list, GL_COMPILE);

      glVertexPointer(3, GL_FLOAT, 0, vertices_0);
      glNormalPointer(GL_FLOAT, 0, normals_0);
      glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_flavor);

      glClientActiveTextureARB(GL_TEXTURE1_ARB);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_lightmap);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

      for (int n = 6; n--; ) {
        vertices_0[3 * n + 0] += 2.0f;
        tex_coords_flavor[2 * n + 0] += 0.5f;
        tex_coords_lightmap[2 * n + 0]
         += 2.0f * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;
      }

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glClientActiveTextureARB(GL_TEXTURE0_ARB);

    glEndList();

  // if no multitexturing
  } else {
#endif

    glNewList(garbage_flavor_list, GL_COMPILE);

      glVertexPointer(3, GL_FLOAT, 0, vertices_0);
      glNormalPointer(GL_FLOAT, 0, normals_0);
      glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_flavor);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

      for (int n = 6; n--; ) {
        vertices_0[3 * n + 0] += 2.0f;
        tex_coords_flavor[2 * n + 0] += 0.5f;
      }

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
  
    glEndList();
#ifndef NO_MULTITEXTURING
  }
#endif

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
};
