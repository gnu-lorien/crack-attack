/*
 * Displayer.h
 * Daniel Nelson - 8/24/0
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
 */

#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Garbage.h"
#include "CountDownManager.h"
#include "Creep.h"
#include "Spring.h"
#include "WinRecord.h"

// I can not find a version of glext.h which defines
// GL_LIGHT_MODEL_COLOR_CONTROL and its allowed values.  The
// GL_LIGHT_MODEL_COLOR_CONTROL_EXT defines from OpenGL 1.1 have the same
// values as their OpenGL 1.2 partiners.  So, here is the work around.
#ifndef _WIN32
#else
#  define GL_LIGHT_MODEL_COLOR_CONTROL       GL_LIGHT_MODEL_COLOR_CONTROL_EXT
#  define GL_SEPARATE_SPECULAR_COLOR         GL_SEPARATE_SPECULAR_COLOR_EXT
#  define GL_SINGLE_COLOR                    GL_SINGLE_COLOR_EXT
#endif

// displayer states
#define DS_MULTITEXTURING                    (1 << 0)

// clip planes
#define GL_CLIP_PLANE_PLAY_FLOOR             (GL_CLIP_PLANE0)
#define GL_CLIP_PLANE_SHATTERING             (GL_CLIP_PLANE1)

// window dimensions
#define DC_WINDOW_PIXEL_WIDTH                (1)
#define DC_WINDOW_PIXEL_HEIGHT               (1)
#define DC_WINDOW_PIXEL_ASPECT_RATIO         ((float) DC_WINDOW_PIXEL_WIDTH \
                                              / DC_WINDOW_PIXEL_HEIGHT)
#define DC_WINDOW_PIXEL_STARTING_HEIGHT      (700)
#define DC_WINDOW_PIXEL_STARTING_WIDTH       (DC_WINDOW_PIXEL_WIDTH \
                                              * DC_WINDOW_PIXEL_STARTING_HEIGHT\
                                              / DC_WINDOW_PIXEL_HEIGHT)

// play area dimensions in pixels as ratio of window size; these times the
// window dimensions must have a 2:1 ratio - like the play area
#define DC_PLAY_PIXEL_WIDTH                  (0.5f)
#define DC_PLAY_PIXEL_HEIGHT                 (1.0f)

// play area offset in pixels as ratio of window size
#define DC_PLAY_PIXEL_OFFSET_X               (0.4)
#define DC_PLAY_PIXEL_OFFSET_Y               (0.0f)

// play area dimensions in GL coordinates
#define DC_PLAY_WIDTH                        (12.0)
#define DC_PLAY_HEIGHT                       (24.0)

// camera height as a fraction of play height
#define DC_CAMERA_HEIGHT_RATIO               (0.25)

// object offset
#define DC_PLAY_OFFSET_X                     (1.0f - DC_PLAY_WIDTH / 2.0f)
#define DC_PLAY_OFFSET_Y                     (-1.0f - DC_PLAY_HEIGHT \
                                              * DC_CAMERA_HEIGHT_RATIO)
#define DC_PLAY_OFFSET_Z                     (-42.5f)
#define DC_EXTERNAL_OFFSET_Z                 (-42.5f)

// grid element size
#define DC_GRID_ELEMENT_LENGTH               (2.0f)

// headlight z offset
#define DC_HEADLIGHT_Y_OFFSET                (2.1f * DC_GRID_ELEMENT_LENGTH)
#define DC_HEADLIGHT_Z_OFFSET                (-35.0f)

// acceleration of death rotation
#define DC_DYING_ROTATE_SPEED                (0.2f)

// constants of death animation
#define DC_DYING_FLASH_TIME                  (12)
#define DC_DYING_SHRINK_MIN_SIZE             (0.1f)
#define DC_DYING_SHRINK_SPEED                ((1.0f \
                                              - DC_DYING_SHRINK_MIN_SIZE) \
                                              / (float) (GC_DYING_DELAY \
                                              - DC_DYING_FLASH_TIME))
#define DC_FLASH_COLOR_RED                   (1.0f)
#define DC_FLASH_COLOR_GREEN                 (1.0f)
#define DC_FLASH_COLOR_BLUE                  (1.0f)

