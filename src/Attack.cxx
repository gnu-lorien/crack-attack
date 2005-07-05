/*
 * Attack.cxx
 * Daniel Nelson - 8/29/0
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
     
#include <cstring>
#include <cctype>
#include <sys/stat.h>

#ifndef _WIN32
#  include <pwd.h>
#else
#  ifndef __MINGW32__
#    include <direct.h>
#    include <glext.h>
#  endif
#endif

using namespace std;

#include "TextureLoader.h"
#include "Attack.h"
#include "Communicator.h"
#include "MetaState.h"
#include "Random.h"

#ifdef AUDIO_ENABLED
#include "Sound.h"
#include "Music.h"
#endif

#ifdef WANT_GTK
#include "gtk-gui/gui_main.h"
#endif

#define GC_HOST_NAME_SIZE (256)

/*
 * Documentation
 *   html tables don't work right in explorer
 *   man-page bug
 *
 * Issues and Watches
 *   slow if left sitting a long time before game start
 *   central server for online game setup
 *   remove dying_count_2
 *   find and use correct GL_LIGHT_MODEL_COLOR_CONTROL defines
 */

int nosound = 0;

int main ( int argc, char **argv )
{
  setupLocalDataDirectory();
#ifdef WANT_GTK
  if (argc <= 1) return gui_main(argc, argv);
#endif
  char player_name[GC_PLAYER_NAME_LENGTH];
  char host_name[GC_HOST_NAME_SIZE];
  int port;
  int mode = 0;
  int height = -1, width = -1;
  
  player_name[0] = '\0';
	glutInit(&argc, argv);
  parseCommandLine(argc, argv, mode, port, host_name, player_name, height, width);
  run_crack_attack(mode, port, host_name, player_name, height, width);

  return 0;
}

inline void usage (   )
{
  cerr << "Usage: " GC_BINARY " --server [PORT] [--low] [--extreme] [--wait] "
   "[--name 'NAME']\n"
   "        <or>\n"
   "       " GC_BINARY " SERVER PORT [[--really] --low] [--name 'NAME']\n"
   "        <or>\n"
   "       " GC_BINARY " --solo [[--really] --low] [-X] [--name 'NAME']\n"
   "        <or in short>\n"
   "       " GC_BINARY " -1 [[-r] -l] [-X] [-n 'NAME']\n"
   "        <or with a computer opponent>\n"
   "       " GC_BINARY " --solo [--hard] [--easy] [--medium] [[--really] --low]"
   " [--name 'NAME']\n"
   "        <etc...>"
   << endl;
  exit(1);
}

void run_crack_attack (
    int mode, 
    int port, 
    char *host_name, 
    char *player_name,
    int width,
    int height) {
  if (!player_name) {
    cerr << "Player name not properly allocated" << endl;
    return;
  }

  if (player_name[0] == '\0') {
#ifndef _WIN32
    struct passwd *uinfo = getpwuid(getuid());
    if (uinfo) {
      strncpy(player_name, uinfo->pw_name, GC_PLAYER_NAME_LENGTH);
      for (int n = strlen(player_name); n--; )
        player_name[n] = toupper(player_name[n]);
    } else
      strncpy(player_name, GC_DEFAULT_PLAYER_NAME, GC_PLAYER_NAME_LENGTH);
#else
    strncpy(player_name, GC_DEFAULT_PLAYER_NAME, GC_PLAYER_NAME_LENGTH);
#endif
  }

  cout << GC_MESSAGE << endl;

  if (!(mode & CM_SOLO))
    Communicator::initialize(mode, port, host_name, player_name);
  else
    Random::seed(Random::generateSeed());

  MetaState::programStart(mode, player_name, width, height);

#ifdef AUDIO_ENABLED
  if (!nosound) {
    Sound::initialize();
    Music::initialize();
    Music::play_prelude();
  }
#endif

  glutMainLoop();

#ifdef AUDIO_ENABLED
  Music::stop();
  Music::cleanup();
  Sound::cleanup();
#endif
}

