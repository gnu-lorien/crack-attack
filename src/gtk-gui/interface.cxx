/*
 * interface.cxx
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_winCrackAttackSplash (void)
{
  GtkWidget *winCrackAttackSplash;
  GtkWidget *hbox1;
  GtkWidget *vbox1;
  GtkWidget *image1;
  GtkWidget *vbox5;
  GtkWidget *label4;
  GtkWidget *entPlayerName;
  GtkWidget *cbtnLowGraphics, *cbtnReallyLowGraphics;
  GtkWidget *alignment1;
  GtkWidget *vbox2;
  GtkWidget *hbox8;
  GtkWidget *lblResolutions;
  GtkWidget *optResolutions;
  GtkWidget *menu1;
  GtkWidget *_640by480;
  GtkWidget *_800by600;
  GtkWidget *_1024by768;
  GtkWidget *_1280by1024;
  GtkWidget *_1600by1200;
  GtkWidget *hbox2;
  GtkWidget *rbtnSingle;
  GSList *rbtnSingle_group = NULL;
  GtkWidget *fraSingle;
  GtkWidget *vbox8;
  GtkWidget *hbox9;
  GtkWidget *lblComputer;
  GtkWidget *optionmenu1;
  GtkWidget *menu2;
  GtkWidget *ai_none;
  GtkWidget *image2;
  GtkWidget *ai_easy;
  GtkWidget *image3;
  GtkWidget *ai_medium;
  GtkWidget *image4;
  GtkWidget *ai_hard;
  GtkWidget *image5;
  GtkWidget *cbtnXtreme;
  GtkWidget *hbox3;
  GtkWidget *rbtnServer;
  GtkWidget *fraServer;
  GtkWidget *vbox3;
  GtkWidget *cbtnXtreme2;
  GtkWidget *lblServerAddress;
  GtkWidget *lblServerIp;
  GtkWidget *hbox5;
  GtkWidget *lblPort;
  GtkWidget *entPort;
  GtkWidget *hbox4;
  GtkWidget *rbtnClient;
  GtkWidget *fraClient;
  GtkWidget *vbox4;
  GtkWidget *label1;
  GtkWidget *entServerAddress;
  GtkWidget *hbox6;
  GtkWidget *label3;
  GtkWidget *entPort2;
  GtkWidget *vbox6;
  GtkWidget *hbox7;
  GtkWidget *btnStart;
  GtkWidget *btnExit;
  GtkWidget *btnHelp;
  GtkWidget *scrolledwindow1;
  GtkWidget *txtGameOutput;

  winCrackAttackSplash = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (winCrackAttackSplash), "Crack-Attack!");
  gtk_window_set_destroy_with_parent (GTK_WINDOW (winCrackAttackSplash), TRUE);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (winCrackAttackSplash), hbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

  image1 = create_pixmap (winCrackAttackSplash, "logo.tga");
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (vbox1), image1, TRUE, TRUE, 0);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_box_pack_start (GTK_BOX (vbox1), vbox5, TRUE, TRUE, 0);

  label4 = gtk_label_new ("Player Name:");
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (vbox5), label4, FALSE, FALSE, 0);

  entPlayerName = gtk_entry_new ();
  gtk_widget_show (entPlayerName);
  gtk_box_pack_start (GTK_BOX (vbox5), entPlayerName, FALSE, FALSE, 0);
  gtk_entry_set_text (GTK_ENTRY (entPlayerName), g_get_user_name());

  cbtnLowGraphics = gtk_check_button_new_with_mnemonic ("Reduced Graphics Mode");
  cbtnReallyLowGraphics = gtk_check_button_new_with_mnemonic("Extremely Reduced Graphics Mode");

  gtk_widget_show (cbtnLowGraphics);
  gtk_widget_show (cbtnReallyLowGraphics);
  gtk_box_pack_start (GTK_BOX (vbox5), cbtnLowGraphics, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox5), cbtnReallyLowGraphics, FALSE, FALSE, 0);


  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_box_pack_start (GTK_BOX (hbox1), alignment1, TRUE, TRUE, 0);

  vbox2 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (alignment1), vbox2);

  hbox8 = gtk_hbox_new (FALSE, 5);
  gtk_widget_show (hbox8);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox8, FALSE, FALSE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox8), 5);

  lblResolutions = gtk_label_new ("Resolution: ");
  gtk_widget_show (lblResolutions);
  gtk_box_pack_start (GTK_BOX (hbox8), lblResolutions, FALSE, FALSE, 0);

  optResolutions = gtk_option_menu_new ();
  gtk_widget_show (optResolutions);
  gtk_box_pack_start (GTK_BOX (hbox8), optResolutions, FALSE, FALSE, 0);

  menu1 = gtk_menu_new ();

  _640by480 = gtk_menu_item_new_with_mnemonic ("640 x 480");
  gtk_widget_show (_640by480);
  gtk_container_add (GTK_CONTAINER (menu1), _640by480);

  _800by600 = gtk_menu_item_new_with_mnemonic ("800 x 600");
  gtk_widget_show (_800by600);
  gtk_container_add (GTK_CONTAINER (menu1), _800by600);

  _1024by768 = gtk_menu_item_new_with_mnemonic ("1024 x 768");
  gtk_widget_show (_1024by768);
  gtk_container_add (GTK_CONTAINER (menu1), _1024by768);

  _1280by1024 = gtk_menu_item_new_with_mnemonic ("1280 x 1024");
  gtk_widget_show (_1280by1024);
  gtk_container_add (GTK_CONTAINER (menu1), _1280by1024);

  _1600by1200 = gtk_menu_item_new_with_mnemonic ("1600 x 1200");
  gtk_widget_show (_1600by1200);
  gtk_container_add (GTK_CONTAINER (menu1), _1600by1200);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optResolutions), menu1);

  hbox2 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, TRUE, TRUE, 0);

  rbtnSingle = gtk_radio_button_new_with_mnemonic (NULL, "Single Player");
  gtk_widget_show (rbtnSingle);
  gtk_box_pack_start (GTK_BOX (hbox2), rbtnSingle, FALSE, TRUE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rbtnSingle), rbtnSingle_group);
  rbtnSingle_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rbtnSingle));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rbtnSingle), TRUE);

  fraSingle = gtk_frame_new (NULL);
  gtk_widget_show (fraSingle);
  gtk_box_pack_end (GTK_BOX (hbox2), fraSingle, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (fraSingle), 2);

  vbox8 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox8);
  gtk_container_add (GTK_CONTAINER (fraSingle), vbox8);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox8), hbox9, FALSE, FALSE, 0);

  lblComputer = gtk_label_new ("Computer AI:");
  gtk_widget_show (lblComputer);
  gtk_box_pack_start (GTK_BOX (hbox9), lblComputer, FALSE, FALSE, 5);

  optionmenu1 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu1);
  gtk_box_pack_start (GTK_BOX (hbox9), optionmenu1, FALSE, FALSE, 5);

  menu2 = gtk_menu_new ();

  ai_none = gtk_image_menu_item_new_with_mnemonic ("None");
  gtk_widget_show (ai_none);
  gtk_container_add (GTK_CONTAINER (menu2), ai_none);

  image2 = gtk_image_new_from_stock ("gtk-no", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image2);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (ai_none), image2);

  ai_easy = gtk_image_menu_item_new_with_mnemonic ("Easy");
  gtk_widget_show (ai_easy);
  gtk_container_add (GTK_CONTAINER (menu2), ai_easy);

  image3 = gtk_image_new_from_stock ("gtk-yes", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image3);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (ai_easy), image3);

  ai_medium = gtk_image_menu_item_new_with_mnemonic ("Medium");
  gtk_widget_show (ai_medium);
  gtk_container_add (GTK_CONTAINER (menu2), ai_medium);

  image4 = gtk_image_new_from_stock ("gtk-dialog-warning", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image4);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (ai_medium), image4);

  ai_hard = gtk_image_menu_item_new_with_mnemonic ("Hard");
  gtk_widget_show (ai_hard);
  gtk_container_add (GTK_CONTAINER (menu2), ai_hard);

  image5 = gtk_image_new_from_stock ("gtk-quit", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image5);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (ai_hard), image5);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu1), menu2);

  cbtnXtreme = gtk_check_button_new_with_mnemonic ("X-treme");
  gtk_widget_show (cbtnXtreme);
  gtk_box_pack_start (GTK_BOX (vbox8), cbtnXtreme, FALSE, FALSE, 0);

  hbox3 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox3, TRUE, TRUE, 0);

  rbtnServer = gtk_radio_button_new_with_mnemonic (NULL, "Two Player - Server");
  gtk_widget_show (rbtnServer);
  gtk_box_pack_start (GTK_BOX (hbox3), rbtnServer, FALSE, TRUE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rbtnServer), rbtnSingle_group);
  rbtnSingle_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rbtnServer));

  fraServer = gtk_frame_new (NULL);
  gtk_widget_show (fraServer);
  gtk_box_pack_start (GTK_BOX (hbox3), fraServer, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (fraServer), 2);
  gtk_widget_set_sensitive (fraServer, FALSE);

  vbox3 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (fraServer), vbox3);

  cbtnXtreme2 = gtk_check_button_new_with_mnemonic ("X-treme");
  gtk_widget_show (cbtnXtreme2);
  gtk_box_pack_start (GTK_BOX (vbox3), cbtnXtreme2, FALSE, FALSE, 0);

  lblServerAddress = gtk_label_new ("Server Address:");
  gtk_widget_show (lblServerAddress);
  gtk_box_pack_start (GTK_BOX (vbox3), lblServerAddress, FALSE, FALSE, 0);

  // Find the ip to display
  struct hostent *h;
  char host_name[256];
  lblServerIp = gtk_label_new ("");
  if (0 == gethostname(host_name, 256)) {
    if (NULL != (h = gethostbyname(host_name))) {
      gtk_label_set_text(
        GTK_LABEL(lblServerIp), 
        inet_ntoa(*((struct in_addr *)h->h_addr)));
    }
  }
  gtk_widget_show (lblServerIp);
  gtk_box_pack_start (GTK_BOX (vbox3), lblServerIp, FALSE, FALSE, 0);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox5, TRUE, TRUE, 0);

  lblPort = gtk_label_new ("Port:");
  gtk_widget_show (lblPort);
  gtk_box_pack_start (GTK_BOX (hbox5), lblPort, FALSE, FALSE, 0);

  entPort = gtk_entry_new ();
  gtk_entry_set_text(
    GTK_ENTRY(entPort),
    "8080");
  gtk_widget_show (entPort);
  gtk_box_pack_start (GTK_BOX (hbox5), entPort, TRUE, TRUE, 0);

  hbox4 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox4, TRUE, TRUE, 0);

  rbtnClient = gtk_radio_button_new_with_mnemonic (NULL, "Two Player - Client");
  gtk_widget_show (rbtnClient);
  gtk_box_pack_start (GTK_BOX (hbox4), rbtnClient, FALSE, TRUE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rbtnClient), rbtnSingle_group);
  rbtnSingle_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rbtnClient));

  fraClient = gtk_frame_new (NULL);
  gtk_widget_show (fraClient);
  gtk_box_pack_start (GTK_BOX (hbox4), fraClient, TRUE, TRUE, 0);
  gtk_widget_set_sensitive (fraClient, FALSE);

  vbox4 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (fraClient), vbox4);

  label1 = gtk_label_new ("Server Address:");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox4), label1, FALSE, FALSE, 0);

  entServerAddress = gtk_entry_new ();
  gtk_widget_show (entServerAddress);
  gtk_box_pack_start (GTK_BOX (vbox4), entServerAddress, FALSE, FALSE, 0);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox6);
  gtk_box_pack_start (GTK_BOX (vbox4), hbox6, TRUE, TRUE, 0);

  label3 = gtk_label_new ("Port:");
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (hbox6), label3, FALSE, FALSE, 0);

  entPort2 = gtk_entry_new ();
  gtk_entry_set_text(
    GTK_ENTRY(entPort2),
    "8080");
  gtk_widget_show (entPort2);
  gtk_box_pack_start (GTK_BOX (hbox6), entPort2, TRUE, TRUE, 0);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox6);
  gtk_box_pack_start (GTK_BOX (vbox2), vbox6, FALSE, FALSE, 4);

  hbox7 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbox7);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox7, TRUE, TRUE, 0);

  btnStart = gtk_button_new_with_mnemonic ("Start Game");
  gtk_widget_show (btnStart);
  gtk_box_pack_start (GTK_BOX (hbox7), btnStart, FALSE, TRUE, 0);

  btnExit = gtk_button_new_with_mnemonic ("Exit");
  gtk_widget_show (btnExit);
  gtk_box_pack_start (GTK_BOX (hbox7), btnExit, FALSE, TRUE, 0);

  btnHelp = gtk_button_new_with_mnemonic ("Help");
  gtk_widget_show (btnHelp);
  gtk_box_pack_start (GTK_BOX (hbox7), btnHelp, FALSE, TRUE, 0);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (GTK_BOX (vbox6), scrolledwindow1, TRUE, TRUE, 0);

  txtGameOutput = gtk_text_view_new ();
  gtk_widget_show (txtGameOutput);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), txtGameOutput);

  g_signal_connect ((gpointer) winCrackAttackSplash, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) cbtnLowGraphics, "toggled",
                    G_CALLBACK (on_cbtnLowGraphics_toggled),
                    NULL);
  g_signal_connect ((gpointer) cbtnReallyLowGraphics, "toggled",
                    G_CALLBACK (on_cbtnReallyLowGraphics_toggled),
                    NULL);
  g_signal_connect ((gpointer) _640by480, "activate",
                    G_CALLBACK (on_640by480_activate),
                    NULL);
  g_signal_connect ((gpointer) _800by600, "activate",
                    G_CALLBACK (on_800by600_activate),
                    NULL);
  g_signal_connect ((gpointer) _1024by768, "activate",
                    G_CALLBACK (on_1024by768_activate),
                    NULL);
  g_signal_connect ((gpointer) _1280by1024, "activate",
                    G_CALLBACK (on_1280by1024_activate),
                    NULL);
  g_signal_connect ((gpointer) _1600by1200, "activate",
                    G_CALLBACK (on_1600by1200_activate),
                    NULL);
  g_signal_connect_after ((gpointer) rbtnSingle, "toggled",
                          G_CALLBACK (on_rbtnSingle_toggled),
                          NULL);
  g_signal_connect ((gpointer) ai_none, "activate",
                    G_CALLBACK (on_ai_none_activate),
                    NULL);
  g_signal_connect ((gpointer) ai_easy, "activate",
                    G_CALLBACK (on_ai_easy_activate),
                    NULL);
  g_signal_connect ((gpointer) ai_medium, "activate",
                    G_CALLBACK (on_ai_medium_activate),
                    NULL);
  g_signal_connect ((gpointer) ai_hard, "activate",
                    G_CALLBACK (on_ai_hard_activate),
                    NULL);
  g_signal_connect ((gpointer) cbtnXtreme, "toggled",
                    G_CALLBACK (on_Xtreme_toggled),
                    NULL);
  g_signal_connect_after ((gpointer) rbtnServer, "toggled",
                          G_CALLBACK (on_rbtnServer_toggled),
                          NULL);
  g_signal_connect ((gpointer) cbtnXtreme2, "toggled",
                    G_CALLBACK (on_Xtreme_toggled),
                    NULL);
  g_signal_connect_after ((gpointer) rbtnClient, "toggled",
                          G_CALLBACK (on_rbtnClient_toggled),
                          NULL);
  g_signal_connect ((gpointer) btnStart, "clicked",
                    G_CALLBACK (on_btnStart_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnExit, "clicked",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) btnHelp, "clicked",
                    G_CALLBACK (on_btnHelp_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (winCrackAttackSplash, winCrackAttackSplash, "winCrackAttackSplash");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, image1, "image1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, label4, "label4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entPlayerName, "entPlayerName");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cbtnLowGraphics, "cbtnLowGraphics");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cbtnReallyLowGraphics, "cbtnReallyLowGraphics");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblResolutions, "lblResolutions");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, optResolutions, "optResolutions");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, _640by480, "_640by480");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, _800by600, "_800by600");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, _1024by768, "_1024by768");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, _1280by1024, "_1280by1024");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, _1600by1200, "_1600by1200");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, rbtnSingle, "rbtnSingle");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, fraSingle, "fraSingle");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox8, "vbox8");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblComputer, "lblComputer");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, optionmenu1, "optionmenu1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, menu2, "menu2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, ai_none, "ai_none");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, image2, "image2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, ai_easy, "ai_easy");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, image3, "image3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, ai_medium, "ai_medium");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, image4, "image4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, ai_hard, "ai_hard");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, image5, "image5");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cbtnXtreme, "cbtnXtreme");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, rbtnServer, "rbtnServer");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, fraServer, "fraServer");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cbtnXtreme2, "cbtnXtreme2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblServerAddress, "lblServerAddress");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblServerIp, "lblServerIp");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblPort, "lblPort");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entPort, "entPort");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, rbtnClient, "rbtnClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, fraClient, "fraClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, label1, "label1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entServerAddress, "entServerAddress");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox6, "hbox6");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, label3, "label3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entPort2, "entPort2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox6, "vbox6");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, btnStart, "btnStart");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, btnExit, "btnExit");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, btnHelp, "btnHelp");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, txtGameOutput, "txtGameOutput");

  return winCrackAttackSplash;
}

