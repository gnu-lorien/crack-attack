/*
 * LightManager.cxx
 * Daniel Nelson - 9/13/0
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
 * Handles turning on and off the required lights during drawing.
 */

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "LightManager.h"
#include "SparkleManager.h"
#include "CountDownManager.h"
#include "CelebrationManager.h"

// assumed to all be 1.0f in setupHeadLight()
const GLfloat headlight_normal_color[]
 = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat headlight_color[]
 = { 0.0f, 0.0f, 0.0f, 1.0f };

Light LightManager::lights[DC_NUMBER_EXTRA_LIGHTS];
int LightManager::last_associated;
bool LightManager::headlight_normal;

void LightManager::initialize (   )
{
  for (int n = DC_NUMBER_EXTRA_LIGHTS; n--; ) {
    lights[n].location[2] = DC_PLAY_OFFSET_Z + DC_MOTE_LIGHT_OFFSET_Z;
    lights[n].location[3] = 1.0f;
    lights[n].enabled = false;
  }

  headlight_normal = false;
}

void LightManager::setupHeadLightPlay_inline_split_ (   )
{
  GLfloat headlight_level;

  headlight_normal = false;

  if (!(Game::state & GS_SYNC_WAIT))
    headlight_level = 1.0f;
  else
    headlight_level = DC_SYNC_WAIT_LIGHT_LEVEL;

  if (CountDownManager::state != -1) {
    float fade = (GC_START_PAUSE_DELAY - CountDownManager::start_pause_alarm)
     * (1.0f / (float) GC_START_PAUSE_DELAY);
    headlight_level *= Game::sqrt(fade);
  }

  headlight_color[0] = headlight_color[1] = headlight_color[2] = 
   headlight_level;

  if (X::crazyLights())
    X::modifyHeadlightColor(headlight_color);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_color);
}

void LightManager::setupHeadLightMeta (   )
/*
 * Setup the headlight for the game end celebration fade out.
 */
{
  headlight_color[0] = headlight_color[1] = headlight_color[2]
   = CelebrationManager::light_level;

  glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_color);
  headlight_normal = false;
}

void LightManager::resetHeadLight_inline_split_ (   )
/*
 * If the headlight is not normal, reset it to standard values.
 */
{
  glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_normal_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_normal_color);
  headlight_normal = true;
}

void LightManager::setupBlockLights ( float x, float y )
/*
 * Here's an out line of the algorithm.  We loop through the motes.  If a 
 * mote is in range, we check to see if it has an associated light.  If so,
 * we make sure the light is enabled.  If not, we find a light to associate
 * with the mote.  If the mote is out of range, we disable the light but we
 * don't disassociate.  This way, if we have space, we can use a minimum of
 * light color and position reasignments.
 *
 * To find a light to associate with a mote, first we look for an unassociated
 * light.  If there are none, we look for a disabled light.  If there are none,
 * we disable a currently enabled light and use it.
 */
{
  for (int n = DC_NUMBER_EXTRA_LIGHTS; n--; )
    lights[n].to_be_enabled = false;

  int c = SparkleManager::mote_count;
  for (int m = 0; c; m++)
    if (SparkleManager::motes[m].active) {
      Mote &mote = SparkleManager::motes[m];
      c--;

      float r = (x - mote.x) * (x - mote.x) + (y - mote.y) * (y - mote.y);

      // if the mote's light needs to be on for this draw; note that range is
      // not the light range but the range to the center of block any block
      // effected
      if (r < DC_MOTE_LIGHT_RANGE * DC_MOTE_LIGHT_RANGE) {

        // if this mote has been previously associated with a light and that
        // light's association still matches
        if (mote.associated_light != -1
         && lights[mote.associated_light].association == m) {

          // make sure the light is enabled
          lights[mote.associated_light].to_be_enabled = true;
          configureBlockBrightness(lights[mote.associated_light],
           mote.associated_light, mote, r);

        // if this mote has no active association; we must try to find an
        // unassociated light
        } else {

          // find an unassociated light
          int a = -1;
          int l = last_associated;
          do {
            if (++last_associated == DC_NUMBER_EXTRA_LIGHTS)
              last_associated = 0;

            if (lights[last_associated].association == -1) {
              a = last_associated;
              break;
            }
          } while (last_associated != l);

          // if we found one
          if (a != -1) {

            // associate
            associateLight(lights[a], a, mote, m);

            // enable
            lights[a].to_be_enabled = true;
            configureBlockBrightness(lights[a], a, mote, r);

          // if we couldn't find an unassociated light
          } else {
            
            // find an associated but disabled light
            l = last_associated;
            do {
              if (++last_associated == DC_NUMBER_EXTRA_LIGHTS)
                last_associated = 0;

              if (!lights[last_associated].to_be_enabled) {
                a = last_associated;
                break;
              }
            } while (last_associated != l);

            // if we found none
            if (a != -1) {

              // associate
              associateLight(lights[a], a, mote, m);

              // enable
              lights[a].to_be_enabled = true;
              configureBlockBrightness(lights[a], a, mote, r);

            // if we couldn't find a disabled light
            } else

              // we have no choice but to steal an enabled one or give up
              break;
          }
        }
      }
    }

  for (int n = DC_NUMBER_EXTRA_LIGHTS; n--; )
    if (lights[n].to_be_enabled) {
      if (!lights[n].enabled) {
        glEnable((GLenum) (DC_EXTRA_LIGHT_BASE + n));
        lights[n].enabled = true;
      }
    } else {
      if (lights[n].enabled) {
        glDisable((GLenum) (DC_EXTRA_LIGHT_BASE + n));
        lights[n].enabled = false;
      }
    }
}

