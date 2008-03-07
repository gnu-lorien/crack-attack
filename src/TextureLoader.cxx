/*
 * TextureLoader.cxx
 * Daniel Nelson - 9/14/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * Copyright (C) 2004  Andrew Sayman
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
 * Loads textures from uncompressed TGA files.
 */

#include "TextureLoader.h"

#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <SDL/SDL_image.h>

#ifndef _WIN32
#  include <unistd.h>
#endif

#include "glext.h"

#include "Game.h"

using namespace std;

// the header of an uncompressed TGA file
const GLubyte header_image[11] = { 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

GLubyte *TextureLoader::loadImageAlpha ( const char *file_name, int _height,
 int _width )
{
  GLubyte *full_texture = loadImage(file_name, _height, _width);

  int texture_size = _width * _height;

  GLubyte *alpha_texture = new GLubyte[texture_size];

  for (int n = 0; n < texture_size; n++)
    alpha_texture[n] = full_texture[4 * n + 3];

  if (full_texture != null) {
    delete [] full_texture;
    full_texture = null;
  }

  return alpha_texture;
}

GLubyte *TextureLoader::loadImageNoAlpha ( const char *file_name,
    int _height, int _width )
{
  GLubyte *full_texture = loadImage(file_name, _height, _width);

  int texture_size = _width * _height;

  GLubyte *no_alpha_texture = new GLubyte[texture_size * 3];

  for (int n = 0; n < texture_size; n++) {
    float alpha = full_texture[4 * n + 3] * (1.0f / 255.0f);
    no_alpha_texture[3 * n + 0] = (GLubyte) (alpha * full_texture[4 * n + 0]);
    no_alpha_texture[3 * n + 1] = (GLubyte) (alpha * full_texture[4 * n + 1]);
    no_alpha_texture[3 * n + 2] = (GLubyte) (alpha * full_texture[4 * n + 2]);
  }

  if (full_texture != null) {
    delete [] full_texture;
    full_texture = null;
  }

  return no_alpha_texture;
}

GLubyte *TextureLoader::loadImage ( const char *file_name, int _height,
    int _width)
{
  SDL_Surface *img = IMG_Load(file_name);

  if (!img) {
    cerr << "Error opening texture file '" << file_name << "'." << endl;
    exit(1);
  }

  int width = img->w;
  int height = img->h;

  if (width != _width || height != _height) {
    cerr << "Texture file '" << file_name << "' does not have the expected "
     "height and width.  [" << height << 'x' << width << "] vs expected ["
     << _height << 'x' << _width << "]." << endl;
    exit(1);
  }

  // Convert the loaded image to a standard 32 bit format

  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  SDL_Surface *formatImg = SDL_CreateRGBSurface(
      SDL_SWSURFACE, 0, 0, 32, rmask, gmask, bmask, amask);

  if (formatImg == NULL) {
    cerr << "Out of memory while opening texture file '" << file_name
      << "'." << endl;
    SDL_FreeSurface(img);
    exit(1);
  }

  SDL_Surface *tmp = SDL_ConvertSurface(img, formatImg->format, SDL_SWSURFACE);

  SDL_FreeSurface(img);
  SDL_FreeSurface(formatImg);

  // TODO: This memcpy is rather pointless. Maybe find a way to avoid it.

  int texture_size = width * height * 32 / 8;
  GLubyte *texture = new GLubyte[texture_size];
  memcpy(texture, tmp->pixels, texture_size);

  SDL_FreeSurface(tmp);

  //cout << "Succesfully loaded '" << file_name << "'." << endl;

  return texture;
}

void TextureLoader::createTGA ( const char *tga_file_name, GLubyte *texture,
 int _height, int _width, const char *tga_id )
{
#ifndef _WIN32
  ofstream file(tga_file_name);
#else
  ofstream file(tga_file_name, ios::binary);
#endif
  if (file.fail()) {
    cerr << "Error creating texture file '" << tga_file_name << "'." << endl;
    exit(1);
  }

  GLubyte tga_id_length = strlen(tga_id);
  file.write((char *) &tga_id_length, 1);

  file.write((char *) header_image, sizeof(header_image));

  GLubyte header[6];
  header[1] = _width / 256;
  header[0] = _width - 256 * header[1];
  header[3] = _height / 256;
  header[2] = _height - 256 * header[3];
  header[4] = 32;
  header[5] = 40;
  file.write((char *) header, sizeof(header));

  file.write(tga_id, tga_id_length);

  // tga is BGR
  for (int n = 0; n < _width * _height * 4; n += 4) {
    GLubyte swap = texture[n];
    texture[n] = texture[n + 2];
    texture[n + 2] = swap;
  }

  file.write((char *) texture, _width * _height * 4);

  for (int n = 0; n < _width * _height * 4; n += 4) {
    GLubyte swap = texture[n];
    texture[n] = texture[n + 2];
    texture[n + 2] = swap;
  }
}

bool TextureLoader::fileExists ( const char *file_name )
{
  struct stat file_stats;

  // stat fails to find directories with trailing delimiters in _WIN32
#ifdef _WIN32
  if (file_name[strlen(file_name) - 1] == GC_DD[0]) {
    char truncated_file_name[256];
    strncpy(truncated_file_name, file_name, 256);
    truncated_file_name[strlen(file_name) - 1] = '\0';
    return !stat(truncated_file_name, &file_stats);
  }
#endif
  return !stat(file_name, &file_stats);
}

unsigned long TextureLoader::determineImageCheckSum ( const char *file_name,
 int _height, int _width )
{
  GLubyte *texture = loadImage(file_name, _height, _width);

  unsigned long check_sum = 0;
  for (int n = _width * _height * 4; n--; )
    check_sum += texture[n];

  if (texture != null) {
    delete [] texture;
    texture = null;
  }

  return check_sum;
}

void TextureLoader::determineImageSize ( const char *file_name, int &height,
    int &width )
{
  SDL_Surface *img = IMG_Load(file_name);

  if (!img) {
    cerr << "Error opening texture file '" << file_name << "'." << endl;
    exit(1);
  }

  width = img->w;
  height = img->h;

  SDL_FreeSurface(img);
}
