/*
 * GarbageFlavorImage.cxx
 * Daniel Nelson - 10/7/0
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
 * Handles the the garbage flavor image.
 */

#include <iomanip>

#ifndef _WIN32
#  include <sstream>
#else
#  include <strstrea.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "GarbageFlavorImage.h"
#include "TextureLoader.h"
#include "Random.h"

int GarbageFlavorImage::current_texture;
int GarbageFlavorImage::associated_garbage_id;
int GarbageFlavorImage::x, GarbageFlavorImage::y;
bool GarbageFlavorImage::network_texture = false;

void GarbageFlavorImage::initialize (   )
{
  // network garbage flavor image stuff is setup before initialization
  current_texture = 0;
  associated_garbage_id = -1;

  // insure existence of default files
  char file_name[256];
  for (int n = GC_NUMBER_STANDARD_GARBAGE_TEX; n--; ) {
    GarbageFlavorImage::buildGarbageTextureFileName(file_name, n);

    if (!TextureLoader::fileExists(file_name)) {
      char original_file_name[256];
      GarbageFlavorImage::buildOriginalGarbageTextureFileName(
       original_file_name, n);

      GLubyte *texture = TextureLoader::loadTGA(original_file_name,
       DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);
      TextureLoader::createTGA(file_name, texture, DC_GARBAGE_TEX_LENGTH,
       DC_GARBAGE_TEX_LENGTH, TL_GARBAGE_TEXTURE_TGA_ID);
      if (texture != null) {
        delete [] texture;
        texture = null;
      }
    }
  }
}

bool GarbageFlavorImage::personalGarbageFlavorImageExists (   )
/*
 * Called by Communicator or obj_garbage.cxx at startup to determine if we have
 * a personal garbage flavor image to send.
 */
{
  char file_name[256];
  TextureLoader::buildLocalDataFileName(GC_GARBAGE_MY_TEX_FILE_NAME, file_name);
  return TextureLoader::fileExists(file_name);
}

GLubyte *GarbageFlavorImage::loadPersonalGarbageFlavorImage (   )
/*
 * Called by Communicator at startup to obtain the personal garbage flavor
 * image or by obj_garbage.cxx if in solo mode.
 */
{
  char file_name[256];
  TextureLoader::buildLocalDataFileName(GC_GARBAGE_MY_TEX_FILE_NAME, file_name);

  int width, height;
  TextureLoader::determineTGASize(file_name, width, height);

  if (width > DC_GARBAGE_TEX_LENGTH || height > DC_GARBAGE_TEX_LENGTH) {
    cerr << "Texture file '" << file_name << "' exceeds allowed " "size of [" <<
     DC_GARBAGE_TEX_LENGTH << 'x' << DC_GARBAGE_TEX_LENGTH << "]." << endl;
    exit(1);
  }
  
  GLubyte *original_texture = TextureLoader::loadTGA(file_name, width, height);

  if (width == DC_GARBAGE_TEX_LENGTH && height == DC_GARBAGE_TEX_LENGTH)
    return original_texture;

  GLubyte *texture
   = new GLubyte[DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH * 4];

  for (int t = DC_GARBAGE_TEX_LENGTH; t--; )
    for (int s = DC_GARBAGE_TEX_LENGTH; s--; ) {
      if (s + height > DC_GARBAGE_TEX_LENGTH
       && t + width > DC_GARBAGE_TEX_LENGTH) {
        int texel = ((t - DC_GARBAGE_TEX_LENGTH + width) * height
         + (s - DC_GARBAGE_TEX_LENGTH + height)) * 4;
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 0]
         = original_texture[texel + 0];
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 1]
         = original_texture[texel + 1];
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 2]
         = original_texture[texel + 2];
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 3]
         = original_texture[texel + 3];
      } else {
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 0] = 0;
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 1] = 0;
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 2] = 0;
        texture[(t * DC_GARBAGE_TEX_LENGTH + s) * 4 + 3] = 0;
      }
    }

  if (original_texture != null) {
    delete [] original_texture;
    original_texture = null;
  }

  return texture;
}

