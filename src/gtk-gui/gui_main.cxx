/*
 * gui_main.c
 * Andrew Sayman - 10/11/04
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

#include <gtk/gtk.h>

using namespace std;

#include "TextureLoader.h"
#include "Game.h"
#include "Attack.h"
#include "Communicator.h"
#include "MetaState.h"
#include "Random.h"
#include "gtk-gui/support.h"
#include "gtk-gui/interface.h"
#include "gtk-gui/callbacks.h"

static void setup_defaults ( GtkWidget *win );

#define GC_HOST_NAME_SIZE (256)
int glut_argc;
char **glut_argv;

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

int gui_main ( int argc, char **argv )
{
  char player_name[GC_PLAYER_NAME_LENGTH];
  char host_name[GC_HOST_NAME_SIZE];
  int port;
  int mode = 0;
  GtkWidget *winCrackAttackSplash = NULL;
  
  // Backup argc and argv to initialize glut in the child process
  glut_argc = argc;
  glut_argv = argv;

  player_name[0] = '\0';

  gtk_set_locale ();
  if (gtk_init_check (&argc, &argv)) {
    add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE);
    winCrackAttackSplash = create_winCrackAttackSplash ();
    setup_defaults(winCrackAttackSplash);
    gtk_widget_show (winCrackAttackSplash);
    gtk_main ();
  } else {
    parseCommandLine(argc, argv, mode, port, host_name, player_name);
    run_crack_attack(mode, port, host_name, player_name, -1, -1);
  }

  return 0;
}

static void setup_defaults ( GtkWidget *win )
{
  const unsigned int n = 256;
  char base_name[] = "gui_prefs";
  char file_name[256], buf[n]; // file_name is 256 because TL requires 256

  TextureLoader::buildLocalDataFileName(base_name, file_name);
  ifstream file(file_name);
  if (file.fail()) {
    cerr << "Error opening defaults file " << file_name << endl;
    return;
  }

  // get name
  file.get(buf, n, '=');
  g_return_if_fail(!strncmp(buf, "name", 5));
  file.get(); // drop delim
  file.get(buf,n);
  GtkEntry *name_ent = GTK_ENTRY(lookup_widget(GTK_WIDGET(win), "entPlayerName"));
  gtk_entry_set_text(name_ent, buf);
  file.get(); // read off newline

  // get resolution
  cout << "'" <<  buf << "'" << endl;
  file.get(buf, n, '=');
  cout << "'" << buf << "'" << endl;
  g_return_if_fail(!strncmp(buf, "resolution", n));
  file.get(); // drop delim
  file.get(buf,n);
  GtkWidget *w = lookup_widget(GTK_WIDGET(win), "optResolutions");
  gtk_option_menu_set_history(GTK_OPTION_MENU(w), atoi(buf));
  file.get(); // read off newline

  /*
  // get graphics settings
  file.get(buf, n, '=');
  g_return_if_fail(!strncmp(buf, "graphics", 9));
  file.get(); // drop delim
  file.get(buf,n);
  GtkWidget *w = lookup_widget(GTK_WIDGET(win), "optResolutions");
  gtk_option_menu_set_history(GTK_OPTION_MENU(w), atoi(buf));
  */

  file.close();
}
