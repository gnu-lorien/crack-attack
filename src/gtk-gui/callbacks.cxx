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
                break;
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

void
on_rbtnSingle_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
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

gboolean
check_for_game_end (gpointer data) {
    pid_t process_id = *((pid_t *)data);
    pid_t ret_pid;
    ret_pid = waitpid (0, NULL, WNOHANG);
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
on_btnStart_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkTextView *view;
    GtkTextBuffer *buffer;
    GtkWidget *dialog;
    //GtkWindow *window;
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
        dialog = gtk_message_dialog_new (window,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "Error: crack-attack is already running");
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
        return;
    }
    if (GAME_SINGLE == TRUE) {
        host_name[0] = '\0';
    } else if (GAME_SERVER == TRUE) {
        //command = g_strconcat(command, "--server ", NULL);
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
            tmp = (gchar *)gtk_entry_get_text(entPort);
            g_strlcpy (host_name, tmp, 256);
        }
    }

    /* Make the game xtreme!! */
    if (GAME_EXTREME == TRUE) {
        //command = g_strconcat(command, "-X ", NULL);
        mode |= CM_X;
    }
    
    /* Set the name */
    entPlayerName = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entPlayerName"));
    if (entPlayerName) {
        tmp = (gchar *)gtk_entry_get_text(entPlayerName);
        if (tmp) {
            g_strlcpy(player_name, tmp, 256);
            g_print ("Player name: %s tmp: %s",player_name,tmp);
        }
    }
    
    g_print(command);
    //error = g_new(GError, 1);
    //g_spawn_command_line_async (command, error);
    gtk_widget_hide(GTK_WIDGET(window));
    g_print("Hiding window");
    fork_ret = fork();
    if (fork_ret == -1) {
        g_error("Crack-attack cannot be run");
        return;
    }
    g_timeout_add (250, check_for_game_end, (gpointer) &fork_ret);
    running_process = fork_ret;
    MS_RUNNING = TRUE;
    if (fork_ret != 0) {
        /*
        int ret;
        ret = waitpid(fork_ret, &status, 0);
        g_print("%d", status);
        if (ret != fork_ret) {
            g_warning("The wrong process exited");
        }
        if (ret == -1) {
            g_warning("Error in the child");
        }
        */
    } else {
        pid_t *process_id = (pid_t *)g_malloc0(sizeof (pid_t));
        *process_id = fork_ret;
        g_print("Adding the in game check.");
        // Init glut here to prevent problems of glut never exiting
        glutInit(&glut_argc, glut_argv);
        run_crack_attack(mode, port, host_name, player_name, height, width);
        g_print("Do I ever return from this?");
        //kill (fork_ret, 9);
        return;
    }

    /* We keep having a zombie process happening here... */
    //wait(&status);
    
    /*
    g_spawn_command_line_sync (
            command,
            output,
            errorput,
            exit,
            error);
    */

    /*
    g_print(*output);
    view = lookup_widget(GTK_WIDGET(button),"txtGameOutput");
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

    gtk_text_buffer_set_text (buffer, *output, -1);
    */
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
    if (gtk_toggle_button_get_active(togglebutton)) {
        GRAPHICS_LOW = TRUE;
        mode |= CM_LOW_GRAPHICS;
    } else {
        GRAPHICS_LOW = FALSE;
        mode &= ~CM_LOW_GRAPHICS;
    }
}

gboolean 
on_winCrackAttackSplash_delete_event   (GtkWindow *window,
                                        gpointer  user_data)
{
    if (MS_RUNNING = TRUE) {
        /* The game is still running, so we need to kill it before
         * we can exit
         */
        kill(running_process, 9);
    }
    return FALSE;
}

void
on_640by480_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = 400;
}

void
on_800by600_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  height = width = 600;
}

void
on_1024by768_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = 700;	
}

void
on_1280by1024_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = 1000;	
}

void
on_1600by1200_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	height = width = 1200;
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
  undo_ai_settings();
}

void
on_ai_easy_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  undo_ai_settings();
  mode |= CM_AI;
  mode |= CM_AI_EASY;
}

void
on_ai_medium_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  undo_ai_settings();
  mode |= CM_AI;
  mode |= CM_AI_MEDIUM;
}

void
on_ai_hard_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  undo_ai_settings();
  mode |= CM_AI;
  mode |= CM_AI_HARD;
}

