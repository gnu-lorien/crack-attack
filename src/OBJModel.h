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
 * $Id: OBJModel.h,v 1.1 2006/07/19 01:56:21 lorien420 Exp $
 */

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <map>
#include <string>
#include <vector>
#include "Primitives.h"

/**
 * A class for loading and rendering OBJ files.
 */
class OBJModel
{
  public:
    OBJModel() { };
    OBJModel(const std::string& filename);
    virtual ~OBJModel();

    virtual void render();
    virtual void load(const std::string& filename);

  private:
    virtual void loadMTL(const std::string& filename);

    std::string name;
    std::string shadowObj;
    std::vector<Vertex> vertices;
    std::vector<VertexTexture> vertexTextures;
    std::vector<VertexNormal> vertexNormals;
    std::vector<VertexColor> vertexColors;
    std::vector<Face> faces;
    std::map<std::string, Material*> materials;
};


#endif