// constants of shattering animation
#define DC_CLIP_SHATTER
#define DC_SHATTER_TIME                      (GC_INITIAL_POP_DELAY)
#define DC_UNSHATTER_TIME                    (GC_FINAL_POP_DELAY)
#define DC_POP_ROTATE_TIME                   (4 * GC_INTERNAL_POP_DELAY / 5)
#define DC_SHATTER_FLASH_TIME                (DC_DYING_FLASH_TIME)

// garbage lightmap constants
#define DC_GARBAGE_LIGHTMAP_LENGTH           (64)
#define DC_GARBAGE_LIGHTMAP_MIN_LUMINANCE    (0.85f)
#define DC_GARBAGE_LIGHTMAP_COORD_CONVERTER  (-1.0f / (DC_GRID_ELEMENT_LENGTH \
                                              * (float) GC_PLAY_WIDTH))
#define DC_NUMBER_GARBAGE_LIGHTMAPS          (6)

// garbage flavor image constants
#define DC_NUMBER_GARBAGE_TEX                (4)
#define DC_NUMBER_USE_GARBAGE_TEX            (4)
#define DC_GARBAGE_TEX_LENGTH                (128)
#define DC_CHANCE_USE_NET_TEX                (5)
#define DC_MIN_FLAVOR_GARBAGE_LENGTH         (4)
#define DC_CHANCE_NO_GARBAGE_FLAVOR          (8)
#define DC_GARBAGE_FLAVOR_TEX_OFFSET_Z       (0.01f)
#define DC_CHANCE_IN_NOT_USE_LOGO_FLAVOR_TEX (4)

// general sparkle constants
#define DC_SPARKLE_LENGTH                    (0.4f / 2.0f)
#define DC_SPARKLE_TEX_LENGTH                (32)

// spark constants
#define DC_MAX_SPARK_NUMBER                  (400)
#define DC_SPARK_GRAVITY                     (0.001f)
#define DC_SPARK_DRAG                        (0.001f)
#define DC_MIN_SPARK_VELOCITY                (0.02f) 
#define DC_MAX_SPARK_VELOCITY                (0.15f)
#define DC_MIN_SPARK_ANGULAR_VELOCITY        (1.0f)
#define DC_MAX_SPARK_ANGULAR_VELOCITY        (15.0f)
#define DC_MIN_SPARK_SIZE                    (0.4f)
#define DC_MEDIUM_SPARK_LIFE_TIME            (120)
#define DC_SPREAD_SPARK_LIFE_TIME            (50)
#define DC_CHANCE_LONG_SPARK_LIFE_TIME       (40)
#define DC_SPARK_FADE_TIME                   (15)
#define DC_SPARK_PULSE_TIME                  (6)

// celebration spark constants
#define DC_MIN_CSPARK_VELOCITY               (0.05f)
#define DC_SPREAD_CSPARK_VELOCITY            (0.05f)
#define DC_SOURCE_1_OFFSET_X                 (0.0f)
#define DC_SOURCE_1_OFFSET_Y                 (7.0f)
#define DC_SOURCE_1_SOLO_OFFSET_X            (0.0f)
#define DC_SOURCE_1_SOLO_OFFSET_Y            (12.0f)
#define DC_SOURCE_2_OFFSET_X                 (0.0f)
#define DC_SOURCE_2_OFFSET_Y                 (19.0f)
#define DC_SOURCE_3_OFFSET_X                 (10.0f)
#define DC_SOURCE_3_OFFSET_Y                 (7.0f)
#define DC_SOURCE_3_SOLO_OFFSET_X            (10.0f)
#define DC_SOURCE_3_SOLO_OFFSET_Y            (12.0f)
#define DC_SOURCE_4_OFFSET_X                 (10.0f)
#define DC_SOURCE_4_OFFSET_Y                 (19.0f)
#define DC_SOURCE_5_OFFSET_X                 (5.0f)
#define DC_SOURCE_5_OFFSET_Y                 (4.0f)
#define DC_SOURCE_5_SOLO_OFFSET_X            (5.0f)
#define DC_SOURCE_5_SOLO_OFFSET_Y            (3.7f)