void GarbageFlavorImage::handleNetworkGarbageFlavorImage ( GLubyte *texture )
/*
 * Called by Communicator at startup if opponent sends us his personal garbage
 * flavor image.
 */
{
  char net_tex_file_name[256];
  TextureLoader::buildLocalDataFileName(GC_GARBAGE_NET_TEX_FILE_NAME,
   net_tex_file_name);

  network_texture = true;

  // create the network texture file
  TextureLoader::createTGA(net_tex_file_name, texture, DC_GARBAGE_TEX_LENGTH,
   DC_GARBAGE_TEX_LENGTH, TL_GARBAGE_TEXTURE_TGA_ID);

  // determine it's check sum
  unsigned long check_sum
   = TextureLoader::determineTGACheckSum(net_tex_file_name,
   DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH);

  // check to see if we already have a copy of it in our main list
  int n;
  int first_open_slot = -1;
  char file_name[256];
  for (n = GC_GARBAGE_TEX_MAX_NUMBER; n--; ) {
    GarbageFlavorImage::buildGarbageTextureFileName(file_name, n);

    if (TextureLoader::fileExists(file_name)) {
      if (check_sum == TextureLoader::determineTGACheckSum(file_name,
       DC_GARBAGE_TEX_LENGTH, DC_GARBAGE_TEX_LENGTH))
        break;
    } else
      first_open_slot = n;
  }

  // if we don't already have a copy, create one
  if (n == -1 && first_open_slot != -1) {
    GarbageFlavorImage::buildGarbageTextureFileName(file_name, first_open_slot);
    TextureLoader::createTGA(file_name, texture, DC_GARBAGE_TEX_LENGTH,
     DC_GARBAGE_TEX_LENGTH, TL_GARBAGE_TEXTURE_TGA_ID);
  }
}

void GarbageFlavorImage::buildGarbageTextureFileName ( char file_name[256],
 const char *dir_name, int n )
{
  ostringstream s;
  s << dir_name << GC_GARBAGE_TEX_FILE_NAME_BASE "_"
   << setw(GC_GARBAGE_TEX_NUMBER_DIGITS) << setfill('0') << n << ".tga" << ends;
  strncpy(file_name, s.str().data(), 256);
}

void GarbageFlavorImage::buildGarbageTextureFileName ( char file_name[256],
 int n )
{
  char base_name[256];
  buildGarbageTextureFileName(base_name, "", n);
  TextureLoader::buildLocalDataFileName(base_name, file_name);
}

void GarbageFlavorImage::buildOriginalGarbageTextureFileName (
 char file_name[256], int n )
{
  buildGarbageTextureFileName(file_name, GC_DATA_DIRECTORY, n);
}

void GarbageFlavorImage::requestGarbageFlavorImage_inline_split_ (
 Garbage &garbage )
/*
 * Request to use the garbage flavor image for this piece of garbage.  If the
 * garbage flavor image is free, it will be set to the given garbage.  The
 * height and width of the garbage must be four or more.
 */
{
  associated_garbage_id = garbage.id;

  x = 1 + Random::number(garbage.width - 3);

  if (Random::number2(4))
    y = 1;
  else
    y = 1 + Random::number(garbage.height - 3);

  int last_texture = current_texture;
  if (network_texture)
    current_texture
     = (Random::number(DC_CHANCE_USE_NET_TEX) ? 0 : 1 + Random::number(3));
  else
    current_texture = Random::number2(4);

  if (last_texture != current_texture) {
    glBindTexture(GL_TEXTURE_2D, Displayer::garbage_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DC_GARBAGE_TEX_LENGTH,
     DC_GARBAGE_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE,
     Displayer::garbage_texture_data[current_texture]);
  }
}
