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
 * $Id: OBJModel.cxx,v 1.1 2006/07/19 01:56:21 lorien420 Exp $
 */

#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <list>

#include "OBJModel.h"

using namespace std;

static void stripNewline(char *str)
{
  // Remove newline from the string (either \r,\n or \r\n)
  int len = strlen(str);
  if (str[len - 2] == '\r') str[len - 2] = '\0';
  else str[len - 1] = '\0';
}

static string stripFilename(const string& path)
{
  // Strip the filename portion from the given path
  int pos = path.rfind("/");
  return path.substr(0, pos + 1);
}

static GLuint loadImage(const string& filename)
{
  if (true) {
    cerr << "Error loading image " << filename << std::endl;
    return 0;
  } else {
    GLuint tid;

    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return tid;
  }
}

OBJModel::OBJModel(const string &filename)
{
  load(filename);
}

OBJModel::~OBJModel()
{
  // FIXME: Delete created materials
}


void OBJModel::render()
{
  // TODO: Could be optimized by using vertex arrays

  vector<Face>::iterator fi;
  vector<FaceVertex>::iterator vi;
  int v, vt, vn, vc;
  Material *activeMaterial = 0;

  // Draw all faces
  for (fi = faces.begin(); fi != faces.end(); fi++) {
    // Check if we need to set new material properties
    if (fi->material && fi->material != activeMaterial) {
      glMaterialf(GL_FRONT,GL_SHININESS,fi->material->shininess);
      glMaterialfv(GL_FRONT,GL_AMBIENT,fi->material->ambient);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,fi->material->diffuse);
      glMaterialfv(GL_FRONT,GL_SPECULAR,fi->material->specular);
      if (fi->material->textureId > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, fi->material->textureId);
      }
      else {
        glDisable(GL_TEXTURE_2D);
      }
      activeMaterial = fi->material;
    }
    glBegin(GL_POLYGON);
    for (vi = fi->vertices.begin(); vi != fi->vertices.end(); vi++) {
      v = vi->v - 1;
      vt = vi->vt - 1;
      vn = vi->vn - 1;
      vc = vi->vc - 1;

      if (vt >= 0) {
        glTexCoord3f(
            vertexTextures[vt].u,
            vertexTextures[vt].v,
            vertexTextures[vt].w);
      }
      if (vc >= 0) {
        glColor3f(
            vertexColors[vc].r,
            vertexColors[vc].g,
            vertexColors[vc].b);
      }

      glNormal3f(
          vertexNormals[vn].i,
          vertexNormals[vn].j,
          vertexNormals[vn].k);

      glVertex4f(
          vertices[v].x,
          vertices[v].y,
          vertices[v].z,
          vertices[v].w);
    }
    glEnd();
  }

  //SGNode::render();
}


void OBJModel::load(const string &filename)
{
  vertices.clear();
  vertexTextures.clear();
  vertexNormals.clear();
  vertexColors.clear();
  faces.clear();

  FILE* file = fopen(filename.c_str(), "r");
  char str[256];

  if (!file) {
    cerr << "Error while opening " << filename << "!\n";
    return;
  }

  // By default no material is used
  Material *activeMaterial = 0;

  while (!feof(file)) {
    str[0] = '\0';
    fgets(str, 256, file);

    switch (str[0]) {
      case 'v':
        switch (str[1]) {
          case ' ': // Plain vertex ("v x y z w")
            {
              Vertex v;
              sscanf(str, "v %f %f %f %f",
                  &(v.x), &(v.y), &(v.z), &(v.w));
              vertices.push_back(v);
              //cout << "vt " << (v.x * 0.5) << " " << (v.y * 0.5) << " " << (v.z * 0.5) << endl;
            }
            break;
          case 't': // Texture vertex ("vt u v w")
            {
              VertexTexture vt;
              sscanf(str, "vt %f %f %f", &(vt.u), &(vt.v), &(vt.w));
              vertexTextures.push_back(vt);
            }
            break;
          case 'n': // Vertex Normal ("vn i j k")
            {
              VertexNormal vn;
              sscanf(str, "vn %f %f %f", &(vn.i), &(vn.j), &(vn.k));
              vertexNormals.push_back(vn);
            }
            break;
          case 'c': // Vertex Color ("vc r g b a") - not standard OBJ
            {
              VertexColor vc;
              sscanf(str, "vc %f %f %f %f",
                  &(vc.r), &(vc.g), &(vc.b), &(vc.a));
              vertexColors.push_back(vc);
            }
            break;
          default: break;
        }
        break;
      case 'f': // Face ("f v/vt/vn v/vt/vn v/vt/vn")
        {
          char* vstr;
          char texlessfaces;
          Face f;
          f.material = activeMaterial;

          vstr = strtok(&str[2], " "); // skip "f "
          for (; vstr; vstr = strtok(NULL," ")) {
            FaceVertex fv;

            // Check if a//c or a/b/c
            sscanf(vstr,"%*d%*c%c", &texlessfaces);
            if (texlessfaces != '/') {
              sscanf(vstr, "%d/%d/%d", &(fv.v), &(fv.vt), &(fv.vn));
            } else {
              sscanf(vstr, "%d//%d", &(fv.v), &(fv.vn));
            }

            f.vertices.push_back(fv);
          }

          // Add the face to the object if there are at least 3 vertices
          if (f.vertices.size() >= 3) {
            faces.push_back(f);
            //cout << "f";
            //vector<FaceVertex>::iterator vi;
            //for (vi = f.vertices.begin(); vi != f.vertices.end(); vi++) {
            //  cout << " " << vi->v << "/" << vi->v << "/" << vi->vn;
            //}
            //cout << endl;
          }
        }
        break;
      case 'o': // Object name ("o name")
        {
          stripNewline(str);
          name = string(&str[2]);
        }
        break;
      case 'u': // ("usemtl name")
        if (strncmp(str, "usemtl ", 7) == 0) {
          stripNewline(str);
          string matName = string(&str[7]);
          if (materials.count(matName) > 0) {
            activeMaterial = materials[matName];
          }
        }
        break;
      case 'm': // ("mtllib file1 file2 ...")
        if (strncmp(str, "mtllib ", 7) == 0) {
          // Read material libraries
          string path = stripFilename(filename);
          char* vstr;

          vstr = strtok(&str[7], " \r\n"); // skip "mtllib "
          for (; vstr; vstr = strtok(NULL," \r\n")) {
            loadMTL(path + string(vstr));
          }
        }
        break;
      case 's': // ("shadow_obj file")
        if (strncmp(str, "shadow_obj ", 11) == 0) {
          stripNewline(str);
          shadowObj = string(&str[11]);
        }
        break;
      default:
        // Nothing recognized, ignore.
        break;
    }
  }

  cout << "Loaded model " << name << " (" << faces.size() << " faces)\n";
}