// mote constants
#define DC_MAX_MOTE_NUMBER                   (40)
#define DC_MULTI_MOTE_FIRE_DELAY             (25)
#define DC_MOTE_COLOR_FADE_TIME              (50)
#define DC_MEDIUM_MOTE_VELOCITY              (0.2f)
#define DC_SPREAD_MOTE_VELOCITY              (0.02f)
#define DC_MEDIUM_MOTE_ANGULAR_VELOCITY      (3.0f)
#define DC_SPREAD_MOTE_ANGULAR_VELOCITY      (1.0f)
#define DC_MOTE_UPWARD_FORCE                 (0.004f)
#define DC_MOTE_CENTER_SPRING                (0.005f)
#define DC_MOTE_TWIST_SPRING                 (0.0008f)
#define DC_MOTE_DRAG                         (0.005f)

// mote types
#define DC_NUMBER_MOTE_LEVELS                (22)
#define DC_NUMBER_MOTE_TYPES                 (7)
#define DC_FIRST_SPECIAL_MOTE_COLOR          (4)
#define MT_FOUR_POINTED_STAR                 (0)
#define MT_FIVE_POINTED_STAR                 (1)
#define MT_SIX_POINTED_STAR                  (2)
#define MT_SPECIAL_STAR                      (3)
#define MT_MULTIPLIER_ONE_STAR               (4)
#define MT_MULTIPLIER_TWO_STAR               (5)
#define MT_MULTIPLIER_THREE_STAR             (6)

// mote light constants
#define DC_NUMBER_EXTRA_LIGHTS               (7)
#define DC_EXTRA_LIGHT_BASE                  (GL_LIGHT1)
#define DC_MOTE_LIGHT_RANGE                  (DC_GRID_ELEMENT_LENGTH \
                                              + DC_GRID_ELEMENT_LENGTH \
                                              * 1.414214f / 2.0f)
#define DC_MAX_MOTE_LIGHT_BRIGHTNESS         (0.4f)
#define DC_MOTE_LIGHT_OFFSET_Z               (3.0f)
#define DC_MOTE_LIGHT_ATTENUATION            (0.1f)

// sign constants
#define DC_MAX_SIGN_NUMBER                   (25)
#define DC_SIGN_LENGTH                       (0.33f * DC_GRID_ELEMENT_LENGTH)
#define DC_SIGN_OFFSET_X                     (-0.5f * DC_GRID_ELEMENT_LENGTH)
#define DC_SIGN_OFFSET_Y                     (0.5f * DC_GRID_ELEMENT_LENGTH)
#define DC_SIGN_OFFSET_SPREAD                (0.1f * DC_GRID_ELEMENT_LENGTH)
#define DC_SIGN_HOLD_TIME                    (100)
#define DC_SIGN_FADE_TIME                    (200)
#define DC_SIGN_LIFE_TIME                    (DC_SIGN_HOLD_TIME \
                                              + DC_SIGN_FADE_TIME)
#define DC_SIGN_TERMINAL_VELOCITY            (DC_GRID_ELEMENT_LENGTH / 500.0f)
#define DC_SIGN_ACCELERATION_TIME            (DC_SIGN_HOLD_TIME)
#define DC_SIGN_ACCELERATION                 (DC_SIGN_TERMINAL_VELOCITY \
                                              / (float) \
                                              DC_SIGN_ACCELERATION_TIME)
#define DC_SIGN_ALPHA                        (1.0f)
#define DC_FINAL_INFLATE_SIZE                (5.0f)

// sign texture constants
#define DC_NUMBER_SMALL_SIGN_SUBTEXTURES     (6)
#define DC_SIGN_SMALL_TEX_GRID_LENGTH_S      (4)
#define DC_SIGN_SMALL_TEX_GRID_LENGTH_T      (2)
#define DC_SIGN_SMALL_SUBTEX_LENGTH_S        (32)
#define DC_SIGN_SMALL_SUBTEX_LENGTH_T        (32)
#define DC_SIGN_SMALL_TEX_LENGTH_S           (DC_SIGN_SMALL_SUBTEX_LENGTH_S \
                                              * DC_SIGN_SMALL_TEX_GRID_LENGTH_S)
#define DC_SIGN_SMALL_TEX_LENGTH_T           (DC_SIGN_SMALL_SUBTEX_LENGTH_T \
                                              * DC_SIGN_SMALL_TEX_GRID_LENGTH_T)
