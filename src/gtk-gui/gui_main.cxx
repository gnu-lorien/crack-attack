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
#endif

#include "glext.h"

#include <gtk/gtk.h>

using namespace std;

#include "MetaState.h"
#include "gtk-gui/support.h"
#include "gtk-gui/interface.h"
#include "gtk-gui/persist.h"

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

    gtk_init(&argc, &argv);
    add_pixmap_directory (GC_DATA_DIRECTORY(""));
    winCrackAttackSplash = create_winCrackAttackSplash ();
		gui_data_read(winCrackAttackSplash);
    gtk_widget_show (winCrackAttackSplash);
    gtk_main ();

  return 0;
}
