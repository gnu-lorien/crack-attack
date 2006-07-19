/*
 * Copyright (C) 2006  Bjørn Lindeijer
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
 * $Id: Primitives.h,v 1.1 2006/07/19 01:56:21 lorien420 Exp $
 */

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/gl.h>
#include <vector>
#include <string>

enum Direction {
  None,
  Left,
  Right,
  Up,
  Down
};

struct Vector
{
  Vector();
  float x, y, z;
};

struct Vertex
{
  Vertex();
  float x, y, z, w;
};

struct VertexTexture
{
  VertexTexture();
  float u, v, w;
};

struct VertexNormal
{
  VertexNormal();
  float i, j, k;
};

struct VertexColor
{
  VertexColor();
  float r, g, b, a;
};

struct FaceVertex
{
  FaceVertex();
  int v, vn, vt, vc;
};

struct Material
{
  Material();
  float ambient[4];
  float specular[4];
  float diffuse[4];
  std::string name;
  float transparency;
  int illuminationModel;
  float shininess;
  GLuint textureId;
};

struct Face
{
  std::vector<FaceVertex> vertices;
  Material *material;
};


#endif
