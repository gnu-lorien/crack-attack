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

#include "Attack.h"

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

#include "TextureLoader.h"
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

#include <boost/program_options.hpp>
namespace po = boost::program_options;

/**
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
  char player_name[GC_PLAYER_NAME_LENGTH] = "";
  char host_name[GC_HOST_NAME_SIZE] = "";
  char cube_tileset_dir[GC_CUBE_TILESET_DIR_LENGTH] = "";
  int port;
  int mode = 0;
  int height = -1, width = -1;
  
  player_name[0] = '\0';
  glutInit(&argc, argv);
  parseCommandLine(argc, argv, mode, port, host_name, player_name, height, width, cube_tileset_dir);
  run_crack_attack(mode, port, host_name, player_name, height, width, cube_tileset_dir);

  return 0;
}

inline void usage (   )
{
  std::cerr << "Usage: " GC_BINARY " --server [PORT] [--low] [--extreme] "
   "[--wait] [--name 'NAME']\n"
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
   << std::endl;
  exit(1);
}

static void conflicting_options(const po::variables_map& vm,
                                const char* opt1, const char* opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted()
        && vm.count(opt2) && !vm[opt2].defaulted())
        throw std::logic_error(std::string("Conflicting options '")
                          + opt1 + "' and '" + opt2 + "'.");
}

static void option_dependency(const po::variables_map& vm,
                              const char* for_what, const char* required_option)
{
    if (vm.count(for_what) && !vm[for_what].defaulted())
        if (vm.count(required_option) == 0 || vm[required_option].defaulted())
            throw std::logic_error(std::string("Option '") + for_what
                              + "' requires option '" + required_option + "'.");
}

void options (int argc, char **argv)
{
  using namespace std;
  using namespace po;

  bool no_sound;
  string player_name;
  string cube_tileset_dir;
  int port;
  int height;
  string host_name;
  int mode;

  po::variables_map vm;
  po::options_description all;

  try {
    po::options_description desc("Description");
    desc.add_options()
      ("help", "Display usage")
      ("version", "Display version information")
      ;

    po::options_description graphical("Output options");
    graphical.add_options()
      ("low,l", "Use low quality graphics. For slower computers.")
      ("really,r", "Use the lowest quality graphics. For the slowest computers. Don't pick this unless you really need it. It looks awful.")
      ("res", value<int>(&height)->default_value(-1),
        "Specify the height in pixels")
      ("cube-tileset-dir", po::value<string>(&cube_tileset_dir),
        "Directory where a cube tileset is stored")
      ("no-sound", "Turn off sound output")
      ;

    po::options_description ai("Computer opponent options");
    ai.add_options()
      ("hard", "Hard AI")
      ("medium", "Medium AI")
      ("easy", "Easy AI")
      ;

    po::options_description records("Score Recording Options");
    records.add_options()
      ("name,n", po::value<string>(&player_name)->default_value(GC_DEFAULT_PLAYER_NAME))
      ;

    po::options_description extreme("Extreme mode");
    extreme.add_options()
      ("extreme,X", "\"Extreme\" mode. Crazy things happen")
      ;

    po::options_description single_player("Single player");
    single_player.add_options()
      ("solo,1", "Solo mode. Play by yourself")
      ;

    po::options_description server("Server");
    server.add_options()
      ("server,s", po::value<int>(&port), "Specify the server port")
      ("wait,w", "Wait on a connection indefinitely")
      ;

    po::options_description client("Client");
    client.add_options()
      ("client", po::value<string>(&host_name), "IP address to connect to")
      ("port", po::value<int>(&port), "Port to connect to")
      ;

    all.add(desc).add(graphical).add(ai).add(records).add(extreme).add(single_player).add(server).add(client);

    cout << all << endl;

    store(parse_command_line(argc, argv, all), vm);
    notify(vm);

    conflicting_options(vm, "server", "solo");
    conflicting_options(vm, "server", "client");
    conflicting_options(vm, "wait", "solo");
    conflicting_options(vm, "wait", "client");
    conflicting_options(vm, "easy", "extreme");
    conflicting_options(vm, "hard", "extreme");
    conflicting_options(vm, "medium", "extreme");

    option_dependency(vm, "client", "port");

  } catch (exception &e) {
    cout << e.what() << endl;
  }

  if (vm.count("help")) {
    cout << all << endl;
    usage();
  }

  if (vm.count("solo")) {
    mode |= CM_SOLO;
  } else if (vm.count("server")) {
    mode |= CM_SERVER;
  } else if (vm.count("low")) {
    mode |= CM_LOW_GRAPHICS;
  } else if (vm.count("really")) {
    mode |= CM_LOW_GRAPHICS;
    mode |= CM_REALLY_LOW_GRAPHICS;
  } else if (vm.count("extreme")) {
    mode |= CM_X;
  } else if (vm.count("wait")) {
    mode |= CM_NO_TIME_OUT;
  } else if (vm.count("--hard")) {
    mode |= CM_AI;
    mode |= CM_AI_HARD;
  } else if (vm.count("--easy")) {
    mode |= CM_AI;
    mode |= CM_AI_EASY;
  } else if (vm.count("--medium")) {
    mode |= CM_AI;
    mode |= CM_AI_MEDIUM;
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

void run_crack_attack (
    int mode, 
    int port, 
    char *host_name, 
    char *player_name,
    int width,
    int height,
    char *cube_tileset_dir) {
  if (!player_name) {
    std::cerr << "Player name not properly allocated" << std::endl;
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

  std::cout << GC_MESSAGE << std::endl;

  if (!(mode & CM_SOLO))
    Communicator::initialize(mode, port, host_name, player_name);
  else
    Random::seed(Random::generateSeed());

  MetaState::programStart(mode, player_name, cube_tileset_dir, width, height);

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
 char *host_name, char *player_name , int &height, int &width, 
 char *cube_tileset_dir )
{
  options(argc, argv);
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

    } else if (!strcmp(argv[n], "--cube-tileset-dir")) {

      if (!argv[n + 1]) usage();

      strncpy(cube_tileset_dir, argv[++n], GC_CUBE_TILESET_DIR_LENGTH);
      cube_tileset_dir[GC_CUBE_TILESET_DIR_LENGTH - 1] = '\0';

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
        std::cerr << "No port specified.\n";
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
    std::cerr << "Error creating local data directory '" << local_directory
      << "'." << std::endl;
    exit(1);
  }
}