void parseCommandLine ( int argc, char **argv, int &mode, int &port,
 char *host_name, char *player_name , int &height, int &width )
{
  for (int n = 1; argv[n]; n++) {

    if (!strcmp(argv[n], "--nosound"))
      nosound = 1;
    else
    if (!strcmp(argv[n], "-s") || !strcmp(argv[n], "--server")) {
      if (mode & (CM_SERVER | CM_CLIENT | CM_SOLO)) usage();

      mode |= CM_SERVER;
      if (argv[n + 1] && argv[n + 1][0] != '-')
        port = atoi(argv[++n]);
      else
        port = 0;

    } else if (!strcmp(argv[n], "-1") || !strcmp(argv[n], "--solo")) {
      if (mode & (CM_SERVER | CM_CLIENT | CM_SOLO)) usage();

      mode |= CM_SOLO;

    } else if (!strcmp(argv[n], "-n") || !strcmp(argv[n], "--name")) {
      if (!argv[n + 1]) usage();

      strncpy(player_name, argv[++n], GC_PLAYER_NAME_LENGTH);
      player_name[GC_PLAYER_NAME_LENGTH - 1] = '\0';
      for (char *p = player_name; *p; p++)
        if (!isprint(*p)) *p = ' ';

    } else if (!strcmp(argv[n], "-l") || !strcmp(argv[n], "--low"))

      mode |= CM_LOW_GRAPHICS;
    
    else if (!strcmp(argv[n], "-r") || !strcmp(argv[n], "--really")) {

      mode |= CM_LOW_GRAPHICS;
      mode |= CM_REALLY_LOW_GRAPHICS;

    } else if (!strcmp(argv[n], "-X") || !strcmp(argv[n], "--extreme"))

      mode |= CM_X;

    else if (!strcmp(argv[n], "-w") || !strcmp(argv[n], "--wait"))
      mode |= CM_NO_TIME_OUT;

    else if (!strcmp(argv[n], "--hard")) {
      mode |= CM_AI;
      mode |= CM_AI_HARD;

    } else if (!strcmp(argv[n], "--easy")) {
      mode |= CM_AI;
      mode |= CM_AI_EASY;

    } else if (!strcmp(argv[n], "--medium")) {
      mode |= CM_AI;
      mode |= CM_AI_MEDIUM;

    } else if (!strcmp(argv[n], "--res")) {
      if (argv[n + 1] && argv[n + 1][0] != '-')
        height = width = atoi(argv[++n]);
      else
        height = width = -1;

    } else {
      if (mode & (CM_SERVER | CM_CLIENT | CM_SOLO)) usage();

      mode |= CM_CLIENT;
      strncpy(host_name, argv[n], GC_HOST_NAME_SIZE);
			++n;
			if (n < argc) {
				port = atoi(argv[n]);
			} else {
				port = 0;
				cerr << "No port specified.\n";
				usage();
			}		
    }
  }

  if (!(mode & (CM_SERVER | CM_CLIENT | CM_SOLO)))
    usage();

  if ((mode & CM_NO_TIME_OUT) && !(mode & CM_SERVER))
    usage();

  if ((mode & CM_REALLY_LOW_GRAPHICS) && !(mode & CM_LOW_GRAPHICS))
    usage();

  if ((mode & CM_AI && (mode & CM_X)))
    usage();

}

#define MKDIR(x,y) (mkdir(x, y))
#ifdef _WIN32
#  include <dir.h>
#  undef MKDIR
#  define MKDIR(x,y) (_mkdir(x))
#endif

void setupLocalDataDirectory (   )
{
  char local_directory[256];
  TextureLoader::buildLocalDataDirectoryName(local_directory);
  if (!TextureLoader::fileExists(local_directory)
   && MKDIR(local_directory, 0777)
   ) {
   cerr << "Error creating local data directory '" << local_directory
    << "'." << endl;
   exit(1);
  }
}
