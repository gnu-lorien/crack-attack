/*
 * Game.h
 * Daniel Nelson - 8/24/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * Copyright (C) 2004  Andrew Sayman
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

#ifndef GAME_H
#define GAME_H

#include <climits>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/glut.h>

#ifndef _WIN32
#  include <config.h>
#else
#  include <glext.h>
#endif

#include "Mode.h"

using namespace std;

// null
#define null                             (0)

// no IEEE super pi
#define PI                               (3.14159265358979323846f)
#define SQRT_3                           (1.73205080756887729352f)

// the name
#define GC_NAME                          "Crack Attack!"
#define GC_BINARY                        "crack-attack"
#define GC_VERSION                       "1.1.11"

// startup message
#define GC_MESSAGE                       GC_NAME " v" GC_VERSION "\n"

// directory delimiter
#ifndef _WIN32
#  define GC_DD                          "/"
#else
#  define GC_DD                          "\\"
#endif

// file constants
#ifndef _WIN32
#  ifndef DATA_DIRECTORY
#    define GC_DATA_DIRECTORY            "." GC_DD "data/"
#  else
#    define GC_DATA_DIRECTORY            DATA_DIRECTORY
#  endif
#  define GC_LOCAL_DATA_DIRECTORY        GC_DD "." GC_BINARY GC_DD
#else
#  define GC_DATA_DIRECTORY              ".." GC_DD "data" GC_DD
#  define GC_LOCAL_DATA_DIRECTORY        ".." GC_DD "localdata" GC_DD
#endif
#define GC_GARBAGE_TEX_FILE_NAME_BASE    "garbage_flavor"
#define GC_GARBAGE_TEX_NUMBER_DIGITS     (3)
#define GC_NUMBER_STANDARD_GARBAGE_TEX   (6)
#define GC_GARBAGE_TEX_MAX_NUMBER        (1000)
#define GC_GARBAGE_NET_TEX_FILE_NAME     GC_GARBAGE_TEX_FILE_NAME_BASE \
                                          "_net.tga"
#define GC_GARBAGE_MY_TEX_FILE_NAME      GC_GARBAGE_TEX_FILE_NAME_BASE \
                                          "_my.tga"
#define GC_LOGO_TEX_FILE_NAME            GC_DATA_DIRECTORY "logo.tga"
#define GC_X_LOGO_TEX_FILE_NAME          GC_DATA_DIRECTORY "logo_x.tga"
#define GC_GARBAGE_LOGO_TEX_FILE_NAME    GC_DATA_DIRECTORY \
                                          GC_GARBAGE_TEX_FILE_NAME_BASE \
                                          "_logo.tga"
#define GC_GARBAGE_X_LOGO_TEX_FILE_NAME  GC_DATA_DIRECTORY \
                                          GC_GARBAGE_TEX_FILE_NAME_BASE \
                                          "_logo_x.tga"
#define GC_COUNT_DOWN_GO_TEX_FILE_NAME   GC_DATA_DIRECTORY "count_down_go.tga"
#define GC_COUNT_DOWN_1_TEX_FILE_NAME    GC_DATA_DIRECTORY "count_down_1.tga"
#define GC_COUNT_DOWN_2_TEX_FILE_NAME    GC_DATA_DIRECTORY "count_down_2.tga"
#define GC_COUNT_DOWN_3_TEX_FILE_NAME    GC_DATA_DIRECTORY "count_down_3.tga"
#define GC_ANYKEY_TEX_FILE_NAME          GC_DATA_DIRECTORY "message_anykey.tga"
#define GC_WAITING_TEX_FILE_NAME         GC_DATA_DIRECTORY "message_waiting.tga"
#define GC_PAUSED_TEX_FILE_NAME          GC_DATA_DIRECTORY "message_paused.tga"
#define GC_WINNER_TEX_FILE_NAME          GC_DATA_DIRECTORY "message_winner.tga"
#define GC_LOSER_TEX_FILE_NAME           GC_DATA_DIRECTORY "message_loser.tga"
#define GC_GAME_OVER_TEX_FILE_NAME       GC_DATA_DIRECTORY \
                                          "message_game_over.tga"
#define GC_X_REC_FILE_NAME               "score_record_x"
#define GC_REC_FILE_NAME                 "score_record"
#define GC_DEFAULT_REC_FILE_NAME         GC_DATA_DIRECTORY "default_record"

// random angle tables' size; must be power of two
#define GC_SIZE_RANDOM_ANGLE_TABLE       (256)

// games per match
#define GC_GAMES_PER_MATCH               (3)

// the play area dimensions; GC_SAFE_HEIGHT - 1 is the board height
#define GC_PLAY_WIDTH                    (6)
#define GC_PLAY_HEIGHT                   (45)
#define GC_SAFE_HEIGHT                   (13)
#define GC_GRID_SIZE                     (GC_PLAY_WIDTH * GC_PLAY_HEIGHT)

// object stores
#define GC_BLOCK_STORE_SIZE              (GC_GRID_SIZE)
#define GC_GARBAGE_STORE_SIZE            (2 * GC_PLAY_HEIGHT)
#define GC_COMBO_TABULATOR_STORE_SIZE    (8)
#define GC_GARBAGE_QUEUE_SIZE            (8)

// time steps per second
#define GC_STEPS_PER_SECOND              (50)

// milliseconds per time step
#define GC_TIME_STEP_PERIOD              (1000 / GC_STEPS_PER_SECOND)

// number of subdivisions per grid location
#define GC_STEPS_PER_GRID                (60)

// names
#define GC_PLAYER_NAME_LENGTH            (256)
#define GC_DEFAULT_PLAYER_NAME           "NamelessOne"

// velocity of falling; must be a factor of GC_STEPS_PER_GRID
#define GC_FALL_VELOCITY                 (20)

// velocity and time length of swapping
#define GC_SWAP_VELOCITY                 (10)
#define GC_SWAP_DELAY                    (GC_STEPS_PER_GRID / GC_SWAP_VELOCITY)

// velocity of creep
#define GC_CREEP_DELAY                   (1200)
#define GC_CREEP_ADVANCE_TIMER_STEP      GC_CREEP_DELAY
#define GC_CREEP_TIMER_STEP_INCREMENT    (20)
#define GC_CREEP_INITIAL_TIMER_STEP      GC_CREEP_TIMER_STEP_INCREMENT
#define GC_CREEP_MAX_TIMER_STEP          (2400)
#define GC_CREEP_ADVANCE_VELOCITY        (3)
#define GC_CREEP_INCREMENT_DELAY         (10 * GC_STEPS_PER_SECOND)

// delay between safe height violation and loss
#define GC_LOSS_DELAY                    (7 * GC_STEPS_PER_SECOND)
#define GC_LOSS_DELAY_ELIMINATION        (1 * GC_STEPS_PER_SECOND)

// time length between moves
#define GC_MOVE_DELAY                    (6)

// time length of block dying
#define GC_DYING_DELAY                   (90)

// time length of hanging
#define GC_HANG_DELAY                    (3)

// time length until popping, between popping, and after popping
#define GC_INTERNAL_POP_DELAY            (15)
#define GC_INITIAL_POP_DELAY             (50 + GC_INTERNAL_POP_DELAY)
#define GC_FINAL_POP_DELAY               (50)

// minimum length of a elimination pattern
#define GC_MIN_PATTERN_LENGTH            (3)

// chance of a creep row having a special block
#define GC_NO_SPECIAL_BLOCK_CHANCE_IN    (3)
#define GC_X_NO_SPECIAL_BLOCK_CHANCE_IN  (10)

// chance of garbage shattering to garbage
#define GC_GARBAGE_TO_GARBAGE_SHATTER    (2)

// time length before garbage falls
#define GC_AVERAGE_GARBAGE_DROP_DELAY    (300)
#define GC_SPREAD_GARBAGE_DROP_DELAY     (40)

// time length of the introduction pause; should be multiple of 3
#define GC_START_PAUSE_DELAY             (150)

// maximum height of a garbage block
#define GC_MAX_GARBAGE_HEIGHT            (11)

// score constants
#define GC_MIN_PATTERN_SCORE             (2)
#define GC_GRAY_SCORE                    (3)
#define GC_NUMBER_DIGITS                 (7)
#define GC_MIN_NUMBER_DIGITS_DISPLAYED   (4)
#define GC_MAX_SCORE_INCREMENT_DELAY     (12)
#define GC_MIN_SCORE_INCREMENT_DELAY     (1)
#define GC_SCORE_DELAY_SLOPE             (2)
#define GC_SCORE_REC_LENGTH              (30)
#define GC_SCORE_DEFAULT_TOP_SCORE       (600)
#define GC_SCORE_REC_DEFAULT_NAME        "-----"

// sine constants
#define GC_SINE_TABLE_LENGTH             (100)
#define GC_SINE_TABLE_LOWER_BOUND        (-PI / 2.0f)
#define GC_SINE_TABLE_STEP_WIDTH         (PI / (GC_SINE_TABLE_LENGTH - 1))

// initial swapper location
#define GC_INITIAL_SWAPPER_LOCATION_X    (GC_PLAY_WIDTH / 2 - 1)
#define GC_INITIAL_SWAPPER_LOCATION_Y    (4)

// extreme constants
#define GC_INVISIBLE_MAX_ALPHA           (330)
#define GC_INVISIBLE_MIN_ALPHA           (-20)
#define GC_INVISIBLE_QUICK_DECAY_RATE    (3)
#define GC_INVISIBLE_PULSE_CHANCE_IN     (30)
#define GC_INVISIBLE_PULSE_STRENGTH      (70)
#define GC_CRAZY_LONG_MODE_PERIOD        (150)
#define GC_CRAZY_SHORT_MODE_PERIOD       (50)
#define GC_MAX_WILD_NUMBER               (3)
#define GC_WILD_PERIOD                   (180)
#define GC_WILD_POLYMORPH_PERIOD         (60)
#define GC_MAX_SPECIAL_COLOR_NUMBER      (6)

// control keys
#define GC_LEFT_KEY                      ('a')
#define GC_RIGHT_KEY                     ('d')
#define GC_UP_KEY                        ('w')
#define GC_DOWN_KEY                      ('s')
#define GC_SWAP_KEY                      ('k')
#define GC_ADVANCE_KEY                   ('l')
#define GC_PAUSE_KEY                     ('p')

// other crap
#ifndef max
#  define max(a, b)                      ((a) > (b) ? (a) : (b))
#endif

// insure that our communication int is the same length on all machines
#if (INT_MAX == 2147483647)
  typedef unsigned int uint32;
#else
  typedef unsigned short uint32;
#endif

// debug
#ifndef NDEBUG
#  include <fstream>
#  ifndef _WIN32
#    define COLOR(n)                     "\33[1;" << (n) << "m"
#    define NOCOLOR                      "\33[m"
#  else
#    define COLOR(n)                     ""
#    define NOCOLOR                      ""
#  endif
#  define DOT(n)                         { cerr << COLOR(37 - n) \
                                            << "." NOCOLOR << flush; }
#  define DUMP(a)                        { cerr << COLOR(32) << "Dump: " \
                                            __FILE__ ":" << __LINE__ << ": " \
                                            NOCOLOR #a \
                                            " = " << (a) << endl; }
#  define PERIODIC_DUMP(a, b)            { if (!(Game::time_step % a)) \
                                            cerr << COLOR(32) << "Dump: " \
                                            __FILE__ ":" << __LINE__ << ": " \
                                            NOCOLOR #b \
                                            " = " << (b) << endl; }
#  define MESSAGE(a)                     { cerr << COLOR(33) << "Mesg: " \
                                            __FILE__ ":" << __LINE__ << ": " \
                                            NOCOLOR << a << endl; }
#  ifndef _WIN32
#    define MARK()                       { cerr << COLOR(35) << "Mark: " \
                                            __FILE__ ":" << __LINE__ << ": " \
                                            << __PRETTY_FUNCTION__ << NOCOLOR \
                                            << endl; }
#  else
#    define MARK()                       { cerr << COLOR(35) << "Mark: " \
                                            __FILE__ ":" << __LINE__ << ":" \
                                            NOCOLOR << endl; }
#  endif
#  define ENDL()                         { cerr << COLOR(34) << "Endl: " \
                                            __FILE__ ":" << __LINE__ << ":" \
                                            NOCOLOR << endl; }
#  define LOG(a)                         { ofstream log("log", ios::app); \
                                            log << "Log: " __FILE__ ":" \
                                            << __LINE__ << ": " << a \
                                            << endl; }
#else
#  define DOT(n)                         ((void) 0)
#  define DUMP(a)                        ((void) 0)
#  define PERIODIC_DUMP(a, b)            ((void) 0)
#  define MESSAGE(a)                     ((void) 0)
#  define MARK()                         ((void) 0)
#  define ENDL()                         ((void) 0)
#  define LOG(a)                         ((void) 0)
#endif

// fix bad Visual C++ scoping
#ifdef _WIN32
#  define for                            if (false) {   } else for
#endif

// game states

// Playing.
#define GS_NORMAL                        (1 << 0)
// Normal playing, but we're paused.
#define GS_PAUSED                        (1 << 1)
// Signals a local unpausing to opponent; used only in communication signals.
#define GS_UNPAUSED                      (1 << 2)
// Normal playing, but we're paused because we've gotten ahead of our opponent;
// most likely due to a pause.
#define GS_SYNC_WAIT                     (1 << 3)
// We've lost, but we haven't heard confirmation from opponent; play continues.
#define GS_MAY_HAVE_LOST                 (1 << 4)
// Opponent lost; we've received the message and it predates our loss; play
// continues until level lights reach minimum.
#define GS_WON                           (1 << 5)
// We've lost and opponent confirms it; play continues until level lights reach
// minimum.
#define GS_LOST                          (1 << 6)
// We've won but we can't quite yet cause we must confirm our opponent's loss.
#define GS_MUST_CONFIRM_LOSS             (1 << 7)
// We've confirmed our opponent's loss, but we have to wait a bit yet until
// he receives it.
#define GS_CONFIRMATION_HOLD             (1 << 8)
// The level lights signaled it's time to end play.  Now we must simply complete
// the current time step.
#define GS_END_PLAY                      (1 << 9)
// Used only for communication.  The match has been conceded.
#define GS_CONCESSION                    (1 << 10)

// flavor of blocks; special color blocks must be last; wild and then gray must
// be directly after normal flavors; here to prevent header entanglements;
// the appearance chance of the various flavors can be found in
// BlockManager::newCreepBlock()
#define BF_NORMAL_1                 ( 0)
#define BF_NORMAL_2                 ( 1)
#define BF_NORMAL_3                 ( 2)
#define BF_NORMAL_4                 ( 3)
#define BF_NORMAL_5                 ( 4)
#define BF_WILD                     ( 5)
#define BF_GRAY                     ( 6)
#define BF_BLACK                    ( 7)
#define BF_WHITE                    ( 8)
#define BF_SPECIAL_COLOR_1          ( 9)
#define BF_SPECIAL_COLOR_2          (10)
#define BF_SPECIAL_COLOR_3          (11)
#define BF_SPECIAL_COLOR_4          (12)
#define BF_SPECIAL_COLOR_5          (13)
#define BF_NUMBER_NORMAL            (BF_NORMAL_5 + 1)
#define BF_NUMBER                   (BF_SPECIAL_COLOR_5 + 1)
#define BF_NUMBER_SPECIAL           (BF_NUMBER - (BF_GRAY + 1))
#define BF_FINAL_GRAY_SPECIAL       (BF_WHITE)

/* static */ class Game {
public:
  static void initialize (   );
  static void gameStart (   );
  static void gameFinish (   );
  static void cleanUp (   );

  static void loss (   );
  static void lossConfirmation (   );
  static void aiPlayerLoss (   );
  static void won (   );
  static void netSignalPause (   );
  static void netSignalUnpause (   );
  static void syncPause ( int delay );
  static void concession (   );

  static void idlePlay (   );
  static void idleMeta (   );

  static inline void go (   )
  {
    previous_time = glutGet((GLenum) GLUT_ELAPSED_TIME);
  }

  static inline float sqrt ( float x )
  /*
   * An OK approximation of sqrt() on [0, 1] which is correct at the boundaries.
   * Since all I really want is the vague shape of sqrt() and perfection at 0
   * and 1, this'll work perfectly.
   *
   * Generated by minimizing int_0^1{dx (sqrt{x} - a x + (1 - a) x)^2}.
   */
  {
    return ((27.0f / 14.0f) - (13.0f / 14.0f) * x) * x;
  }

  static int state;
  static int time_step;
  static int awaking_count;
  static int dying_count;
  static int dying_count_2;

private:
  static void buttonPause (   );
  static void syncUnpause (   );

  static int previous_time;
  static int remaining_time;
  static bool button_down_pause;
  static bool step_play;
  static int sync_wait;
  static double lastframe;
};

#endif
