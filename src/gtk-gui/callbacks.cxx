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

#ifndef _WIN32
	#include <sys/types.h>
	#include <signal.h>
#else
  #include <windows.h>
#endif

static GtkWidget *fraClient, *fraSingle, *fraServer;
static GtkWindow *window = NULL, *networking = NULL;
static int mode = CM_SOLO;
static GPid game_pid = 0;

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
	GtkWidget *networking;
#ifdef DEVELOPMENT
	g_print("game_end called!\n");
#endif
	game_pid = 0;
	if (networking) {
		gtk_widget_destroy(networking);
	}
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
on_btnCancel_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *win = GTK_WIDGET(user_data);
	gtk_widget_destroy(win);
}

void
on_winNetworking_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
#ifndef _WIN32
	kill(game_pid, 9);
#else
	TerminateProcess(pid, 9);
#endif
	networking = NULL;
}

gboolean networking_output (
		GIOChannel *source, GIOCondition condition, gpointer data)
{
	GtkWidget *win = GTK_WIDGET(data);
	GtkTextView *txtOutput = NULL;
	GtkTextIter iter;
	GtkTextBuffer *buffer = NULL;
	gchar *out = NULL;
	gsize length = 0;

	txtOutput = GTK_TEXT_VIEW(
			lookup_widget(GTK_WIDGET(win), "txtOutput"));
	buffer = gtk_text_view_get_buffer(txtOutput);
	gtk_text_buffer_get_end_iter(buffer, &iter);
	g_print("|");
	g_io_channel_read_line(source, &out, &length, NULL, NULL);
	g_print(out);
	g_print("|");
	gtk_text_buffer_insert(buffer, &iter, out, length);
	g_free(out);
	return true;
}

void
spawn_networking_dialog                (gint       standard_output)
{
	GIOChannel *channel;

	networking = GTK_WINDOW(create_winNetworking());
	gtk_widget_show(GTK_WIDGET(networking));
	
	channel = g_io_channel_unix_new(standard_output);
	g_io_add_watch(channel, G_IO_IN, networking_output, networking);
}

void
on_btnStart_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    window = 
			GTK_WINDOW(lookup_widget(GTK_WIDGET(button),"winCrackAttackSplash"));

		// Save the gui data when the game starts.
		gui_data_save(GTK_WIDGET(button));

		// Set the mode based on all the widget values...
		mode = generate_mode(GTK_WIDGET(button));

#ifdef DEVELOPMENT
    g_print("Looking for location: %s\n", GC_BINARY_LOCATION);
#endif
    gtk_widget_hide(GTK_WIDGET(window));
    GError *err = NULL;
		GPid pid;
		gtk_widget_hide(GTK_WIDGET(window));
		GSpawnFlags flags = (GSpawnFlags) (G_SPAWN_LEAVE_DESCRIPTORS_OPEN |
				G_SPAWN_DO_NOT_REAP_CHILD);
		
		gchar **args =
			generate_array(mode, GC_BINARY_LOCATION, GTK_WIDGET(button));
		
		gint sin, sout, serr;
		gboolean ret =
			g_spawn_async_with_pipes(
					NULL,	args,	NULL,
					flags, NULL, NULL,
					&pid,
					&sin, &sout, &serr,
					&err);

		g_child_watch_add(pid, (GChildWatchFunc) game_end, NULL);
		game_pid = pid;
		g_free(args);
    if (!ret) {
      if (err) ca_error_dialog(err->message);
    }

		if (mode & CM_SERVER) {
    	spawn_networking_dialog(sout);
    }
}

gboolean 
on_winCrackAttackSplash_delete_event   (GtkWindow *window,
                                        gpointer  user_data)
{
    return FALSE;
}

void
on_cmbQuality_changed                  (GtkComboBox     *cmb,
                                        gpointer         user_data)
{
  gint tmp = gtk_combo_box_get_active(cmb);
  GtkImage * imgLogo = GTK_IMAGE(lookup_widget(GTK_WIDGET(cmb), "imgLogo"));
  gchar *filename = NULL;
  switch (tmp) {
    case 0: {
              filename = find_pixmap_file("preview_normal.tga");
              break;
            }
    case 1: {
              filename = find_pixmap_file("preview_reduced.tga");
              break;
            }
    case 2: {
              filename = find_pixmap_file("preview_extremely_reduced.tga");
              break;
            }
    default: {
              filename = find_pixmap_file("preview_normal.tga");
             }
  }
  gtk_image_set_from_file(imgLogo, filename);
}
