/*
 * DrawGarbages.cxx
 * Daniel Nelson - 9/1/0
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
 * Handle's the displaying of the garbage.
 */

#define GL_GLEXT_LEGACY

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Block.h"
#include "Creep.h"
#include "Displayer.h"
#include "Garbage.h"
#include "GarbageManager.h"
#include "LightManager.h"
#include "GarbageFlavorImage.h"

const GLfloat white[3]
 = { 1.0f, 1.0f, 1.0f };

const GLfloat Displayer::garbage_colors[GF_NUMBER][3]
 = { { 1.0f, 0.0f, 0.0f },    // normal
     { 0.4f, 0.4f, 0.4f },    // gray
     { 0.05f, 0.05f, 0.05f }, // black
     { 0.95f, 0.95f, 0.95f }, // white
     { 0.73f, 0.0f, 0.73f },  // purple
     { 0.2f, 0.2f, 0.8f },    // blue
     { 0.0f, 0.6f, 0.05f },   // green
     { 0.85f, 0.85f, 0.0f },  // yellow
     { 1.0f, 0.4f, 0.0f } };  // orange

const GLfloat xy_swap_matrix[]
 = { 0.0f, 1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, -1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f };

GLdouble shatter_clip_plane[4]
 = { 0.0, 1.0, 0.0, 0.0 };

