/*
 * modeparser.cxx
 * Kevin Webb - 4/16/05
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
 * vim: set noet ts=2 sw=2:
 */
#include <gtk/gtk.h>
#include "Mode.h"
#include "persist.h"
#include "modeparser.h"
#include "support.h"

/* Single Player Options. */
static const gchar *single_player = "--solo";
static const gchar *aieasy        = "--easy";
static const gchar *aimedium      = "--medium";
static const gchar *aihard        = "--hard";

/* Multiplayer Server Options.
 * PORT is dynamic. */
static const gchar *server        = "--server";

/* The client side doesn't need anything special.
 * SERVER and PORT are dynamic. */

/* Global Options.
 * NAME is dynamic. */
static const gchar *reduced       = "--low";
static const gchar *veryreduced   = "--really";
static const gchar *xtreme        = "--extreme";
static const gchar *name          = "--name";
static const gchar *space         = " ";

/* Resolution */
static const gchar *resolution    = "--res";

gchar**
generate_array(int mode, const gchar *start, GtkWidget *widget) {

#define RESULT_SIZE 15

	const gchar **result = (const gchar **) g_malloc0(sizeof(gchar *) * RESULT_SIZE);
	int i;
	for (i = 1; i < RESULT_SIZE; ++i) {
		result[i] = NULL;
	}
	result[0] = start;
	i = 1;

	if (mode & CM_SOLO) {  /* Solo */
		result[i] = single_player;
		++i;
		if (mode & CM_AI_EASY) {
			result[i] = aieasy;
			++i;
		} else if (mode & CM_AI_MEDIUM) {
			result[i] = aimedium;
			++i;
		} else if (mode & CM_AI_HARD) {
			result[i] = aihard;
			++i;
		}
	} else if (mode & CM_SERVER) {  /* Server */
		result[i] = server;
		++i;
		result[i] = gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entPort"));
		++i;
	} else if (mode & CM_CLIENT) {  /* Client */
		result[i] = gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entServerAddressClient"));
		++i;
		result[i] = gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entPortClient"));
		++i;
	}

	/* Global */
	if (mode & CM_REALLY_LOW_GRAPHICS) {
		result[i] = veryreduced;
		++i;
	}
	if (mode & CM_LOW_GRAPHICS) {
		result[i] = reduced;
		++i;
	}
	if (mode & CM_X) {
		result[i] = xtreme;
		++i;
	}


	/* Resolution */
	result[i] = resolution;
	++i;
	GtkWidget *cmbResolution = lookup_widget(GTK_WIDGET(widget), "cmbResolution");
	int resolution = (int) gtk_combo_box_get_active((GtkComboBox *) cmbResolution);
	switch(resolution) {
		case 1:
			result[i] = "570";
			++i;
			break;
		case 2:
			result[i] = "680";
			++i;
			break;
		case 3:
			result[i] = "970";
			++i;
			break;
		case 4:
			result[i] = "1170";
			++i;
			break;
		default:
			result[i] = "400";
			++i;
	}

	/* Add the name to the end, without a trailing space. */
	result[i] = name;
	++i;
	result[i] = gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entPlayerName"));
	++i;

#ifdef DEVELOPMENT
	g_print("Arguments:\n");
	for (i = 0; i < RESULT_SIZE; ++i) {
		if (result[i] != NULL) {
			g_print("%d: |%s|\n", i, result[i]);
		}
	}
#endif

	return (gchar **) result;
}

gboolean
validate_mode(int mode) {
	if (!(mode & (CM_SERVER | CM_CLIENT | CM_SOLO)))
		return FALSE;

	if ((mode & CM_NO_TIME_OUT) && !(mode & CM_SERVER))
		return FALSE;

	if ((mode & CM_REALLY_LOW_GRAPHICS) && !(mode & CM_LOW_GRAPHICS))
		return FALSE;

	if ((mode & CM_AI && (mode & CM_X)))
		return FALSE;

	return TRUE;
}

gint
generate_mode(GtkWidget *widget) {
	gint mode = 0;
	gint tmp = 0;

	tmp = gtk_notebook_get_current_page((GtkNotebook *) lookup_widget(GTK_WIDGET(widget), "ntbGameMode"));
	switch (tmp) {
		case 1: {
							mode |= CM_SERVER;
							mode &= ~CM_SOLO;
							mode &= ~CM_CLIENT;
							mode &= ~CM_AI;
							break;
						}
		case 2: {
							mode |= CM_CLIENT;
							mode &= ~CM_SOLO;
							mode &= ~CM_SERVER;
							mode &= ~CM_AI;
							break;
						}
		default: {
							 mode |= CM_SOLO;
							 mode &= ~CM_SERVER;
							 mode &= ~CM_CLIENT;
							 tmp = gtk_combo_box_get_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbAI"));
							 switch (tmp) {
								 case 1: {
													 mode |= CM_AI;
													 mode |= CM_AI_EASY;
													 break;
												 }
								 case 2: {
													 mode |= CM_AI;
													 mode |= CM_AI_MEDIUM;
													 break;
												 }
								 case 3: {
													 mode |= CM_AI;
													 mode |= CM_AI_HARD;
													 break;
												 }
								 default: {
														mode &= ~CM_AI;
														mode &= ~CM_AI_EASY;
														mode &= ~CM_AI_MEDIUM;
														mode &= ~CM_AI_HARD;
													}
							 }
						 }
	}
	
	tmp = gtk_toggle_button_get_active((GtkToggleButton *) lookup_widget(GTK_WIDGET(widget), "rbtnXtreme"));
	// Enable extreme mode iff it's selected and AI isn't selected.
	if (tmp && !(mode & CM_AI)) {
		mode |= CM_X;
	} else {
		mode &= ~CM_X;
	}

	tmp = gtk_combo_box_get_active((GtkComboBox *) lookup_widget(GTK_WIDGET(widget), "cmbQuality"));
	switch (tmp) {
		case 1: {
							mode |= CM_LOW_GRAPHICS;
							break;
						}
		case 2: {
							mode |= CM_LOW_GRAPHICS;
							mode |= CM_REALLY_LOW_GRAPHICS;
							break;
						}
		default: {
							 mode &= ~CM_LOW_GRAPHICS;
							 mode &= ~CM_REALLY_LOW_GRAPHICS;
						 }
	}
	return mode;
}