#define DC_SIGN_SMALL_SUBTEX_COORD_S         (1.0f / (float) \
                                              DC_SIGN_SMALL_TEX_GRID_LENGTH_S)
#define DC_SIGN_SMALL_SUBTEX_COORD_T         (1.0f / (float) \
                                              DC_SIGN_SMALL_TEX_GRID_LENGTH_T)
#define DC_NUMBER_LARGE_SIGN_SUBTEXTURES     (15)
#define DC_SIGN_LARGE_TEX_GRID_LENGTH_S      (4)
#define DC_SIGN_LARGE_TEX_GRID_LENGTH_T      (4)
#define DC_SIGN_LARGE_SUBTEX_LENGTH_S        (32)
#define DC_SIGN_LARGE_SUBTEX_LENGTH_T        (64)
#define DC_SIGN_LARGE_TEX_LENGTH_S           (DC_SIGN_LARGE_SUBTEX_LENGTH_S \
                                              * DC_SIGN_LARGE_TEX_GRID_LENGTH_S)
#define DC_SIGN_LARGE_TEX_LENGTH_T           (DC_SIGN_LARGE_SUBTEX_LENGTH_T \
                                              * DC_SIGN_LARGE_TEX_GRID_LENGTH_T)
#define DC_SIGN_LARGE_SUBTEX_COORD_S         (1.0f / (float) \
                                              DC_SIGN_LARGE_TEX_GRID_LENGTH_S)
#define DC_SIGN_LARGE_SUBTEX_COORD_T         (1.0f / (float) \
                                              DC_SIGN_LARGE_TEX_GRID_LENGTH_T)

// swapper constants
#define DC_SWAPPER_GRAB_LENGTH               (0.2f)

// external candy constants
#define DC_LEFT_EXTERNAL_CENTER              (-10.5f)

// logo constants
#define DC_LOGO_TEX_LENGTH                   (256)
#define DC_LOGO_OFFSET_Y                     (13.5f)
#define DC_LOGO_LENGTH                       (7.2f)

// level lights constants
#define DC_LEVEL_LIGHT_FADE_TIME             (150)
#define DC_LEVEL_LIGHT_IMPACT_FLASH_TIME     (20)
#define DC_LEVEL_LIGHT_FLASH_INFLECTION      (0.9f)
#define DC_LEVEL_LIGHT_DEATH_FLASH_TIME      (12)
#define DC_LEVEL_LIGHT_RED                   (0.7f)
#define DC_LEVEL_LIGHT_BLUE                  (0.7f)
#define DC_LEVEL_LIGHT_LOCAL_OFFSET_X        (7.3f)
#define DC_LEVEL_LIGHT_OPPONENT_OFFSET_X     (-15.0f)

// message constants
#define DC_NUMBER_MESSAGE_TEX                (10)
#define DC_COUNT_DOWN_GO_INFLECTION          (0.8f)
#define DC_COUNT_DOWN_HEIGHT_RATIO           (0.4f)
#define DC_COUNT_DOWN_MAX_SCALE              (7.0f)
#define DC_MESSAGE_SCALE                     (6.0f)
#define DC_SYNC_WAIT_LIGHT_LEVEL             (0.75f)
#define DC_MESSAGE_PULSE_PERIOD              (320)

// clock constants 
#define DC_CLOCK_TEX_LENGTH                  (32)
#define DC_CLOCK_OFFSET_Y                    (-2.5f)
#define DC_CLOCK_DIGIT_LENGTH                (0.6f)
#define DC_CLOCK_DIGIT_OFFSET                (1.0f)
#define DC_CLOCK_CENTER_OFFSET               (0.7f)

