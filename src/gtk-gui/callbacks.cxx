/*
 * callbacks.cxx
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
 *
 */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib.h>
#include "../Mode.h"
#include "../Attack.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "prefs.h"
#include "persist.h"
#include "modeparser.h"

static GtkWidget *fraClient, *fraSingle, *fraServer;
static GtkWindow *window = NULL;
static int mode = CM_SOLO;
static gboolean MS_RUNNING = FALSE;
static pid_t running_process = 0;
static int height = 400, width = 400;

extern int glut_argc;
extern char **glut_argv;

void turn_game_prefs_off () {
    GAME_SINGLE = GAME_SERVER = GAME_CLIENT = GAME_EXTREME = FALSE;
}

void lookup_widgets(GtkToggleButton * gtb) {
    fraClient = lookup_widget(GTK_WIDGET(gtb), "fraClient");
    fraSingle = lookup_widget(GTK_WIDGET(gtb), "fraSingle");
    fraServer = lookup_widget(GTK_WIDGET(gtb), "fraServer");
}

void turn_sensitive_off() {
    gtk_widget_set_sensitive (fraClient, FALSE);
    gtk_widget_set_sensitive (fraSingle, FALSE);
    gtk_widget_set_sensitive (fraServer, FALSE);
}

void toggle_sensitive (GtkToggleButton* gtb, gint selector) {
    gtk_widget_set_sensitive (fraClient, FALSE);
    gtk_widget_set_sensitive (fraSingle, FALSE);
    gtk_widget_set_sensitive (fraServer, FALSE);
    switch (selector) {
        case 1: gtk_widget_set_sensitive (fraClient, TRUE);
                break;
        case 2: gtk_widget_set_sensitive (fraServer, TRUE);
                break;
    }
}

void prepare_for_actions (GtkToggleButton *gtb) {
    lookup_widgets(gtb);
    turn_game_prefs_off();
    turn_sensitive_off();
}

static void
game_end (GPid pid, gint status, gpointer data) {
#ifdef DEVELOPMENT
	g_print("game_end called!\n");
#endif
	MS_RUNNING = FALSE;
	running_process = 0;
	if (window) {
		gtk_widget_show(GTK_WIDGET(window));
	}
	g_spawn_close_pid (pid);
}

void 
ca_error_dialog (const char *message)
{
    GtkWidget *dialog = NULL;
    dialog = gtk_message_dialog_new (window,
                              GTK_DIALOG_DESTROY_WITH_PARENT,
                              GTK_MESSAGE_ERROR,
                              GTK_BUTTONS_CLOSE,
                              message);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void
on_btnStart_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkTextView *view;
    GtkTextBuffer *buffer;
    GtkWidget *dialog;
    GtkEntry *entPlayerName, *entPort, *entServerAddress;
    GError **error = NULL;
    gchar **output = NULL, **errorput = NULL;
    gint *exit = NULL;
    gchar *command = NULL;
    gchar *tmp = NULL;
    pid_t fork_ret;
    int status;
    command = g_strdup("crack-attack ");
    char player_name[GC_PLAYER_NAME_LENGTH];
    char host_name[256];
    int port;
    window = 
			GTK_WINDOW(lookup_widget(GTK_WIDGET(button),"winCrackAttackSplash"));
    if (MS_RUNNING) {
        //Game is running, display an error message to the user
        ca_error_dialog("Error: crack-attack is already running");
        return;
    }
    if (GAME_SINGLE == TRUE) {
        host_name[0] = '\0';
    } else if (GAME_SERVER == TRUE) {
        host_name[0] = '\0';
        entPort = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entPort"));
        if (entPort) {
            tmp = (gchar *)gtk_entry_get_text(entPort);
            if (tmp) {
                port = atoi(tmp);
            } else {
                port = 0;
            }
        }
    } else if (GAME_CLIENT == TRUE) {
        entPort = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entPort2"));
        if (entPort) {
            tmp = (gchar *)gtk_entry_get_text(entPort);
            if (tmp) {
                port = atoi(tmp);
            } else {
                port = 0;
            }
        }
        entServerAddress = 
					GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entServerAddress"));
        if (entServerAddress) {
            tmp = (gchar *)gtk_entry_get_text(entServerAddress);
            g_strlcpy (host_name, tmp, 256);
        }
#ifdef DEVELOPMENT
        cout << "Host name: " << host_name << endl;
#endif
    }

    /* Make the game xtreme!! */
    if (GAME_EXTREME == TRUE) {
        mode |= CM_X;
    }
    
    /* Set the name */
    entPlayerName = 
			GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entPlayerName"));
    if (entPlayerName) {
        tmp = (gchar *)gtk_entry_get_text(entPlayerName);
        if (tmp) {
            if (strlen(tmp) == 0) {
                ca_error_dialog("Player's name isn't set.");
                gtk_entry_set_text(entPlayerName, g_get_user_name());
                return;
            }
            g_strlcpy(player_name, tmp, 256);
#ifdef DEVELOPMENT
            g_print ("Player name: %s tmp: %s", player_name,tmp);
#endif 
        }
    }
#ifdef DEVELOPMENT
    g_print(command);
#endif

		// Save the gui data when the game starts.
		gui_data_save(button);

		// Set the height and width to the selected value.
		height = width = gui_get_dimensions(GTK_WIDGET(button));

		// Set the AI difficulty to the correct setting.
		//mode = gui_get_difficulty(mode, GTK_WIDGET(button));

		// Set the mode based on all the widget values...
		mode = generate_mode(GTK_WIDGET(button));

#ifdef DEVELOPMENT
    g_print("Looking for location: %s\n", GC_BINARY_LOCATION);
#endif
    gtk_widget_hide(GTK_WIDGET(window));
    int exit_status;
    GError *err = NULL;
		GPid pid;
		gtk_widget_hide(GTK_WIDGET(window));
		GSpawnFlags flags = (GSpawnFlags) (G_SPAWN_LEAVE_DESCRIPTORS_OPEN |
				G_SPAWN_DO_NOT_REAP_CHILD);
		
		gchar **args =
			generate_array(mode, GC_BINARY_LOCATION, GTK_WIDGET(button));
		
		gboolean ret =
			g_spawn_async(NULL, args, NULL, flags, NULL, NULL, &pid, &err);

		MS_RUNNING = TRUE;
		g_child_watch_add(pid, (GChildWatchFunc) game_end, NULL);
		g_free(args);
    if (!ret) {
      if (err) ca_error_dialog(err->message);
    }
    g_free(command);
}

gboolean 
on_winCrackAttackSplash_delete_event   (GtkWindow *window,
                                        gpointer  user_data)
{
    return FALSE;
}
