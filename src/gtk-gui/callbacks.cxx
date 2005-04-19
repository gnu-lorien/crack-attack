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
#include <sys/wait.h>

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

gboolean
check_for_game_end (gpointer data) {
	int status;
    pid_t process_id = *((pid_t *)data);
    pid_t ret_pid;
    ret_pid = waitpid (process_id, &status, 0);
#ifdef DEVELOPMENT
		printf("process_id is %d and ret_pid is %d\n", process_id, ret_pid);
#endif
    if (ret_pid == process_id) {
        // Game finished
        MS_RUNNING = FALSE;
        running_process = 0;
        if (window) {
            gtk_widget_show(GTK_WIDGET(window));
        }
    } else {
        MS_RUNNING = TRUE;
    }
    return MS_RUNNING;
}

void
on_rbtnSingle_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *x = lookup_widget(GTK_WIDGET(togglebutton), "cbtnXtreme2");
		gtk_toggle_button_set_active((GtkToggleButton *) x, FALSE);
    if (gtk_toggle_button_get_active(togglebutton)) {
        prepare_for_actions(togglebutton);
        gtk_widget_set_sensitive (fraSingle, TRUE);
        GAME_SINGLE = TRUE;
        mode |= CM_SOLO;
    } else {
        mode &= ~CM_SOLO;
    }
}


void
on_rbtnServer_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *x = lookup_widget(GTK_WIDGET(togglebutton), "cbtnXtreme");
		gtk_toggle_button_set_active((GtkToggleButton *) x, FALSE);
    if (gtk_toggle_button_get_active(togglebutton)) {
        prepare_for_actions(togglebutton);
        gtk_widget_set_sensitive (fraServer, TRUE);
        GAME_SERVER = TRUE;
        mode |= CM_SERVER;
    } else {
        mode &= ~CM_SERVER;
    }
}

void
on_rbtnClient_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        prepare_for_actions(togglebutton);
        gtk_widget_set_sensitive (fraClient, TRUE);
        GAME_CLIENT = TRUE;
        mode |= CM_CLIENT;
    } else {
        mode &= ~CM_CLIENT;
    }
}


void
on_Xtreme_toggled                      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        GAME_EXTREME = TRUE;
        mode |= CM_X;
    } else {
        GAME_EXTREME = FALSE;
        mode &= ~CM_X;
    }
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
    window = GTK_WINDOW(lookup_widget(GTK_WIDGET(button),"winCrackAttackSplash"));
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
        entServerAddress = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entServerAddress"));
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
    entPlayerName = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entPlayerName"));
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
            g_print ("Player name: %s tmp: %s",player_name,tmp);
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

    gtk_widget_hide(GTK_WIDGET(window));
    int exit_status;
    GError *err = NULL;
		GPid pid;
		gtk_widget_hide(GTK_WIDGET(window));
		GSpawnFlags flags = (GSpawnFlags) (G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_DO_NOT_REAP_CHILD);
		gchar **args = generate_array(mode, br_strcat(BINDIR, GC_DD GC_BINARY), GTK_WIDGET(button));
		gboolean ret = g_spawn_async(NULL, args, NULL, flags, NULL, NULL, &pid, &err);
		g_timeout_add (500, check_for_game_end, (gpointer) &pid);
		g_free(args);
    if (!ret) {
      if (err) ca_error_dialog(err->message);
    }
    g_free(command);
}



void
on_btnHelp_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_cbtnLowGraphics_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *lowButton = lookup_widget(GTK_WIDGET(togglebutton), "cbtnReallyLowGraphics");
    if (gtk_toggle_button_get_active(togglebutton)) {
        GRAPHICS_LOW = TRUE;
        mode |= CM_LOW_GRAPHICS;
    } else {
        GRAPHICS_LOW = FALSE;
        mode &= ~CM_LOW_GRAPHICS;
        mode &= ~CM_REALLY_LOW_GRAPHICS;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lowButton), FALSE);
    }
}

void
on_cbtnReallyLowGraphics_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *lowButton = lookup_widget(GTK_WIDGET(togglebutton), "cbtnLowGraphics");
    if (gtk_toggle_button_get_active(togglebutton)) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lowButton), TRUE);
        GRAPHICS_LOW = TRUE;
        mode |= CM_LOW_GRAPHICS;
        mode |= CM_REALLY_LOW_GRAPHICS;
    } else {
        GRAPHICS_LOW = FALSE;
        mode &= ~CM_REALLY_LOW_GRAPHICS;
    }
}

gboolean 
on_winCrackAttackSplash_delete_event   (GtkWindow *window,
                                        gpointer  user_data)
{
    return FALSE;
}

void
on_640by480_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = GC_RESOLUTION_0;
}

void
on_800by600_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  height = width = GC_RESOLUTION_1;
}

void
on_1024by768_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = GC_RESOLUTION_2;	
}

void
on_1280by1024_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = GC_RESOLUTION_3;	
}

void
on_1600by1200_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = GC_RESOLUTION_4;
}

static void undo_ai_settings() 
{
    mode &= ~CM_AI;
    mode &= ~CM_AI_EASY;
    mode &= ~CM_AI_MEDIUM;
    mode &= ~CM_AI_HARD;
}

void
on_ai_none_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkWidget *x = lookup_widget(GTK_WIDGET(menuitem), "cbtnXtreme");
    gtk_widget_set_sensitive(x, TRUE);
    undo_ai_settings();
}

void
on_ai_easy_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkWidget *x = lookup_widget(GTK_WIDGET(menuitem), "cbtnXtreme");
		gtk_toggle_button_set_active((GtkToggleButton *) x, FALSE);
    gtk_widget_set_sensitive(x, FALSE);
    undo_ai_settings();
    mode |= CM_AI;
    mode |= CM_AI_EASY;
}

void
on_ai_medium_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkWidget *x = lookup_widget(GTK_WIDGET(menuitem), "cbtnXtreme");
		gtk_toggle_button_set_active((GtkToggleButton *) x, FALSE);
    gtk_widget_set_sensitive(x, FALSE);
    undo_ai_settings();
    mode |= CM_AI;
    mode |= CM_AI_MEDIUM;
}

void
on_ai_hard_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkWidget *x = lookup_widget(GTK_WIDGET(menuitem), "cbtnXtreme");
		gtk_toggle_button_set_active((GtkToggleButton *) x, FALSE);
    gtk_widget_set_sensitive(x, FALSE);
    undo_ai_settings();
    mode |= CM_AI;
    mode |= CM_AI_HARD;
}


