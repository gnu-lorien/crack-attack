/*
 * Displayer.cxx
 * Daniel Nelson - 8/26/0
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
 * Handles all the prettyness; or uglyness, as it now stands.
 */

#include <cstring>
#include <cctype>
#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

#ifdef DEVELOPMENT
#include <iomanip>
#ifndef _WIN32
#  include <sstream>
#else
#  include <strstrea.h>
#endif
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "CelebrationManager.h"
#include "GarbageFlavorImage.h"
#include "SignManager.h"
#include "LightManager.h"
#include "MetaState.h"
#include "SparkleManager.h"
#include "TextureLoader.h"
#include "String.h"
#include "ScoreRecordManager.h"
#include "Score.h"

GLenum Displayer::rescale_method;
bool Displayer::opengl_version_1_2;

int Displayer::state;
GLfloat Displayer::play_offset_y;

#ifndef NO_MULTITEXTURING
#  ifndef _WIN32
#  else
PFNGLACTIVETEXTUREARBPROC Displayer::glActiveTextureARB;
PFNGLCLIENTACTIVETEXTUREARBPROC Displayer::glClientActiveTextureARB;
#  endif
#endif

#ifdef DEVELOPMENT
GLint Displayer::screen_length;
#endif

void Displayer::initialize (   )
{
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(DC_WINDOW_PIXEL_STARTING_WIDTH,
   DC_WINDOW_PIXEL_STARTING_HEIGHT);
  glutCreateWindow(GC_NAME);

  state = 0;

  opengl_version_1_2 = false;
  char *string = (char *) glGetString(GL_VERSION);
  if (string[0] > '1' || (string[0] == '1' && string[2] >= '2'))
    opengl_version_1_2 = true;

  // determine rescaling method for normals
  if (opengl_version_1_2)
    rescale_method = GL_RESCALE_NORMAL;
  else
    rescale_method = GL_NORMALIZE;

#ifndef NO_MULTITEXTURING
  // check for multitexturing
  if (checkExtension("GL_ARB_multitexture")) {
    GLint n;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &n);
    if (n > 1) state |= DS_MULTITEXTURING;
  }

#  ifndef _WIN32
#  else
  // locate the extension function pointers
  if (state & DS_MULTITEXTURING) {
    glActiveTextureARB
     = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB
     = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
  }
#  endif
#endif

#ifdef DEVELOPMENT
  string = (char *) glGetString(GL_EXTENSIONS);
  cout << "OpenGL version:  " << glGetString(GL_VERSION)
   << "\nOpenGL extensions:\n  " << *string;
  for (char *p = string + 1; *p; p++) {
    if (isspace(*p)) continue;
    if (isspace(*(p - 1)))
      cout << "\n  ";
    cout << *p;
  }
  cout << endl;
#endif

  SparkleManager::initialize();
  SignManager::initialize();
  LightManager::initialize();
  GarbageFlavorImage::initialize();
  CelebrationManager::initialize();
  ScoreRecordManager::initialize();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  glEnable(GL_CLIP_PLANE_PLAY_FLOOR);
  const GLdouble play_bottom[]
   = { 0.0, 1.0, 0.0, DC_PLAY_HEIGHT * DC_CAMERA_HEIGHT_RATIO };
  glClipPlane(GL_CLIP_PLANE_PLAY_FLOOR, play_bottom);

  glEnable(GL_LIGHT0);
  const GLfloat headlight_position[]
   = { 0.0f, DC_HEADLIGHT_Y_OFFSET, DC_HEADLIGHT_Z_OFFSET, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, headlight_position);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  generateBlockDisplayList();
  generateGarbageExtras();
  generateGarbageThinMiddleList();
  generateGarbageThinCapList();
  generateGarbageThickCornerList();
  generateGarbageThickEdgeList();
  generateGarbageThickMiddleList();
  generateGarbageSmallList();
  generateSparkleList();
  generateSignList();
  generateSwapperList();
  generateExternalCandy();
  generateLevelLights();
  generateClock();
  generateMessages();

  String::readyLetterTextures();
  generateNameTexture();
  generateScoreRecord();
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-(DC_PLAY_PIXEL_OFFSET_X / DC_PLAY_PIXEL_WIDTH + 0.5f)
   * DC_PLAY_WIDTH, ((1.0f - DC_PLAY_PIXEL_OFFSET_X - DC_PLAY_PIXEL_WIDTH)
   / DC_PLAY_PIXEL_WIDTH + 0.5f) * DC_PLAY_WIDTH, -DC_PLAY_HEIGHT
   * DC_CAMERA_HEIGHT_RATIO, DC_PLAY_HEIGHT * (1.0 - DC_CAMERA_HEIGHT_RATIO),
   40.0, 65.0);
  glMatrixMode(GL_MODELVIEW);
}