// celebration constants
#define DC_CELEBRATION_FADE_TIME             (200)
#define DC_CELEBRATION_TIME                  (225)
#define DC_CSPARK_SOURCE_NUMBER              (5)
#define DC_CSPARK_COLOR_NUMBER               (5)
#define DC_CSPARK_STARTING_RATE              (270)
#define DC_CSPARK_FULL_RATE                  (600)
#define DC_CSPARK_LOW_RATE                   (150)
#define DC_CSPARK_QUICK_RATE_DROP            (3)
#define DC_CSPARK_BOOST_CHANCE_IN            (40)
#define DC_CSPARK_RATE_BOOST                 (90)
#define DC_CSPARK_COLOR_CHANGE_CHANCE_IN     (5)
#define DC_STARTING_WIN_SCALE                (12.0f)
#define DC_STARTING_LOSS_HEIGHT              (9.0f * DC_GRID_ELEMENT_LENGTH)
#define DC_STARTING_BOUNCE_COUNT             (6)
#define DC_FINAL_BOUNCE_COUNT                (2)
#define DC_WIN_FADE_TIME                     (50)
#define DC_WIN_FLASH_1_TIME                  (8)
#define DC_WIN_FLASH_2_TIME                  (12)
#define DC_WIN_FLASH_1_CHANCE_IN             (3)
#define DC_WIN_FLASH_2_CHANCE_IN             (6)
#define DC_LOSS_GRAVITY                      (0.01f)
#define DC_LOSS_DRAG                         (0.005f)
#define DC_LOSS_MIN_VELOCITY                 (10.0f * DC_LOSS_GRAVITY)
#define DC_LOSS_BOUNCE_ELASTICITY            (0.5f)
#define DC_LOSS_END_BOUNCE_ELASTICITY        (0.1f)
#define DC_WIN_MESSAGE_RED                   (1.0f)
#define DC_WIN_MESSAGE_GREEN                 (0.7f)
#define DC_WIN_MESSAGE_BLUE                  (0.9f)
#define DC_LOSS_MESSAGE_RED                  (0.7f)
#define DC_LOSS_MESSAGE_GREEN                (0.2f)
#define DC_LOSS_MESSAGE_BLUE                 (0.9f)
#define DC_GAME_OVER_SCALE_RATIO             (75.0f / 64.0f)

// star constants
#define DC_STAR_DISPLACEMENT                 (2.0f)
#define DC_STAR_OFFSET_X                     (DC_LEFT_EXTERNAL_CENTER \
                                              - DC_STAR_DISPLACEMENT \
                                              * (GC_GAMES_PER_MATCH - 1) / 2.0f)
#define DC_STAR_OFFSET_Y                     (0.5f)
#define DC_STAR_PLAY_ANGULAR_VELOCITY        (1.0f)
#define DC_STAR_WIN_MIN_ANGULAR_DEVIATION    (180.0f)
#define DC_STAR_WIN_SPREAD_ANGULAR_DEVIATION (180.0f)
#define DC_STAR_WIN_OFFSET_X                 (0.0f)
#define DC_STAR_WIN_OFFSET_Y                 (13.0f)
#define DC_STAR_WIN_SOLO_OFFSET_Y            (17.5f)
#define DC_STAR_WIN_PRESET_1_VELOCITY_X      (0.5f)
#define DC_STAR_WIN_PRESET_1_VELOCITY_Y      (-0.25f)
#define DC_STAR_WIN_PRESET_2_VELOCITY_X      (-0.3f)
#define DC_STAR_WIN_PRESET_2_VELOCITY_Y      (0.35f)
#define DC_STAR_WIN_MIN_VELOCITY             (0.2f)
#define DC_STAR_WIN_SPREAD_VELOCITY          (0.2f)
#define DC_STAR_SIZE_EQUILIBRIUM             (5.0f)
#define DC_STAR_WIN_SIZE_SPRING              (0.1f)
#define DC_STAR_WIN_SIZE_DRAG                (0.05f)
#define DC_STAR_WIN_SIZE_EPSILON             (0.00005f)
#define DC_STAR_WIN_SIZE_PULSE_CHANCE_IN     (100)
#define DC_STAR_WIN_SIZE_PULSE_VELOCITY      (1.0f)
#define DC_STAR_WIN_ANGULAR_SPRING           (0.0001f)
#define DC_STAR_WIN_KICK_DELAY               (100)
#define DC_STAR_WIN_SPRING                   (0.0025f)
#define DC_STAR_WIN_DRAG                     (0.04f)
#define DC_STAR_WIN_EPSILON                  (0.01f)
#define DC_STAR_WIN_VELOCITY_EPSILON         (0.01f)
#define DC_STAR_WIN_RED                      (0.85f)
#define DC_STAR_WIN_GREEN                    (0.85f)
#define DC_STAR_WIN_BLUE                     (0.0f)
#define DC_STAR_UNPLAYED_RED                 (0.4f)
#define DC_STAR_UNPLAYED_GREEN               (0.4f)
#define DC_STAR_UNPLAYED_BLUE                (0.7f)
#define DC_STAR_LOST_ALPHA                   (0.4f)
#define DC_SOLO_STAR_ID                      (1)

