/*
 * persist.cxx
 * Kevin Webb - 3/21/05
 *
 * Copyright (C) 2005  Kevin Webb
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
 */
#include <gtk/gtk.h>
#include <fstream>
#include <cstring>

#include "TextureLoader.h"
#include "persist.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"


int
gui_get_dimensions               (GtkWidget *button)

{
	GtkWidget *optResolutions;
	int resolution;
	
	optResolutions = lookup_widget(GTK_WIDGET(button), "optResolutions");
	if (optResolutions) {
		resolution = (int) gtk_option_menu_get_history((GtkOptionMenu *) optResolutions);
	}

	switch(resolution) {
		case 1:
			return GC_RESOLUTION_1;
		case 2:
			return GC_RESOLUTION_2;
		case 3:
			return GC_RESOLUTION_3;
		case 4:
			return GC_RESOLUTION_4;
		default:
			return GC_RESOLUTION_0;
	}
}


void
gui_data_save                    (GtkButton *button)

{
	char file_name[256];
	GtkEntry *entPlayerName;
	GtkWidget *optResolutions;
	GtkWidget *cbtnLowGraphics;
	GtkWidget *cbtnReallyLowGraphics;
	GtkWidget *optionmenu1;
	gchar *tmp = NULL;
	char player_name[GC_PLAYER_NAME_LENGTH];
	int resolution;
	gboolean low;
	gboolean reallylow;
	int ai_difficulty;

	// Get the current name in the name text box.
	entPlayerName = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entPlayerName"));
	if (entPlayerName) {
		tmp = (gchar *) gtk_entry_get_text(entPlayerName);
		if (tmp) {
			if (strlen(tmp) == 0) {
				gtk_entry_set_text(entPlayerName, g_get_user_name());
				return;
			}
			g_strlcpy(player_name, tmp, 256);
		}
	}

	// Get the currently selected resolution.
	optResolutions = lookup_widget(GTK_WIDGET(button), "optResolutions");
	if (optResolutions) {
		resolution = (int) gtk_option_menu_get_history((GtkOptionMenu *) optResolutions);
	}

	// Get the status of the low and really low graphics check boxes.
	cbtnLowGraphics = lookup_widget(GTK_WIDGET(button), "cbtnLowGraphics");
	cbtnReallyLowGraphics = lookup_widget(GTK_WIDGET(button), "cbtnReallyLowGraphics");
	if (cbtnLowGraphics && cbtnReallyLowGraphics) {
		low = gtk_toggle_button_get_active((GtkToggleButton *) cbtnLowGraphics);
		reallylow = gtk_toggle_button_get_active((GtkToggleButton *) cbtnReallyLowGraphics);
	}

	// Get the current AI difficulty setting.
	optionmenu1 = lookup_widget(GTK_WIDGET(button), "optionmenu1");
	ai_difficulty = (int) gtk_option_menu_get_history((GtkOptionMenu *) optionmenu1);

	// Generate the file name and write the data,
	TextureLoader::buildLocalDataFileName(GC_GUI_FILE_NAME, file_name);

	ofstream file(file_name);
	if (file.fail()) {
		cerr << "Unable to write gui data to file!\n";
		return;
	}

	file << player_name << endl;
	file << resolution << endl;
	file << low << endl;
	file << reallylow << endl;
	file << ai_difficulty << endl;

	file.close();
}


void
gui_data_read                    (GtkWidget *widget)

{
	char buffer[256];
	char file_name[256];
	GtkEntry *entPlayerName;
	GtkWidget *optResolutions;
	GtkWidget *cbtnLowGraphics;
	GtkWidget *cbtnReallyLowGraphics;
	GtkWidget *optionmenu1;
	char player_name[GC_PLAYER_NAME_LENGTH];
	int resolution;
	gboolean low;
	gboolean reallylow;
	int ai_difficulty;

	// Generate the name of the file and read the data.
	TextureLoader::buildLocalDataFileName(GC_GUI_FILE_NAME, file_name);

	ifstream file(file_name);
	if (file.fail()) {
#ifdef DEVELOPMENT
		cerr << "Unable to find gui data file!\n";
#endif
		return;
	}

	if (!file.eof()) {
		file.getline(buffer, 256);
		strcpy(player_name, buffer);
		
		file.getline(buffer, 256);
		resolution = atoi(buffer);

		if (resolution < 0 || resolution > 4) {
			resolution = 0;
		}

		file.getline(buffer, 256);
		low = atoi(buffer);

		file.getline(buffer, 256);
		reallylow = atoi(buffer);

		file.getline(buffer, 256);
		ai_difficulty = atoi(buffer);
		
	} else {
		file.close();
		return;
	}
	file.close();

	// Set the name.
	entPlayerName = GTK_ENTRY(lookup_widget(GTK_WIDGET(widget), "entPlayerName"));
	if (entPlayerName) {
		gtk_entry_set_text (GTK_ENTRY (entPlayerName), player_name);
	}

	/* Set the resolution in the option box.  This does not actually change the
	 * value of the height and width.  That is done with the gui_get_dimensions
	 * function before the game is launched...
	 */
	optResolutions = lookup_widget(GTK_WIDGET(widget), "optResolutions");
	if (optResolutions) {
		gtk_option_menu_set_history((GtkOptionMenu *) optResolutions, (guint) resolution);
	}

	/* Set the AI difficulty. This does not actually change the
	 * value of the height and width.  That is done with the gui_get_difficulty
	 * function before the game is launched...
	 */
	optionmenu1 = lookup_widget(GTK_WIDGET(widget), "optionmenu1");
	if (optionmenu1) {
		gtk_option_menu_set_history((GtkOptionMenu *) optionmenu1, (guint) ai_difficulty);
	}

	// Set the low and really low graphics check boxes.
	cbtnLowGraphics = lookup_widget(GTK_WIDGET(widget), "cbtnLowGraphics");
	cbtnReallyLowGraphics = lookup_widget(GTK_WIDGET(widget), "cbtnReallyLowGraphics");
	if (cbtnLowGraphics && cbtnReallyLowGraphics) {
		gtk_toggle_button_set_active((GtkToggleButton *) cbtnLowGraphics, low);
		gtk_toggle_button_set_active((GtkToggleButton *) cbtnReallyLowGraphics, reallylow);
	}
}

int
gui_get_difficulty(int mode_in, GtkWidget *widget) {
	GtkWidget *menu = lookup_widget(GTK_WIDGET(widget), "optionmenu1");
	int difficulty = (int) gtk_option_menu_get_history((GtkOptionMenu *) menu);
	int mode = mode_in;

	mode &= ~CM_AI;
	mode &= ~CM_AI_EASY;
	mode &= ~CM_AI_MEDIUM;
	mode &= ~CM_AI_HARD;

	switch(difficulty) {
		case 1: {
							mode |= CM_AI;
							mode |= CM_AI_EASY;
							return mode;
						}
		case 2: {
							mode |= CM_AI;
							mode |= CM_AI_MEDIUM;
							return mode;
						}
		case 3: {
							mode |= CM_AI;
							mode |= CM_AI_HARD;
							return mode;
						}
		default: {
							 return mode;
						 }
	}
}
