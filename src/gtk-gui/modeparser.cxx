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
 */
#include <gtk/gtk.h>
#include "../Mode.h"
#include "modeparser.h"
#include "support.h"

/* Single Player Options. */
static const gchar *single_player = "--solo ";
static const gchar *aieasy        = "--easy ";
static const gchar *aimedium      = "--medium ";
static const gchar *aihard        = "--hard ";

/* Multiplayer Server Options.
 * PORT is dynamic. */
static const gchar *server        = "--server ";

/* The client side doesn't need anything special.
 * SERVER and PORT are dynamic. */

/* Global Options.
 * NAME is dynamic. */
static const gchar *reduced       = "--low ";
static const gchar *veryreduced   = "--really ";
static const gchar *xtreme        = "--extreme ";
static const gchar *name          = "--name ";
static const gchar *space         = " ";
static const gchar *colon         = ":";

gchar*
generate_arguments(int mode, GtkWidget *widget) {
	
	gchar *args = "";
	
	if (mode & CM_SOLO) {  /* Solo */
		args = args_cat(args, single_player);
		if (mode & CM_AI_EASY) {
			args = args_cat(args, aieasy);
		} else if (mode & CM_AI_MEDIUM) {
			args = args_cat(args, aimedium);
		} else if (mode & CM_AI_HARD) {
			args = args_cat(args, aihard);
		}
	} else if (mode & CM_SERVER) {  /* Server */
		args = args_cat(args, server);
		args = args_cat(args, gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entPort")));
		args = args_cat(args, space);
	} else if (mode & CM_CLIENT) {  /* Client */
		args = args_cat(args, gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entServerAddress")));
		args = args_cat(args, colon);
		args = args_cat(args, gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entPort2")));
		args = args_cat(args, space);
	}

	/* Global */
	if (mode & CM_REALLY_LOW_GRAPHICS) {
		args = args_cat(args, veryreduced);
	}
	if (mode & CM_LOW_GRAPHICS) {
		args = args_cat(args, reduced);
	}
	if (mode & CM_X) {
		args = args_cat(args, xtreme);
	}

	/* Add the name to the end, without a trailing space. */
	args = args_cat(args, name);
	args = args_cat(args, gtk_entry_get_text((GtkEntry *) lookup_widget(GTK_WIDGET(widget), "entPlayerName")));

	return args;
}

gchar*
args_cat(gchar *old, const gchar *to_cat) {
	gchar *tmp = g_strconcat(old, to_cat, NULL);
	g_free(old);
	return tmp;
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