// name constants
#define DC_NAME_OFFSET_Y                     (7.0f)
#define DC_NAME_SCALE                        (4.4f)
#define DC_LETTER_TEX_LENGTH                 (32)
#define DC_NAME_TEX_LENGTH_S                 (256)
#define DC_NAME_TEX_LENGTH_T                 (128)
#define DC_FONT_NUMBER                       (1)
#define DC_FONT_COLOR_NUMBER                 (10)
#define DC_LETTER_NUMBER                     (86)
#define DC_BACK_SPACE_WIDTH                  (5)
#define DC_SPACE_WIDTH                       (10)
#define DC_DEFAULT_FONT_COLOR                (0)
#define DC_VS_STRING                         "vs"

// xtreme constants
#define DC_GLEAM_PERIOD                      (64)
#define DC_GLEAM_ROTATION_RATE               (1.0f / 8.0f)
#define DC_GLEAM_ROTATION_AXIS_X             (1.0f / 3.741657f)
#define DC_GLEAM_ROTATION_AXIS_Y             (2.0f / 3.741657f)
#define DC_GLEAM_ROTATION_AXIS_Z             (3.0f / 3.741657f)

// score record constants
#define DC_SCORE_TO_BEAT_MESSAGE             "Score to Beat:"
#define DC_SCORE_TO_BEAT_SCALE               (4.9f)
#define DC_SCORE_TO_BEAT_TEX_LENGTH_S        (256)
#define DC_SCORE_TO_BEAT_TEX_LENGTH_T        (128)
#define DC_SCORE_REC_TEX_LENGTH_S            (512)
#define DC_SCORE_REC_RANK_STRING_TEX_WIDTH   (45)
#define DC_SCORE_REC_RANK_TEX_MARGIN         (85)
#define DC_SCORE_REC_RANK_MIN_SPACE          DC_SPACE_WIDTH
#define DC_SCORE_REC_NUMBER_DRAW             (7)
#define DC_SCORE_REC_START_VELOCITY          (0.6f)
#define DC_SCORE_REC_RANK_HEIGHT             (3.4f)
#define DC_SCORE_REC_SPRING                  (0.01f)
#define DC_SCORE_REC_SPRING_DRAG             (0.05f)
#define DC_SCORE_REC_EDGE_BOUNCE_ELASTICITY  (0.8f)
#define DC_SCORE_REC_VELOCITY_CUTOFF         (0.001f)
#define DC_SCORE_REC_OFFSET_CUTOFF           (0.001f)
#define DC_SCORE_REC_CONTROL_VELOCITY_CUTOFF (0.1f)
#define DC_SCORE_REC_CONTROL_OFFSET_CUTOFF   (0.1f)
#define DC_SCORE_REC_BOUNCE_VELOCITY_CUTOFF  (0.01f)
#define DC_SCORE_REC_CONTROL_VELOCITY        (0.1f)
#define DC_SCORE_REC_CONTROL_DRAG            (0.1f)
#define DC_SCORE_REC_CIRCLE_SCALE            (6.0f)
#define DC_SCORE_REC_Y_OFFSET                (8.0f)
#define DC_SCORE_REC_SCALE_X                 (9.8f)
#define DC_SCORE_REC_SCALE_Y                 (4.78f)
#define DC_SCORE_REC_RANK_DRAW_HEIGHT        (2.0f)

// losebar constants
#define DC_LOSEBAR_OFFSET_Y                  (3.0f)
#define DC_LOSEBAR_FADE_TIME                 (20)
#define DC_LOSEBAR_TEX_LENGTH_S              (128)
#define DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S    (128 - 2)
#define DC_LOSEBAR_TEX_LENGTH_T              (16)
#define DC_LOSEBAR_LENGTH                    (7.0f)
#define DC_LOSEBAR_HEIGHT                    (DC_LOSEBAR_LENGTH \
                                              * DC_LOSEBAR_TEX_LENGTH_T \
                                              / \
                                              DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S)
