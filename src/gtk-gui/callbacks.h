/*
 * callbacks.h
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
#include <gtk/gtk.h>

void
on_btnStart_clicked                    (GtkButton       *button,
                                        gpointer         user_data);
void
on_btnCancel_clicked                    (GtkButton       *button,
                                        gpointer         user_data);
void
on_cmbQuality_changed                  (GtkComboBox     *button,
                                        gpointer         user_data);

#ifndef _WIN32
void
on_winNetworking_destroy               (GtkObject       *object,
                                        gpointer         user_data);
#endif