void OBJModel::loadMTL(const string &filename)
{
  FILE* file = fopen(filename.c_str(), "r");
  if (!file) {
    cerr << "Error while opening " << filename << "!\n";
    return;
  }

  char str[256];
  Material* mat = 0;

  while (!feof(file)) {
    str[0] = '\0';
    fgets(str, 256, file);

    switch (str[0]) {
      case 'n': // ("newmtl name")
        if (strncmp(str, "newmtl ", 7) == 0) {
          // TODO: Check if same name already exists
          stripNewline(str);
          string matName = string(&str[7]);
          if (materials.count(matName) == 0) {
            mat = new Material();
            mat->name = matName;
            materials[matName] = mat;
          } else {
            mat = materials[matName];
          }
        }
    }

    if (!mat) continue;

    switch (str[0]) {
      case 'K':
        switch (str[1]) {
          case 'a': // ("Ka r g b")
            // Ambient
            sscanf(str, "Ka %f %f %f",
                &mat->ambient[0], &mat->ambient[1], &mat->ambient[2]);
            break;
          case 'd': // ("Kd r g b")
            // Diffuse
            sscanf(str, "Kd %f %f %f",
                &mat->diffuse[0], &mat->diffuse[1], &mat->diffuse[2]);
            break;
          case 's': // ("Ks r g b")
            // Specular
            sscanf(str, "Ks %f %f %f",
                &mat->specular[0],&mat->specular[1],&mat->specular[2]);
            break;
          default: break;
        }
      case 'i': // ("illum i")
        // Illumination model (0, 1 or 2)
        if (strncmp(str, "illum ", 6)) {
          sscanf(str, "illum %d", &mat->illuminationModel);
        }
        else if (strncmp(str, "d ", 2)) {
          sscanf(str, "d %d", &mat->illuminationModel);
        }
        break;
      case 'T':
        if (str[1] == 'r') {
          // ("Tr transparency")
          sscanf(str, "Tr %f", &mat->transparency);
        }
        mat->ambient[3] = mat->transparency;
        mat->specular[3] = mat->transparency;
        mat->diffuse[3] = mat->transparency;
        break;
      case 'd': // ("d transparency")
        sscanf(str, "d %f", &mat->transparency);
        mat->ambient[3] = mat->transparency;
        mat->specular[3] = mat->transparency;
        mat->diffuse[3] = mat->transparency;
        break;
      case 'N':
        if (str[1] == 's') {
          // ("Ns shininess")
          sscanf(str, "Ns %f", &mat->shininess);
        }
        break;
      case 'm':
        if (strncmp(str, "map_K", 5) == 0) {
          switch (str[5]) {
            case 'd': // ("map_Kd filename") - Diffuse map
              {
                stripNewline(&str[7]);
                string imageFile =
                  stripFilename(filename) + string(&str[7]);
                mat->textureId = loadImage(imageFile);
                break;
              }
            case 'a': // ("map_Ka filename") - Ambient map
              break;
            case 's': // ("map_Ks filename") - Specular map
              break;
          }
        }
        else if (strncmp(str, "map_Bump ", 9) == 0) {
          // ("map_Bump filename") - Bump map
        }
        else if (strncmp(str, "map_d ", 6) == 0) {
          // ("map_d filename") - Opacity map
        }
        break;
      default:
        // Nothing recognized, ignore.
        break;
    }
  }
}