#define DC_LOSEBAR_FADE_LENGTH               (2.0f)
#define DC_LOSEBAR_FADE_TEX_LENGTH           (DC_LOSEBAR_FADE_LENGTH \
                                              / (float) DC_LOSEBAR_LENGTH)
#define DC_LOSEBAR_ANTIALIAS_LENGTH          (4)
#define DC_LOSEBAR_END_RATIO                 (0.5f * DC_LOSEBAR_TEX_LENGTH_T \
                                              / \
                                              DC_LOSEBAR_TEX_EFFECTIVE_LENGTH_S)
#define DC_LOSEBAR_LIGHT_VECTOR_X            (-1.0f / SQRT_3)
#define DC_LOSEBAR_LIGHT_VECTOR_Y            (1.0f / SQRT_3)
#define DC_LOSEBAR_LIGHT_VECTOR_Z            (1.0f / SQRT_3)
#define DC_LOSEBAR_LIGHT_SPECULAR_POWER      (0.45f)
#define DC_LOSEBAR_LIGHT_AMBIENT_RATIO       (0.5f)
#define DC_LOSEBAR_INACTIVE_RED              (0.0f)
#define DC_LOSEBAR_INACTIVE_GREEN            (0.0f)
#define DC_LOSEBAR_INACTIVE_BLUE             (1.0f)
#define DC_LOSEBAR_LOW_ALERT_RED             (0.8f)
#define DC_LOSEBAR_LOW_ALERT_GREEN           (0.0f)
#define DC_LOSEBAR_LOW_ALERT_BLUE            (0.8f)
#define DC_LOSEBAR_HIGH_ALERT_RED            (1.0f)
#define DC_LOSEBAR_HIGH_ALERT_GREEN          (0.0f)
#define DC_LOSEBAR_HIGH_ALERT_BLUE           (0.0f)

// screen shot constants
#define DC_SCREEN_SHOT_FILE_NAME_BASE        "screen_shot"

class Block;
class Sign;