void Displayer::drawGarbage (   )
{
  GLfloat x, y;

  glBindTexture(GL_TEXTURE_2D, garbage_lightmap);

  // blocks have already been drawn, so we use their material calls

  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

#ifdef DC_CLIP_SHATTER
  bool clipping = false;
#else
  bool fading = false;
  GLfloat alpha = 0.0f;
#endif

  int c = GarbageManager::garbage_count;
  for (int n = 0; c; n++)
    if (GarbageManager::storeMap[n]) {
      Garbage &garbage = GarbageManager::garbageStore[n];
      c--;

      if (garbage.y > GC_SAFE_HEIGHT) continue;

      glPushMatrix();

        y = garbage.y * DC_GRID_ELEMENT_LENGTH
         + garbage.f_y * (DC_GRID_ELEMENT_LENGTH / (GLfloat) GC_STEPS_PER_GRID)
         + play_offset_y;
        x = garbage.x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X;

        // awaking; it is assumed that awaking garbage is always grid wide
        if (garbage.state & GS_AWAKING) {

          // if still popping our internal sections
          if (garbage.alarm - Game::time_step > DC_UNSHATTER_TIME) {

            // no garbage lightmap for blocks
            glDisable(GL_TEXTURE_2D);

            int popped = garbage.sections_popped;

            for (int h = 0; h < garbage.height; h++)
              for (int w = 0; w < GC_PLAY_WIDTH; w++) {

                glPushMatrix();

                LightManager::setupBlockLights(x + w * DC_GRID_ELEMENT_LENGTH,
                 y + h * DC_GRID_ELEMENT_LENGTH);
                glTranslatef(x + w * DC_GRID_ELEMENT_LENGTH,
                 y + h * DC_GRID_ELEMENT_LENGTH, DC_PLAY_OFFSET_Z);

                // if this section is about to pop; this code here mimics code
                // found in DrawBlocks.cxx
                if (popped == 0
                 && garbage.pop_alarm - Game::time_step < DC_POP_ROTATE_TIME) {
                  GLfloat p = (garbage.pop_alarm - Game::time_step)
                   * (1.0f / (GLfloat) DC_POP_ROTATE_TIME);
                  
                  // each block rotates a different direction
                  if (garbage.pop_direction & BR_DIRECTION_1) {
                    glRotatef(45.0f * p, 1.0f, 0.0f, 0.0f);
                    glRotatef(45.0f * p, 0.0f, 1.0f, 0.0f);
                  } else if (garbage.pop_direction & BR_DIRECTION_2) {
                    glRotatef(90.0f - 45.0f * p, 1.0f, 0.0f, 0.0f);
                    glRotatef(45.0f * p, 0.0f, 1.0f, 0.0f);
                  } else if (garbage.pop_direction & BR_DIRECTION_3) {
                    glRotatef(90.0f - 45.0f * p, 1.0f, 0.0f, 0.0f);
                    glRotatef(-45.0f * p, 0.0f, 1.0f, 0.0f);
                  } else {
                    glRotatef(45.0f * p, 1.0f, 0.0f, 0.0f);
                    glRotatef(-45.0f * p, 0.0f, 1.0f, 0.0f);
                  }

                  GLfloat scale = 1.0f - 0.5f * p;
                  glScalef(scale, scale, scale);

                  if (garbage.pop_color == garbage.flavor)
                    glColor3fv(garbage_colors[garbage.flavor]);
                  else
                    glColor3f( (1.0f - p) * garbage_colors[garbage.flavor][0]
                     + p * garbage_colors[garbage.pop_color][0],
                     (1.0f - p) * garbage_colors[garbage.flavor][1]
                     + p * garbage_colors[garbage.pop_color][1],
                     (1.0f - p) * garbage_colors[garbage.flavor][2]
                     + p * garbage_colors[garbage.pop_color][2]);

                  glEnable(rescale_method);
                  glCallList(block_list);
                  glDisable(rescale_method);

                // if this section has popped
                } else if (popped > 0) {
                  glColor3fv(garbage_colors[garbage.flavor]);

                  glCallList(block_list);

                // if this section has not popped
                } else {
                  glColor3fv(garbage_colors[garbage.pop_color]);

                  // the small block is pre-rotated
                  glCallList(small_block_list);
                }

                popped--;
                glPopMatrix();
              }
          
            // don't draw a shell
            glPopMatrix();

            glEnable(GL_TEXTURE_2D);

            continue;

          // if we're about to awake; all sections have popped
          } else {
            // crunch our sections back up

            // no garbage lightmap for blocks
            glDisable(GL_TEXTURE_2D);

            GLfloat p = (DC_UNSHATTER_TIME - garbage.alarm + Game::time_step)
             * (1.0f / (GLfloat) DC_UNSHATTER_TIME);
            GLfloat scale = 1.0f - 0.5f * p;

            glColor3fv(garbage_colors[garbage.flavor]);

            glEnable(rescale_method);

            for (int h = 0; h < garbage.height; h++)
              for (int w = 0; w < GC_PLAY_WIDTH; w++) {

                glPushMatrix();

                LightManager::setupBlockLights(x + w * DC_GRID_ELEMENT_LENGTH,
                 y + h * DC_GRID_ELEMENT_LENGTH);
                glTranslatef(x + w * DC_GRID_ELEMENT_LENGTH,
                 y + h * DC_GRID_ELEMENT_LENGTH, DC_PLAY_OFFSET_Z);
                  
                glRotatef(45.0f * p, 1.0f, 0.0f, 0.0f);
                glRotatef(45.0f * p, 0.0f, 1.0f, 0.0f);

                glScalef(scale, scale, scale);

                glCallList(block_list);

                glPopMatrix();
              }

            glDisable(rescale_method);

            glEnable(GL_TEXTURE_2D);

            // if we're in the first half of the unshatter
            if (garbage.alarm - Game::time_step > DC_UNSHATTER_TIME / 4) {
              // don't draw a shell
              glPopMatrix();
              continue;
            }

            LightManager::setupGarbageLights(x, y, garbage.height,
             garbage.width);

            glTranslatef(x, y, DC_PLAY_OFFSET_Z);

            // only push texture matrix once we know we're using it
            glMatrixMode(GL_TEXTURE);
            glPushMatrix();
            glTranslatef(0.5f + x * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
             0.5f + y * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f);
            glMatrixMode(GL_MODELVIEW);

            // clip pane into existance

#ifdef DC_CLIP_SHATTER
            clipping = true;
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
            glDisable(GL_CULL_FACE);

            glEnable(GL_CLIP_PLANE_SHATTERING);
            shatter_clip_plane[3] = -(garbage.alarm - Game::time_step)
             * garbage.height * (DC_GRID_ELEMENT_LENGTH * 4.0f
             / (GLfloat) DC_UNSHATTER_TIME)
             + (DC_GRID_ELEMENT_LENGTH / 2.0f);
            glClipPlane(GL_CLIP_PLANE_SHATTERING, shatter_clip_plane);
#else
            fading = true;
            alpha = (DC_UNSHATTER_TIME / 4 - garbage.alarm + Game::time_step)
             * (4.0f / (GLfloat) DC_UNSHATTER_TIME);
            glEnable(GL_BLEND);
#endif
          }

        } else {
          LightManager::setupGarbageLights(x, y, garbage.height,
           garbage.width);

          glTranslatef(x, y, DC_PLAY_OFFSET_Z);

          // only push texture matrix once we know we're using it
          glMatrixMode(GL_TEXTURE);
          glPushMatrix();
          glTranslatef(0.5f + x * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
           0.5f + y * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f);
          glMatrixMode(GL_MODELVIEW);
        }
        
        // normal and shattering

        // if we're shattering
        if (garbage.state & GS_SHATTERING) {

          // first we flash
          if (Game::time_step - garbage.alarm
           < (DC_SHATTER_FLASH_TIME - DC_SHATTER_TIME)) {

            GLfloat flash = ((GLfloat) DC_SHATTER_TIME - garbage.alarm
             + Game::time_step) * (4.0f / (GLfloat) DC_SHATTER_FLASH_TIME);
            if (flash > 2.0f) flash = 4.0f - flash;
            if (flash > 1.0f) flash = 2.0f - flash;
            glColor3f(garbage_colors[garbage.flavor][0] + flash
             * (DC_FLASH_COLOR_RED - garbage_colors[garbage.flavor][0]),
             garbage_colors[garbage.flavor][1] + flash
             * (DC_FLASH_COLOR_GREEN - garbage_colors[garbage.flavor][1]),
             garbage_colors[garbage.flavor][2] + flash
             * (DC_FLASH_COLOR_BLUE - garbage_colors[garbage.flavor][2]));

          } else {
            // then we clip plane away

#ifdef DC_CLIP_SHATTER
            clipping = true;
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
            glDisable(GL_CULL_FACE);

            glEnable(GL_CLIP_PLANE_SHATTERING);
            shatter_clip_plane[3] = -((DC_SHATTER_TIME - DC_SHATTER_FLASH_TIME)
             - garbage.alarm + Game::time_step) * garbage.height
             * (DC_GRID_ELEMENT_LENGTH / (GLfloat) (DC_SHATTER_TIME
             - DC_SHATTER_FLASH_TIME))
             + (DC_GRID_ELEMENT_LENGTH / 2.0f);
            glClipPlane(GL_CLIP_PLANE_SHATTERING, shatter_clip_plane);

            glColor3fv(garbage_colors[garbage.flavor]);
          }

        } else
          glColor3fv(garbage_colors[garbage.flavor]);
#else
            fading = true;
            alpha = (garbage.alarm - Game::time_step)
             * (1.0f / (GLfloat) (DC_SHATTER_TIME - DC_SHATTER_FLASH_TIME));
            glEnable(GL_BLEND);
            glColor4f(garbage_colors[garbage.flavor][0],
             garbage_colors[garbage.flavor][1],
             garbage_colors[garbage.flavor][2], alpha);
          }

        } else if (fading)
          glColor4f(garbage_colors[garbage.flavor][0],
           garbage_colors[garbage.flavor][1],
           garbage_colors[garbage.flavor][2], alpha);
        else
          glColor3fv(garbage_colors[garbage.flavor]);
