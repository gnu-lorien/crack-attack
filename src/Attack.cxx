/*
 * Attack.cxx
 * Daniel Nelson - 8/29/0
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
     
#include <GL/glut.h>
#include <cstring>
#include <cctype>
#include <sys/stat.h>

#ifndef _WIN32
#  include <pwd.h>
#else
#  include <direct.h>
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Attack.h"
#include "Communicator.h"
#include "MetaState.h"
#include "Random.h"
#include "TextureLoader.h"

/*
 * Documentation
 *   html tables don't work right in explorer
 *   man-page bug
 *
 * Issues and Watches
 *   slow if left sitting a long time before game start
 *   gtk+ frontend
 *   central server for online game setup
 *   remove dying_count_2
 *   find and use correct GL_LIGHT_MODEL_COLOR_CONTROL defines
 */

int main ( int argc, char **argv )
{
  char player_name[GC_PLAYER_NAME_LENGTH];
  char host_name[256];
  int port;
  int mode = 0;

  glutInit(&argc, argv);

  player_name[0] = '\0';
  parseCommandLine(argc, argv, mode, port, host_name, player_name);

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

  setupLocalDataDirectory();

  if (!(mode & CM_SOLO))
    Communicator::initialize(mode, port, host_name, player_name);
  else
    Random::seed(Random::generateSeed());

  MetaState::programStart(mode, player_name);

  glutMainLoop();

  return 0;
}

inline void usage (   )
{
  cerr << "Usage: " GC_BINARY " --server [PORT] [--low] [-X] [--wait] "
   "[--name 'NAME']\n"
   "        <or>\n"
   "       " GC_BINARY " SERVER[:PORT] [--low] [--name 'NAME']\n"
   "        <or>\n"
   "       " GC_BINARY " --solo [--low] [-X] [--name 'NAME']"
   << endl;
  exit(1);
}

void parseCommandLine ( int argc, char **argv, int &mode, int &port,
 char *host_name, char *player_name )
{
  for (int n = 1; argv[n]; n++) {

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

    else if (!strcmp(argv[n], "-X") || !strcmp(argv[n], "--extreme"))

      mode |= CM_X;

    else if (!strcmp(argv[n], "-w") || !strcmp(argv[n], "--wait"))
      mode |= CM_NO_TIME_OUT;

    else {
      if (mode & (CM_SERVER | CM_CLIENT | CM_SOLO)) usage();

      mode |= CM_CLIENT;
      strcpy(host_name, argv[n]);
      char *ptr = strchr(host_name, ':');
      if (ptr) {
        port = atoi(ptr + 1);
        *ptr = '\0';
      } else
        port = 0;
    }
  }

  if (!(mode & (CM_SERVER | CM_CLIENT | CM_SOLO)))
    usage();

  if ((mode & CM_NO_TIME_OUT) && !(mode & CM_SERVER))
    usage();
}

void setupLocalDataDirectory (   )
{
  char local_directory[256];
  TextureLoader::buildLocalDataDirectoryName(local_directory);
  if (!TextureLoader::fileExists(local_directory)
#ifndef _WIN32
   && mkdir(local_directory, 0777)) {
#else
   && _mkdir(local_directory)) {
#endif
    cerr << "Error creating local data directory '" << local_directory
     << "'." << endl;
    exit(1);
  }
}