/* static */ class Displayer {
public:
  static void initialize (   );
  static void gameStart (   );
  static void gameFinish (   );
  static void cleanUp (   );

  static void reshape ( int width, int height );
  static void displayPlay (   );
  static void displayMeta (   );

  static void readyCountDownDisplay ( int state );
  static void nextCountDownDisplay ( int state );
  static void freeCountDownDisplay (   );
  static void readyMessage ( int message );
  static void nextMessage ( int message );
  static void freeMessage (   );
  static void rerankScoreRecord (   );
  static void generateScoreRankTexture ( int rank, int score, const char *name,
   GLubyte *texture );

#ifdef DEVELOPMENT
  static void screenShot (   );
#endif

  static GLfloat play_offset_y;

  static int message_shape[DC_NUMBER_MESSAGE_TEX];

  static const GLfloat mote_light_colors[7][3];

  static GLuint garbage_texture;
  static GLubyte *garbage_texture_data[DC_NUMBER_USE_GARBAGE_TEX];

  static GLuint score_to_beat_texture;
  static GLuint record_textures[DC_SCORE_REC_NUMBER_DRAW];
  static GLubyte **record_texture_data;
  static GLubyte *player_rank_texture_data;

private:
  static void drawBlocks (   );
  static void drawGarbage (   );
  static void drawCandy (   );
  static void drawSwapper (   );
  static void drawExternalCandy (   );
  static void drawLevelLights (   );
  static void drawMessage (   );
  static void drawClock (   );
  static void drawWinRecord (   );

  static void drawCountDown_inline_split_ (   );
  static inline void drawCountDown (   )
  {
    if (CountDownManager::state == -1) return;
    if (Game::state & GS_PAUSED) return;
    drawCountDown_inline_split_();
  }

  static void drawScoreToBeatMessage_inline_split_ (   );
  static inline void drawScoreToBeatMessage (   )
  {
    if (!(MetaState::mode & CM_SOLO)) return;
    if (!(MetaState::state & MS_BOTH_KEY_WAIT)) return;
    drawScoreToBeatMessage_inline_split_();
  }

  static void drawScoreRecord_inline_split_ (   );
  static inline void drawScoreRecord (   )
  {
    if (!(MetaState::mode & CM_SOLO)) return;
    if (!(MetaState::state & MS_GAME_OVER_KEY_WAIT)) return;
    if (!WinRecord::won) return;
    if (Game::time_step < DC_WIN_FADE_TIME) return;
    drawScoreRecord_inline_split_();
  }

  static void drawBlock ( Block &block, bool special );
  static void drawSign ( Sign &sign, int texture );
  static void drawTriangle ( float a_x, float a_y, float a_z,
   float b_x, float b_y, float b_z, float c_x, float c_y, float c_z );
  static void drawDigit ( GLfloat alpha );
  static void drawLeftBar ( GLfloat b, GLfloat t, GLfloat color[3] );
  static void drawRightBar ( GLfloat b, GLfloat t, GLfloat color[3] );
  static void drawCenterBar ( GLfloat b, GLfloat t, GLfloat color1[3],
   GLfloat color2[3] );
  static void drawFullBar ( GLfloat color[3] );
  static void copyRecordSubTexture ( GLubyte *texture, GLubyte *subtexture,
   int subtex_width, int s_location, int copy_width );

  static void setWildBlockColor ( Block &block, float flash );

  static inline void calculatePlayOffset (   )
  {
    play_offset_y = DC_PLAY_OFFSET_Y
     + Creep::creep * (DC_GRID_ELEMENT_LENGTH / (float) GC_STEPS_PER_GRID)
     + Spring::y;
  }

  static bool checkExtension ( char *extension_name );

  static void generateBlockDisplayList (   );
  static void generateGarbageExtras (   );
  static void generateGarbageThinCapList (   );
  static void generateGarbageThinMiddleList (   );
  static void generateGarbageThickCornerList (   );
  static void generateGarbageThickEdgeList (   );
  static void generateGarbageThickMiddleList (   );
  static void generateGarbageSmallList (   );
  static void generateSparkleList (   );
  static void generateSignList (   );
  static void generateSwapperList (   );
  static void generateExternalCandy (   );
  static void generateLevelLights (   );
  static void generateMessages (   );
  static void generateClock (   );
  static void generateNameTexture (   );
  static void generateScoreRecord (   );

  static GLuint block_list;
  static GLuint small_block_list;
  static GLuint special_block_list;
  static GLuint garbage_thin_cap_list;
  static GLuint garbage_thin_middle_list;
  static GLuint garbage_thick_corner_list;
  static GLuint garbage_thick_edge_list;
  static GLuint garbage_thick_middle_list;
  static GLuint garbage_small_list;
  static GLuint garbage_flavor_list;
  static GLuint sparkle_list;
  static GLuint sign_small_list;
  static GLuint sign_large_list;
  static GLuint swapper_list;
  static GLuint logo_list;
  static GLuint level_light_list;
  static GLuint message_1x1_list;
  static GLuint message_2x1_list;
  static GLuint message_4x1_list;
  static GLuint message_16x1_list;

  static GLuint special_block_lightmap;
  static GLuint garbage_lightmap;
  static GLuint mote_textures[DC_NUMBER_MOTE_TYPES];
  static GLuint spark_texture;
  static GLuint sign_small_texture;
  static GLuint sign_large_texture;
  static GLuint logo_texture;
  static GLuint message_texture;
  static GLuint clock_digit_textures[11];
  static GLuint name_texture;
  static GLuint losebar_texture;

  static int state;

  static bool opengl_version_1_2;
  static GLenum rescale_method;

  static GLsizei message_width[DC_NUMBER_MESSAGE_TEX];
  static GLsizei message_height[DC_NUMBER_MESSAGE_TEX];
  static GLfloat message_alpha[DC_MESSAGE_PULSE_PERIOD];

  static GLubyte *message_texture_data[DC_NUMBER_MESSAGE_TEX];

  static const GLfloat block_colors[BF_NUMBER][3];
  static const GLfloat garbage_colors[GF_NUMBER][3];

#ifndef NO_MULTITEXTURING
#  ifndef _WIN32
#  else
  static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
  static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
#  endif
#endif

#ifdef DEVELOPMENT 
  static GLint screen_length;
#endif
};

#endif
