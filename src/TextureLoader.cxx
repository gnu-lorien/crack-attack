/*
 * TextureLoader.cxx
 * Daniel Nelson - 9/14/0
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
 * Loads textures from uncompressed TGA files.
 */

#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sys/stat.h>

#ifndef _WIN32
#  include <unistd.h>
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "TextureLoader.h"

// the header of an uncompressed TGA file
const GLubyte header_image[11] = { 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

GLubyte *TextureLoader::loadAlphaTGA ( const char *tga_file_name, int _height,
 int _width )
{
  GLubyte *full_texture = loadTGA(tga_file_name, _height, _width);

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

GLubyte *TextureLoader::loadNoAlphaTGA ( const char *tga_file_name, int _height,
 int _width )
{
  GLubyte *full_texture = loadTGA(tga_file_name, _height, _width);

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

GLubyte *TextureLoader::loadTGA ( const char *tga_file_name, int _height,
 int _width, int _color_depth )
{
#ifndef _WIN32
  ifstream file(tga_file_name);
#else
  ifstream file(tga_file_name, ios::binary);
#endif
  if (file.fail()) {
    cerr << "Error opening texture file '" << tga_file_name << "'." << endl;
    exit(1);
  }

  GLubyte id_length;
  file.read((char *) &id_length, 1);

  GLubyte static_header[11];
  file.read((char *) static_header, sizeof(static_header));
  if (memcmp(static_header, header_image, sizeof(static_header)) != 0) {
    cerr << "Texture file '" << tga_file_name << "' is not in uncompressed "
     "TGA format." << endl;
    exit(1);
  }

  GLubyte header[6];
  file.read((char *) header, sizeof(header));

  int width = header[1] * 256 + header[0];
  int height = header[3] * 256 + header[2];

  if (width != _width || height != _height) {
    cerr << "Texture file '" << tga_file_name << "' does not have the expected "
     "height and width.  [" << height << 'x' << width << "] vs expected ["
     << _height << 'x' << _width << "]." << endl;
    exit(1);
  }

  int color_depth = header[4];
  if (color_depth != 24 && color_depth != 32) {
    cerr << "Texture file '" << tga_file_name << "' has an unsupported color "
     "depth." << endl;
    exit(1);
  }
  if (_color_depth != 0 && _color_depth != color_depth) {
    cerr << "Texture file '" << tga_file_name << "' does not have the expected "
     "color depth." << endl;
    exit(1);
  }

  GLubyte b;
  while (id_length--)
    file.read((char *) &b, 1);

  int texture_size = width * height * color_depth / 8;

  GLubyte *texture = new GLubyte[texture_size];

  file.read((char *) texture, texture_size);

  // tga is BGR
  for (int n = 0; n < texture_size; n += color_depth / 8) {
    GLubyte swap = texture[n];
    texture[n] = texture[n + 2];
    texture[n + 2] = swap;
  }

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

  file.write((char *) tga_id, tga_id_length);

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

#ifndef _WIN32
#else // stat fails to find directories with trailing delimiters in _WIN32
  if (file_name[strlen(file_name) - 1] == GC_DD[0]) {
    char truncated_file_name[256];
    strncpy(truncated_file_name, file_name, 256);
    truncated_file_name[strlen(file_name) - 1] = '\0';
    return !stat(truncated_file_name, &file_stats);
  }
#endif
  return !stat(file_name, &file_stats);
}

unsigned long TextureLoader::determineTGACheckSum ( const char *tga_file_name,
 int _height, int _width )
{
  GLubyte *texture = loadTGA(tga_file_name, _height, _width);

  unsigned long check_sum = 0;
  for (int n = _width * _height * 4; n--; )
    check_sum += texture[n];

  if (texture != null) {
    delete [] texture;
    texture = null;
  }

  return check_sum;
}

void TextureLoader::determineTGASize ( const char *tga_file_name, int &height,
 int &width )
{
#ifndef _WIN32
  ifstream file(tga_file_name);
#else
  ifstream file(tga_file_name, ios::binary);
#endif
  if (file.fail()) {
    cerr << "Error opening texture file '" << tga_file_name << "'." << endl;
    exit(1);
  }

  GLubyte id_length;
  file.read((char *) &id_length, 1);

  GLubyte static_header[11];
  file.read((char *) static_header, sizeof(static_header));
  if (memcmp(static_header, header_image, sizeof(static_header)) != 0) {
    cerr << "Texture file '" << tga_file_name << "' is not in uncompressed "
     "TGA format." << endl;
    exit(1);
  }

  GLubyte header[6];
  file.read((char *) header, sizeof(header));

  width = header[1] * 256 + header[0];
  height = header[3] * 256 + header[2];
}