#endif

        // thin garbage
        if (garbage.height == 1) {

          if (garbage.width != 1) {

            glCallList(garbage_thin_cap_list);
            for (int w = garbage.width - 1; --w; ) {
              glTranslatef(DC_GRID_ELEMENT_LENGTH, 0.0f, 0.0f);

              glMatrixMode(GL_TEXTURE);
              glTranslatef(DC_GRID_ELEMENT_LENGTH
               * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f, 0.0f);
              glMatrixMode(GL_MODELVIEW);

              glCallList(garbage_thin_middle_list);
            }
            glTranslatef(DC_GRID_ELEMENT_LENGTH, 0.0f, 0.0f);
            glScalef(-1.0f, 1.0f, -1.0f);

            glMatrixMode(GL_TEXTURE);
            glTranslatef(DC_GRID_ELEMENT_LENGTH
             * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f, 0.0f);
            glScalef(-1.0f, 1.0f, -1.0f);
            glMatrixMode(GL_MODELVIEW);

            glCallList(garbage_thin_cap_list);

          // tiny garbage
          } else
            glCallList(garbage_small_list);

        // thick garbage
        } else {

          // draw the bottom
          glPushMatrix();

          glMatrixMode(GL_TEXTURE);
          glPushMatrix();

            glScalef(1.0f, -1.0f, -1.0f);
            glMatrixMode(GL_MODELVIEW);

            glScalef(1.0f, -1.0f, -1.0f);

            glCallList(garbage_thick_corner_list);
            for (int w = garbage.width - 1; --w; ) {
              glTranslatef(DC_GRID_ELEMENT_LENGTH, 0.0f, 0.0f);

              glMatrixMode(GL_TEXTURE);
              glTranslatef(DC_GRID_ELEMENT_LENGTH
               * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f, 0.0f);
              glMatrixMode(GL_MODELVIEW);

              glCallList(garbage_thick_edge_list);
            }
            glTranslatef(DC_GRID_ELEMENT_LENGTH, 0.0f, 0.0f);
            glScalef(-1.0f, 1.0f, -1.0f);

            glMatrixMode(GL_TEXTURE);
            glTranslatef(DC_GRID_ELEMENT_LENGTH
             * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f, 0.0f);
            glScalef(-1.0f, 1.0f, -1.0f);

            glCallList(garbage_thick_corner_list);

          glPopMatrix();
          glMatrixMode(GL_MODELVIEW);

          glPopMatrix();

          // draw the middle
          for (int h = garbage.height - 1; --h;) {
            glPushMatrix();

            glMatrixMode(GL_TEXTURE);
            glPushMatrix();
            glMatrixMode(GL_MODELVIEW);

              int w = garbage.width - 1;
              glTranslatef(w * DC_GRID_ELEMENT_LENGTH,
               h * DC_GRID_ELEMENT_LENGTH, 0.0f);
              glMultMatrixf(xy_swap_matrix);

              glMatrixMode(GL_TEXTURE);
              glTranslatef(w * (DC_GRID_ELEMENT_LENGTH
               * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER), h
               * (DC_GRID_ELEMENT_LENGTH * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER),
               0.0f);
              glMultMatrixf(xy_swap_matrix);
              glMatrixMode(GL_MODELVIEW);

              glCallList(garbage_thick_edge_list);
              while (--w) {
                glTranslatef(0.0f, -DC_GRID_ELEMENT_LENGTH, 0.0f);

                glMatrixMode(GL_TEXTURE);
                glTranslatef(0.0f, -DC_GRID_ELEMENT_LENGTH
                 * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f);
                glMatrixMode(GL_MODELVIEW);

                glCallList(garbage_thick_middle_list);
              }
              glTranslatef(0.0f, -DC_GRID_ELEMENT_LENGTH, 0.0f);
              glScalef(1.0f, -1.0f, -1.0f);

              glMatrixMode(GL_TEXTURE);
              glTranslatef(0.0f, -DC_GRID_ELEMENT_LENGTH
               * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f);
              glScalef(1.0f, -1.0f, -1.0f);
              glMatrixMode(GL_MODELVIEW);

              glCallList(garbage_thick_edge_list);

            glPopMatrix();

            glMatrixMode(GL_TEXTURE);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
          }

          // draw the top
          glPushMatrix();

          glMatrixMode(GL_TEXTURE);
          glPushMatrix();

            glTranslatef(0.0f, (garbage.height - 1) * (DC_GRID_ELEMENT_LENGTH
             * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER), 0.0f);

            glMatrixMode(GL_MODELVIEW);
            glTranslatef(0.0f, (garbage.height - 1) * DC_GRID_ELEMENT_LENGTH,
             0.0f);

            glCallList(garbage_thick_corner_list);
            for (int w = garbage.width - 1; --w; ) {
              glTranslatef(DC_GRID_ELEMENT_LENGTH, 0.0f, 0.0f);

              glMatrixMode(GL_TEXTURE);
              glTranslatef(DC_GRID_ELEMENT_LENGTH
               * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f, 0.0f);
              glMatrixMode(GL_MODELVIEW);
              
              glCallList(garbage_thick_edge_list);
            }
            glTranslatef(DC_GRID_ELEMENT_LENGTH, 0.0f, 0.0f);
            glScalef(-1.0f, 1.0f, -1.0f);

            glMatrixMode(GL_TEXTURE);
            glTranslatef(DC_GRID_ELEMENT_LENGTH
             * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f, 0.0f);
            glScalef(-1.0f, 1.0f, -1.0f);

            glCallList(garbage_thick_corner_list);

          glPopMatrix();
          glMatrixMode(GL_MODELVIEW);

          glPopMatrix();

          // draw flavor image; if any
          if (GarbageFlavorImage::associated_garbage_id == garbage.id) {
#ifndef NO_MULTITEXTURING
            if (state & DS_MULTITEXTURING) {
              glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);

                glMatrixMode(GL_TEXTURE);
                glLoadIdentity();

                glBindTexture(GL_TEXTURE_2D, garbage_texture);

                glActiveTextureARB(GL_TEXTURE1_ARB);
                glPushMatrix();
                glTranslatef(0.5f + (x + 2.0f * GarbageFlavorImage::x)
                 * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER,
                 0.5f + (y + 2.0f * GarbageFlavorImage::y)
                 * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER, 0.0f);

                glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);

                glColor3fv(white);

                glMatrixMode(GL_MODELVIEW);
                glTranslatef(GarbageFlavorImage::x * DC_GRID_ELEMENT_LENGTH,
                 GarbageFlavorImage::y * DC_GRID_ELEMENT_LENGTH,
                 DC_GARBAGE_FLAVOR_TEX_OFFSET_Z);

                glCallList(garbage_flavor_list);

                glMatrixMode(GL_TEXTURE);
                glPopMatrix();
                glActiveTextureARB(GL_TEXTURE0_ARB);
                glMatrixMode(GL_MODELVIEW);

              glPopAttrib();

              glBindTexture(GL_TEXTURE_2D, garbage_lightmap);

            } else {
#endif
              glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);

                glMatrixMode(GL_TEXTURE);
                glLoadIdentity();

                glBindTexture(GL_TEXTURE_2D, garbage_texture);

                glEnable(GL_BLEND);

                glColor3fv(white);

                glMatrixMode(GL_MODELVIEW);
                glTranslatef(GarbageFlavorImage::x * DC_GRID_ELEMENT_LENGTH,
                 GarbageFlavorImage::y * DC_GRID_ELEMENT_LENGTH,
                 DC_GARBAGE_FLAVOR_TEX_OFFSET_Z);

                glCallList(garbage_flavor_list);

                glBindTexture(GL_TEXTURE_2D, garbage_lightmap);

              glPopAttrib();
#ifndef NO_MULTITEXTURING
            }
#endif
          }
        }

        // turn off the clip plane
#ifdef DC_CLIP_SHATTER
        if (clipping) {
          clipping = false;

          glDisable(GL_CLIP_PLANE_SHATTERING);
          glEnable(GL_CULL_FACE);
          glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
#else
        if (fading) {
          fading = false;

          glDisable(GL_BLEND);
#endif

          // delete fully shattered garbage; note that we can't assume
          // we'll display each time step
          if (garbage.alarm <= Game::time_step)
            GarbageManager::deleteGarbage(&garbage);
        }

      glPopMatrix();
      glMatrixMode(GL_TEXTURE);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }
}
