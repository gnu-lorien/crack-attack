/*
 * messages.cxx
 * Daniel Nelson - 10/27/0
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
 * Loads the textures that are used in the various signs within the game.  These
 * signs are held in RAM and only made into textures when needed.
 */

#include <GL/glut.h>
#include <cmath>

#ifndef _WIN32
#  include <sstream>
#else
#  include <glext.h>
#  include <strstrea.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "TextureLoader.h"
#include "MessageManager.h"

GLuint Displayer::message_1x1_list;
GLuint Displayer::message_2x1_list;
GLuint Displayer::message_4x1_list;
GLuint Displayer::message_16x1_list;
GLuint Displayer::message_texture;

GLubyte *Displayer::message_texture_data[DC_NUMBER_MESSAGE_TEX];

GLsizei Displayer::message_width[DC_NUMBER_MESSAGE_TEX]
 = { 256, 128, 128, 128, 256, 256, 256, 256, 256, 256 };

GLsizei Displayer::message_height[DC_NUMBER_MESSAGE_TEX]
 = { 128, 128, 128, 128, 128, 64, 64, 256, 256, 256 };

GLfloat Displayer::message_alpha[DC_MESSAGE_PULSE_PERIOD];

int Displayer::message_shape[DC_NUMBER_MESSAGE_TEX]
 = { MS_2x1, MS_1x1, MS_1x1, MS_1x1, MS_2x1,
     MS_4x1, MS_4x1, MS_1x1, MS_1x1, MS_1x1 };

void Displayer::generateMessages (   )
{
  // load count down messages
  message_texture_data[MS_COUNT_DOWN_1]
   = TextureLoader::loadAlphaTGA(GC_COUNT_DOWN_1_TEX_FILE_NAME,
   message_height[MS_COUNT_DOWN_1], message_width[MS_COUNT_DOWN_1]);
  message_texture_data[MS_COUNT_DOWN_2]
   = TextureLoader::loadAlphaTGA(GC_COUNT_DOWN_2_TEX_FILE_NAME,
   message_height[MS_COUNT_DOWN_2], message_width[MS_COUNT_DOWN_2]);
  message_texture_data[MS_COUNT_DOWN_3]
   = TextureLoader::loadAlphaTGA(GC_COUNT_DOWN_3_TEX_FILE_NAME,
   message_height[MS_COUNT_DOWN_3], message_width[MS_COUNT_DOWN_3]);
  message_texture_data[MS_COUNT_DOWN_GO]
   = TextureLoader::loadAlphaTGA(GC_COUNT_DOWN_GO_TEX_FILE_NAME,
   message_height[MS_COUNT_DOWN_GO], message_width[MS_COUNT_DOWN_GO]);
  message_texture_data[MS_ANYKEY]
   = TextureLoader::loadAlphaTGA(GC_ANYKEY_TEX_FILE_NAME,
   message_height[MS_ANYKEY], message_width[MS_ANYKEY]);
  message_texture_data[MS_WAITING]
   = TextureLoader::loadAlphaTGA(GC_WAITING_TEX_FILE_NAME,
   message_height[MS_WAITING], message_width[MS_WAITING]);
  message_texture_data[MS_PAUSED]
   = TextureLoader::loadAlphaTGA(GC_PAUSED_TEX_FILE_NAME,
   message_height[MS_PAUSED], message_width[MS_PAUSED]);
  message_texture_data[MS_WINNER]
   = TextureLoader::loadAlphaTGA(GC_WINNER_TEX_FILE_NAME,
   message_height[MS_WINNER], message_width[MS_WINNER]);
  message_texture_data[MS_LOSER]
   = TextureLoader::loadAlphaTGA(GC_LOSER_TEX_FILE_NAME,
   message_height[MS_LOSER], message_width[MS_LOSER]);
  message_texture_data[MS_GAME_OVER]
   = TextureLoader::loadAlphaTGA(GC_GAME_OVER_TEX_FILE_NAME,
   message_height[MS_GAME_OVER], message_width[MS_GAME_OVER]);

  // generic message lists

  message_1x1_list = glGenLists(1);

  glNewList(message_1x1_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 1.0f, 0.0f);

    glEnd();
  glEndList();

  message_2x1_list = glGenLists(1);

  glNewList(message_2x1_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, -0.5f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, -0.5f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.5f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.5f, 0.0f);

    glEnd();
  glEndList();
  
  message_4x1_list = glGenLists(1);

  glNewList(message_4x1_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, -0.25f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, -0.25f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.25f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.25f, 0.0f);

    glEnd();
  glEndList();
  
  message_16x1_list = glGenLists(1);

  glNewList(message_16x1_list, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, -0.125f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, -0.125, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.125, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.125, 0.0f);

    glEnd();
  glEndList();

  // pulsing alpha values
  for (int n = DC_MESSAGE_PULSE_PERIOD; n--; ) {
    float s = cos(n * (2.0f * PI / (float) DC_MESSAGE_PULSE_PERIOD));
    message_alpha[n] = 0.75f + 0.6f * s * s;
  }

}
