/*
 * DrawBlocks.cxx
 * Daniel Nelson - 8/31/0
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
 * Handle's the displaying of the blocks.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "Creep.h"
#include "Block.h"
#include "BlockManager.h"
#include "LightManager.h"
#include "Swapper.h"
#include "X.h"

const GLfloat Displayer::block_colors[BF_NUMBER][3]
 = { { 0.73f, 0.0f, 0.73f },  // purple
     { 0.2f, 0.2f, 0.8f },    // blue
     { 0.0f, 0.6f, 0.05f },   // green
     { 0.85f, 0.85f, 0.0f },  // yellow
     { 1.0f, 0.4f, 0.0f },    // orange
     { 1.0f, 0.0f, 0.0f },    // wild
     { 0.4f, 0.4f, 0.4f },    // gray
     { 0.05f, 0.05f, 0.05f }, // black
     { 0.95f, 0.95f, 0.95f }, // white
     { 2.0f * 0.73f, 2.0f * 0.0f, 2.0f * 0.73f },  // special purple
     { 2.0f * 0.2f, 2.0f * 0.2f, 2.0f * 0.8f },    // special blue
     { 2.0f * 0.0f, 2.0f * 0.6f, 2.0f * 0.05f },   // special green
     { 2.0f * 0.85f, 2.0f * 0.85f, 2.0f * 0.0f },  // special yellow
     { 2.0f * 1.0f, 2.0f * 0.4f, 2.0f * 0.0f } };  // special orange

const GLfloat creep_colors[BF_NUMBER][3]
 = { { 0.25f * 0.73f, 0.25f * 0.0f, 0.25f * 0.73f },  // purple
     { 0.25f * 0.2f, 0.25f * 0.2f, 0.25f * 0.8f },    // blue
     { 0.25f * 0.0f, 0.25f * 0.6f, 0.25f * 0.05f },   // green
     { 0.25f * 0.85f, 0.25f * 0.85f, 0.25f * 0.0f },  // yellow
     { 0.25f * 1.0f, 0.25f * 0.4f, 0.25f * 0.0f },    // orange
     { 0.25f * 1.0f, 0.25f * 0.0f, 0.25f * 0.0f },    // wild
     { 0.25f * 0.4f, 0.25f * 0.4f, 0.25f * 0.4f },    // gray
     { 0.25f * 0.05f, 0.25f * 0.05f, 0.25f * 0.05f }, // black
     { 0.25f * 0.95f, 0.25f * 0.95f, 0.25f * 0.95f }, // white
     { 0.5f * 0.73f, 0.5f * 0.0f, 0.5f * 0.73f },     // special purple
     { 0.5f * 0.2f, 0.5f * 0.2f, 0.5f * 0.8f },       // special blue
     { 0.5f * 0.0f, 0.5f * 0.6f, 0.5f * 0.05f },      // special green
     { 0.5f * 0.85f, 0.5f * 0.85f, 0.5f * 0.0f },     // special yellow
     { 0.5f * 1.0f, 0.5f * 0.4f, 0.5f * 0.0f } };     // special orange

void Displayer::setWildBlockColor ( Block &block, float flash )
{
  Wild &wild = X::wild(block);

  if (wild.alarm < GC_WILD_POLYMORPH_PERIOD) {
    GLfloat fade = wild.alarm * (1.0f / (GLfloat) GC_WILD_POLYMORPH_PERIOD);

    int next_color;
    if (wild.flavor != BF_NORMAL_1)
      next_color = wild.flavor - 1;
    else
      next_color = BF_WILD;

    GLfloat color[3];
    if (block.y != 0) {
      color[0] = fade * block_colors[wild.flavor][0]
       + (1.0f - fade) * block_colors[next_color][0];
      color[1] = fade * block_colors[wild.flavor][1]
       + (1.0f - fade) * block_colors[next_color][1];
      color[2] = fade * block_colors[wild.flavor][2]
       + (1.0f - fade) * block_colors[next_color][2];
    } else {
      color[0] = fade * creep_colors[wild.flavor][0]
       + (1.0f - fade) * creep_colors[next_color][0];
      color[1] = fade * creep_colors[wild.flavor][1]
       + (1.0f - fade) * creep_colors[next_color][1];
      color[2] = fade * creep_colors[wild.flavor][2]
       + (1.0f - fade) * creep_colors[next_color][2];
    }

    glColor3f(color[0] + flash * (DC_FLASH_COLOR_RED - color[0]),
     color[1] + flash * (DC_FLASH_COLOR_GREEN - color[1]),
     color[2] + flash * (DC_FLASH_COLOR_BLUE - color[2]));

  } else
    if (block.y != 0)
      glColor3f(block_colors[wild.flavor][0]
       + flash * (DC_FLASH_COLOR_RED - block_colors[wild.flavor][0]),
       block_colors[wild.flavor][1]
       + flash * (DC_FLASH_COLOR_GREEN - block_colors[wild.flavor][1]),
       block_colors[wild.flavor][2]
       + flash * (DC_FLASH_COLOR_BLUE - block_colors[wild.flavor][2]));
    else
      glColor3f(creep_colors[wild.flavor][0]
       + flash * (DC_FLASH_COLOR_RED - creep_colors[wild.flavor][0]),
       creep_colors[wild.flavor][1]
       + flash * (DC_FLASH_COLOR_GREEN - creep_colors[wild.flavor][1]),
       creep_colors[wild.flavor][2]
       + flash * (DC_FLASH_COLOR_BLUE - creep_colors[wild.flavor][2]));
}

void Displayer::drawBlocks (   )
{
  glColorMaterial(GL_FRONT, GL_DIFFUSE);

  // GL_FRONT_AND_BACK because the garbage uses these material properties
  // also and I don't think it hurts with cull face on.
  GLfloat color[4];
  color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
  color[0] = 0.5f; color[1] = 0.5f; color[2] = 0.5f; color[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
  color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);

  // First we'll draw the non-special color blocks.

  int c = BlockManager::block_count;
  for (int n = 0; c; n++)
    if (BlockManager::storeMap[n]) {
      Block &block = BlockManager::blockStore[n];
      c--;

      if (BlockManager::isSpecialColorFlavor(block.flavor)) continue;

      if (block.y > GC_SAFE_HEIGHT) continue;

      drawBlock(block, false);
    }

  if (!X::specialColorActive()) return;

  // Then we set things up for special color blocks and draw them.

  if (opengl_version_1_2)
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

  glEnable(GL_TEXTURE_1D);

  glBindTexture(GL_TEXTURE_1D, special_block_lightmap);

  c = X::special_color_count;
  for (int n = 0; c; n++)
    if (BlockManager::storeMap[n]) {
      Block &block = BlockManager::blockStore[n];

      if (!BlockManager::isSpecialColorFlavor(block.flavor)) continue;
      c--;

      glMatrixMode(GL_TEXTURE);
      glPushMatrix();

        glTranslatef(((Game::time_step + block.X) & (DC_GLEAM_PERIOD - 1))
         * (1.0f / (GLfloat) DC_GLEAM_PERIOD), 0.0f, 0.0f);
        glRotatef(DC_GLEAM_ROTATION_RATE * (Game::time_step + block.X),
         DC_GLEAM_ROTATION_AXIS_X, DC_GLEAM_ROTATION_AXIS_Y,
         DC_GLEAM_ROTATION_AXIS_Z);
        glMatrixMode(GL_MODELVIEW);

        drawBlock(block, true);

      glMatrixMode(GL_TEXTURE);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }

  glDisable(GL_TEXTURE_1D);

  if (opengl_version_1_2)
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
}

inline void Displayer::drawBlock ( Block &block, bool special )
{
  GLfloat x, y;
  GLfloat d_x;

  glPushMatrix();

    y = block.y * DC_GRID_ELEMENT_LENGTH
     + block.f_y * (DC_GRID_ELEMENT_LENGTH / (GLfloat) GC_STEPS_PER_GRID)
     + play_offset_y;

    // normal
   if (block.state & (BS_STATIC | BS_FALLING)) {
      x = block.x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X;

      if (block.flavor != BF_WILD)
        if (block.y != 0)
          glColor3fv(block_colors[block.flavor]);
        else
          glColor3fv(creep_colors[block.flavor]);
      else
        setWildBlockColor(block, 0.0f);

      LightManager::setupBlockLights(x, y);
      glTranslatef(x, y, DC_PLAY_OFFSET_Z);

      if (!special)
        glCallList(block_list);
      else
        glCallList(special_block_list);

    // awaking and popping
    } else if (block.state & BS_AWAKING) {
      x = block.x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X;

      LightManager::setupBlockLights(x, y);
      glTranslatef(x, y, DC_PLAY_OFFSET_Z);

      // if we haven't popped yet
      if (block.pop_alarm != 0) {

        // if we're just about to pop
        if (block.pop_alarm - Game::time_step < DC_POP_ROTATE_TIME) {
          GLfloat p = (block.pop_alarm - Game::time_step)
           * (1.0f /  (GLfloat) DC_POP_ROTATE_TIME);
       
          // each block rotates a different direction
          if (block.pop_direction & BR_DIRECTION_1) {
            glRotatef(45.0f * p, 1.0f, 0.0f, 0.0f);
            glRotatef(45.0f * p, 0.0f, 1.0f, 0.0f);
          } else if (block.pop_direction & BR_DIRECTION_2) {
            glRotatef(90.0f - 45.0f * p, 1.0f, 0.0f, 0.0f);
            glRotatef(45.0f * p, 0.0f, 1.0f, 0.0f);
          } else if (block.pop_direction & BR_DIRECTION_3) {
            glRotatef(90.0f - 45.0f * p, 1.0f, 0.0f, 0.0f);
            glRotatef(-45.0f * p, 0.0f, 1.0f, 0.0f);
          } else {
            glRotatef(45.0f * p, 1.0f, 0.0f, 0.0f);
            glRotatef(-45.0f * p, 0.0f, 1.0f, 0.0f);
          }

          GLfloat scale = 1.0f - 0.5f * p;
          glScalef(scale, scale, scale);

          glColor3f( (1.0f - p) * block_colors[block.flavor][0]
           + p * garbage_colors[block.pop_color][0],
           (1.0f - p) * block_colors[block.flavor][1]
           + p * garbage_colors[block.pop_color][1],
           (1.0f - p) * block_colors[block.flavor][2]
           + p * garbage_colors[block.pop_color][2]);

          glEnable(rescale_method);
          glCallList(block_list);
          glDisable(rescale_method);

        // if we're not popping
        } else {
          
          glColor3fv(garbage_colors[block.pop_color]);

          // the small block is pre-rotated
          glCallList(small_block_list);
        }

      // if we've already popped
      } else {
        glColor3fv(block_colors[block.flavor]);

        glCallList(block_list);
      }

    // swapping
    } else if (block.state & BS_SWAPPING) {
      x = block.x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X;

     if (block.state & BS_SWAP_DIRECTION_MASK) {
        LightManager::setupBlockLights(x + Swapper::swap_factor
         * DC_GRID_ELEMENT_LENGTH, y);
       d_x = -DC_GRID_ELEMENT_LENGTH / 2.0f;

     } else {
        LightManager::setupBlockLights(x - Swapper::swap_factor
         * DC_GRID_ELEMENT_LENGTH, y);
       d_x = DC_GRID_ELEMENT_LENGTH / 2.0f;
      }

      if (block.flavor != BF_WILD)
        glColor3fv(block_colors[block.flavor]);
      else
        setWildBlockColor(block, 0.0f);

      glTranslatef(x - d_x, y, DC_PLAY_OFFSET_Z);
      glRotatef(-180.0f * (1.0f - Swapper::swap_factor), 0.0f, 1.0f, 0.0f);
      glTranslatef(d_x, 0.0f, 0.0f);

      if (!special)
        glCallList(block_list);
      else
        glCallList(special_block_list);

    // dying
   } else if (block.state & BS_DYING) {
      x = block.x * DC_GRID_ELEMENT_LENGTH + DC_PLAY_OFFSET_X;

      // when dying, first we flash
      if (GC_DYING_DELAY - block.alarm < DC_DYING_FLASH_TIME) {

        GLfloat flash = ((GLfloat) GC_DYING_DELAY - block.alarm)
         * (4.0f / (GLfloat) DC_DYING_FLASH_TIME);
        if (flash > 2.0f) flash = 4.0f - flash;
        if (flash > 1.0f) flash = 2.0f - flash;

        if (block.flavor != BF_WILD)
          glColor3f(block_colors[block.flavor][0]
           + flash * (DC_FLASH_COLOR_RED - block_colors[block.flavor][0]),
           block_colors[block.flavor][1]
           + flash * (DC_FLASH_COLOR_GREEN - block_colors[block.flavor][1]),
           block_colors[block.flavor][2]
           + flash * (DC_FLASH_COLOR_BLUE - block_colors[block.flavor][2]));
        else
          setWildBlockColor(block, flash);

        LightManager::setupBlockLights(x, y);
        glTranslatef(x, y, DC_PLAY_OFFSET_Z);
        if (!special)
          glCallList(block_list);
        else
          glCallList(special_block_list);

      // then we shrink and spin
      } else {
        if (block.flavor != BF_WILD)
          glColor3fv(block_colors[block.flavor]);
        else
          setWildBlockColor(block, 0.0f);

        LightManager::setupBlockLights(x, y);
        glTranslatef(x, y, DC_PLAY_OFFSET_Z);
        glRotatef((GC_DYING_DELAY - DC_DYING_FLASH_TIME - block.alarm)
         * (GC_DYING_DELAY - DC_DYING_FLASH_TIME - block.alarm)
         * DC_DYING_ROTATE_SPEED, block.axis_x, block.axis_y, 0.0f);

        GLfloat scale = block.alarm * DC_DYING_SHRINK_SPEED
         + DC_DYING_SHRINK_MIN_SIZE;
        glScalef(scale, scale, scale);

        glEnable(rescale_method);
        if (!special)
          glCallList(block_list);
        else
          glCallList(special_block_list);
        glDisable(rescale_method);
      }
    }

  glPopMatrix();
}
