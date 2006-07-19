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
 * $Id: Primitives.cxx,v 1.1 2006/07/19 01:56:21 lorien420 Exp $
 */

#include "Primitives.h"


Vector::Vector()
{
  x = y = z = 0.0f;
}

Vertex::Vertex()
{
  x = y = z = 0.0f;
  w = 1.0f;
}

VertexTexture::VertexTexture()
{
  u = v = w = 0.0f;
}

VertexNormal::VertexNormal()
{
  i = j = k = 0.0f;
}

VertexColor::VertexColor()
{
  r = g = b = a = 1.0f;
}

Material::Material()
{
  name = "untitled";
  int i;
  for (i = 0; i < 3; i++) {
    ambient[i] = 0.2f;
    specular[i] = 0.0f;
    diffuse[i] = 0.8f;
  }
  ambient[3] = 1.0f;
  specular[3] = 1.0f;
  diffuse[3] = 1.0f;
  illuminationModel = 1;
  transparency = 0.0f;
  shininess = 0.0f;
  textureId = 0;
}

FaceVertex::FaceVertex()
{
  v = vn = vt = vc = -1;
}
