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

void
gui_data_save                    (GtkWidget *widget)

{
	gchar file_name[256];
	GtkEntry *entPlayerName;
	gchar *tmp = NULL;
	gchar player_name[GC_PLAYER_NAME_LENGTH];
	gint resolution = 0;
	gint quality = 0;
	gint ai_difficulty;

	// Get the current name in the name text box.
	entPlayerName = GTK_ENTRY(lookup_widget(GTK_WIDGET(widget), "entPlayerName"));
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
	resolution = (gint) gtk_combo_box_get_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbResolution"));

	// Get the status of the quality combo box.
	quality = gtk_combo_box_get_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbQuality"));

	// Get the current AI difficulty setting.
	ai_difficulty = gtk_combo_box_get_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbAI"));

	// Generate the file name and write the data,
	TextureLoader::buildLocalDataFileName(GC_GUI_FILE_NAME, file_name);

	ofstream file(file_name);
	if (file.fail()) {
		cerr << "Unable to write gui data to file!\n";
		return;
	}

	file << player_name << endl;
	file << resolution << endl;
	file << quality << endl;
	file << ai_difficulty << endl;

	file.close();
}


void
gui_data_read                    (GtkWidget *widget)

{
	char buffer[256];
	char file_name[256];
	GtkEntry *entPlayerName;
	gchar player_name[GC_PLAYER_NAME_LENGTH];
	gint resolution = 0;
	gint quality = 0;
	gint ai_difficulty;


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
		quality = atoi(buffer);

		if (quality < 0 || quality > 2) {
			quality = 0;
		}

		file.getline(buffer, 256);
		ai_difficulty = atoi(buffer);

		if (ai_difficulty < 0 || ai_difficulty > 3) {
			ai_difficulty = 0;
		}
		
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

	// Set the resolution.
	gtk_combo_box_set_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbResolution"), resolution);

	// Set the AI.
	gtk_combo_box_set_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbAI"), ai_difficulty);

	// Set the 	quality.
	gtk_combo_box_set_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbQuality"), quality);
}