void LightManager::setupGarbageLights ( float x, float y, int h, int w )
/*
 * We only hand dim the light for motes not in the garbage.  Plus, we have to
 * use an attenuated light source.
 */
{
  for (int n = DC_NUMBER_EXTRA_LIGHTS; n--; )
    lights[n].to_be_enabled = false;

  int c = SparkleManager::mote_count;
  for (int m = 0; c; m++)
    if (SparkleManager::motes[m].active) {
      Mote &mote = SparkleManager::motes[m];
      c--;

      float x_high = x + (w - 1) * DC_GRID_ELEMENT_LENGTH;
      float y_high = y + (h - 1) * DC_GRID_ELEMENT_LENGTH;

      float r = 0.0f;

      if (mote.x < x)
        r += (x - mote.x) * (x - mote.x);
      else if (mote.x > x_high)
        r += (x_high - mote.x) * (x_high - mote.x);
      if (mote.y < y)
        r += (y - mote.y) * (y - mote.y);
      else if (mote.y > y_high)
        r += (y_high - mote.y) * (y_high - mote.y);

      // if the mote's light needs to be on for this draw
      if (r < DC_MOTE_LIGHT_RANGE * DC_MOTE_LIGHT_RANGE) {

        // if this mote has been previously associated with a light and that
        // light's association still matches
        if (mote.associated_light != -1
         && lights[mote.associated_light].association == m) {

          // make sure the light is enabled
          lights[mote.associated_light].to_be_enabled = true;
          configureGarbageBrightness(lights[mote.associated_light],
           mote.associated_light, mote, r);

        // if this mote has no active association; we must try to find an
        // unassociated light
        } else {

          // find an unassociated light
          int a = -1;
          int l = last_associated;
          do {
            if (++last_associated == DC_NUMBER_EXTRA_LIGHTS)
              last_associated = 0;

            if (lights[last_associated].association == -1) {
              a = last_associated;
              break;
            }
          } while (last_associated != l);

          // if we found one
          if (a != -1) {

            // associate
            associateLight(lights[a], a, mote, m);

            // enable
            lights[a].to_be_enabled = true;
            configureGarbageBrightness(lights[a], a, mote, r);

          // if we couldn't find an unassociated light
          } else {
            
            // find an associated but disabled light
            l = last_associated;
            do {
              if (++last_associated == DC_NUMBER_EXTRA_LIGHTS)
                last_associated = 0;

              if (!lights[last_associated].to_be_enabled) {
                a = last_associated;
                break;
              }
            } while (last_associated != l);

            // if we found none
            if (a != -1) {

              // associate
              associateLight(lights[a], a, mote, m);

              // enable
              lights[a].to_be_enabled = true;
              configureGarbageBrightness(lights[a], a, mote, r);

            // if we couldn't find a disabled light
            } else

              // we have no choice but to steal an enabled one or give up
              break;
          }
        }
      }
    }

  for (int n = DC_NUMBER_EXTRA_LIGHTS; n--; )
    if (lights[n].to_be_enabled) {
      if (!lights[n].enabled) {
        glEnable((GLenum) (DC_EXTRA_LIGHT_BASE + n));
        if (!lights[n].attenuated)
          attenuateLight(lights[n], n);
        lights[n].enabled = true;
      }
    } else {
      if (lights[n].enabled) {
        glDisable((GLenum) (DC_EXTRA_LIGHT_BASE + n));
        lights[n].enabled = false;
      }
    }
}