void Displayer::gameStart (   )
{
  LightManager::resetHeadLight();
  ScoreRecordManager::gameStart();
}

void Displayer::gameFinish (   )
{
  CelebrationManager::gameFinish();
  ScoreRecordManager::gameFinish();
}

void Displayer::cleanUp (   )
{
  for (int n = DC_NUMBER_USE_GARBAGE_TEX; n--; )
    if (garbage_texture_data[n] != null) {
      delete [] garbage_texture_data[n];
      garbage_texture_data[n] = null;
    }
  for (int n = DC_NUMBER_MESSAGE_TEX; n--; )
    if (message_texture_data[n] != null) {
      delete [] message_texture_data[n];
      message_texture_data[n] = null;
    }

  if (player_rank_texture_data != null) {
    delete [] player_rank_texture_data;
    player_rank_texture_data = null;
  }
  if (MetaState::mode & CM_SOLO)
    for (int n = GC_SCORE_REC_LENGTH - 1; n--; )
      if (record_texture_data[n] != null) {
        delete [] record_texture_data[n];
        record_texture_data[n] = null;
      }
  if (record_texture_data != null) {
    delete [] record_texture_data;
    record_texture_data = null;
  }

  String::freeLetterTextures();
}

void Displayer::reshape ( int width, int height )
{
  if (DC_WINDOW_PIXEL_WIDTH * height / DC_WINDOW_PIXEL_HEIGHT != width) {
    height = (height + width) >> 1;
    glutReshapeWindow(DC_WINDOW_PIXEL_WIDTH * height / DC_WINDOW_PIXEL_HEIGHT,
     height);
  } else {
    glViewport(0, 0, width, height);
#ifdef DEVELOPMENT
    screen_length = width;
#endif
  }
}

void Displayer::displayMeta (   )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  // disables GL_BLEND internally
  drawExternalCandy();

  drawScoreToBeatMessage();

  glDisable(GL_TEXTURE_2D);

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);

  drawLevelLights();

  if (CelebrationManager::draw_game) {

    LightManager::timeStepInitialize();

    int hold_time_step = Game::time_step;
    Game::time_step = MetaState::final_time_step;

    calculatePlayOffset();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    LightManager::setupHeadLightMeta();

    drawBlocks();

    glEnable(GL_TEXTURE_2D);

    drawGarbage();

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    drawSwapper();

    glDisable(GL_DEPTH_TEST);

    LightManager::resetHeadLight();

    Game::time_step = hold_time_step;
  }

  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  drawMessage();

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

  drawWinRecord();

  // calls glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
  // internally
  drawCandy();

  drawScoreRecord();

  glutSwapBuffers();
}

void Displayer::displayPlay (   )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  // calls glDisable(GL_BLEND) internally
  drawExternalCandy();

  glDisable(GL_TEXTURE_2D);

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);

  drawLevelLights();

  if (!((Game::state & (GS_PAUSED | GS_SYNC_WAIT)) == GS_PAUSED)) {

    LightManager::timeStepInitialize();

    calculatePlayOffset();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    LightManager::setupHeadLightPlay();

    drawBlocks();

    glEnable(GL_TEXTURE_2D);

    drawGarbage();

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    drawSwapper();

    glDisable(GL_DEPTH_TEST);

    LightManager::resetHeadLight();
  }

  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

  drawWinRecord();

  // calls glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
  // internally
  drawCandy();

  if (!(Game::state & GS_PAUSED))
    drawCountDown();
  else
    drawMessage();
  
  glutSwapBuffers();
}

bool Displayer::checkExtension ( char *extension_name )
{
  char *extensions = (char *) glGetString(GL_EXTENSIONS);

  if (!extensions) return false;

  int length = strlen(extension_name);
  char *end = extensions + strlen(extensions);

  while (extensions < end) {
    int n = strcspn(extensions, " ");

    if (n == length && strncmp(extension_name, extensions, n) == 0)
      return true;

    extensions += n + 1;
  }

  return false;
}

#ifdef DEVELOPMENT
void Displayer::screenShot (   )
{
  static int count = 0;
  GLubyte *image = new GLubyte[4 * screen_length * screen_length];

  glReadPixels(0, 0, screen_length, screen_length, GL_RGBA, GL_UNSIGNED_BYTE,
   image);

  ostringstream file_name;
  file_name << DC_SCREEN_SHOT_FILE_NAME_BASE "_" << setw(GC_GARBAGE_TEX_NUMBER_DIGITS)
   << setfill('0') << count << ".tga" << ends;
  TextureLoader::createTGA(file_name.str(), image, screen_length, screen_length,
   TL_SCREEN_SHOT_TGA_ID);

  MESSAGE("creating " << file_name.str());

  count++;

  if (image != null) {
    delete [] image;
    image = null;
  }
}
#endif
