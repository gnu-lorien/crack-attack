/*
 * interface.cxx
 * Andrew Sayman - 4/28/05
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
  GdkPixbuf *winCrackAttackSplash_icon_pixbuf;
  GtkWidget *hbox1;
  GtkWidget *vbox3;
  GtkWidget *alignment8;
  GtkWidget *frmGraphicsPreview;
  GtkWidget *alignment1;
  GtkWidget *vbox4;
  GtkWidget *imgLogo;
  GtkWidget *lblGraphicsPreview;
  GtkWidget *alignment9;
  GtkWidget *vbox1;
  GtkWidget *ntbGameMode;
  GtkWidget *vbox6;
  GtkWidget *lblAI;
  GtkWidget *cmbAI;
  GtkWidget *lblSolo;
  GtkWidget *vbox7;
  /*GtkWidget *lblTmpServerAddress;*/
  /*GtkWidget *lblServerAddress;*/
  GtkWidget *hbox8;
  GtkWidget *lblPort;
  GtkWidget *entPort;
  GtkWidget *lblServer;
  GtkWidget *vbox8;
  GtkWidget *lblTmpServerAddressClient;
  GtkWidget *entServerAddressClient;
  GtkWidget *hbox9;
  GtkWidget *lblTmpPortClient;
  GtkWidget *alignment7;
  GtkWidget *entPortClient;
  GtkWidget *lblClient;
  GtkWidget *lblAboutInfo;
  GtkWidget *lblAbout;
  GtkWidget *frmGraphicsSettings;
  GtkWidget *alignment10;
  GtkWidget *table1;
  GtkWidget *lblResolution;
  GtkWidget *lblQuality;
  GtkWidget *alignment4;
  GtkWidget *cmbResolution;
  GtkWidget *alignment5;
  GtkWidget *cmbQuality;
  GtkWidget *lblGraphicsSettings;
  GtkWidget *vbox2;
  GtkWidget *hbox2;
  GtkWidget *lblPlayerName;
  GtkWidget *alignment11;
  GtkWidget *entPlayerName;
  GtkWidget *frmGameMode;
  GtkWidget *alignment3;
  GtkWidget *vbox5;
  GtkWidget *hbox7;
  GtkWidget *hbox5;
  GtkWidget *rbtnNormal;
  GSList *rbtnNormal_group = NULL;
  GtkWidget *rbtnXtreme;
  GtkWidget *lblGameMode;
  GtkWidget *hbox10;
  GtkWidget *alignment12;
  GtkWidget *hbox3;
  GtkWidget *btnStart;
  GtkWidget *alignment2;
  GtkWidget *hbox4;
  GtkWidget *image3;
  GtkWidget *label8;
  GtkWidget *btnQuit;

  winCrackAttackSplash = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (winCrackAttackSplash, "winCrackAttackSplash");
  gtk_window_set_title (GTK_WINDOW (winCrackAttackSplash), "Crack Attack! 1.1.15-cvs");
  gtk_window_set_destroy_with_parent (GTK_WINDOW (winCrackAttackSplash), TRUE);
  winCrackAttackSplash_icon_pixbuf = create_pixbuf ("crack-attack.xpm");
  if (winCrackAttackSplash_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (winCrackAttackSplash), winCrackAttackSplash_icon_pixbuf);
      gdk_pixbuf_unref (winCrackAttackSplash_icon_pixbuf);
    }

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox1, "hbox1");
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (winCrackAttackSplash), hbox1);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox3, "vbox3");
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox3, TRUE, TRUE, 0);

  alignment8 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment8, "alignment8");
  gtk_widget_show (alignment8);
  gtk_box_pack_start (GTK_BOX (vbox3), alignment8, TRUE, TRUE, 3);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment8), 0, 0, 3, 3);

  frmGraphicsPreview = gtk_frame_new (NULL);
  gtk_widget_set_name (frmGraphicsPreview, "frmGraphicsPreview");
  gtk_widget_show (frmGraphicsPreview);
  gtk_container_add (GTK_CONTAINER (alignment8), frmGraphicsPreview);
  gtk_frame_set_shadow_type (GTK_FRAME (frmGraphicsPreview), GTK_SHADOW_IN);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment1, "alignment1");
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (frmGraphicsPreview), alignment1);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 3, 0, 6, 6);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox4, "vbox4");
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (alignment1), vbox4);

  imgLogo = create_pixmap (winCrackAttackSplash, "preview_normal.png");
  gtk_widget_set_name (imgLogo, "imgLogo");
  gtk_widget_show (imgLogo);
  gtk_box_pack_start (GTK_BOX (vbox4), imgLogo, FALSE, FALSE, 0);
  gtk_widget_set_size_request (imgLogo, 256, 256);

  lblGraphicsPreview = gtk_label_new ("<b>Graphics Preview</b>");
  gtk_widget_set_name (lblGraphicsPreview, "lblGraphicsPreview");
  gtk_widget_show (lblGraphicsPreview);
  gtk_frame_set_label_widget (GTK_FRAME (frmGraphicsPreview), lblGraphicsPreview);
  gtk_label_set_use_markup (GTK_LABEL (lblGraphicsPreview), TRUE);

  alignment9 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment9, "alignment9");
  gtk_widget_show (alignment9);
  gtk_box_pack_start (GTK_BOX (hbox1), alignment9, TRUE, TRUE, 3);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment9), 0, 0, 3, 3);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox1, "vbox1");
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (alignment9), vbox1);

  ntbGameMode = gtk_notebook_new ();
  gtk_widget_set_name (ntbGameMode, "ntbGameMode");
  gtk_widget_show (ntbGameMode);
  gtk_box_pack_start (GTK_BOX (vbox1), ntbGameMode, FALSE, FALSE, 3);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox6, "vbox6");
  gtk_widget_show (vbox6);
  gtk_container_add (GTK_CONTAINER (ntbGameMode), vbox6);

  lblAI = gtk_label_new_with_mnemonic ("Computer _AI");
  gtk_widget_set_name (lblAI, "lblAI");
  gtk_widget_show (lblAI);
  gtk_box_pack_start (GTK_BOX (vbox6), lblAI, FALSE, FALSE, 0);

  cmbAI = gtk_combo_box_new_text ();
  gtk_widget_set_name (cmbAI, "cmbAI");
  gtk_widget_show (cmbAI);
  gtk_box_pack_start (GTK_BOX (vbox6), cmbAI, FALSE, TRUE, 3);
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbAI), "None");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbAI), "Easy");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbAI), "Medium");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbAI), "Hard");
	gtk_combo_box_set_active (GTK_COMBO_BOX (cmbAI), 0);

  lblSolo = gtk_label_new_with_mnemonic ("S_olo");
  gtk_widget_set_name (lblSolo, "lblSolo");
  gtk_widget_show (lblSolo);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntbGameMode), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntbGameMode), 0), lblSolo);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox7, "vbox7");
  gtk_widget_show (vbox7);
  gtk_container_add (GTK_CONTAINER (ntbGameMode), vbox7);

  /*
  lblTmpServerAddress = gtk_label_new ("Server Address:");
  gtk_widget_set_name (lblTmpServerAddress, "lblTmpServerAddress");
  gtk_widget_show (lblTmpServerAddress);
  gtk_box_pack_start (GTK_BOX (vbox7), lblTmpServerAddress, FALSE, FALSE, 0);

  lblServerAddress = gtk_label_new ("INADDR_ANY");
  gtk_widget_set_name (lblServerAddress, "lblServerAddress");
  gtk_widget_show (lblServerAddress);
  gtk_box_pack_start (GTK_BOX (vbox7), lblServerAddress, FALSE, FALSE, 0);
  */

  hbox8 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox8, "hbox8");
  gtk_widget_show (hbox8);
  gtk_box_pack_start (GTK_BOX (vbox7), hbox8, TRUE, TRUE, 0);

  lblPort = gtk_label_new_with_mnemonic ("_Port:");
  gtk_widget_set_name (lblPort, "lblPort");
  gtk_widget_show (lblPort);
  gtk_box_pack_start (GTK_BOX (hbox8), lblPort, FALSE, FALSE, 0);

  entPort = gtk_entry_new ();
  gtk_widget_set_name (entPort, "entPort");
  gtk_widget_show (entPort);
  gtk_box_pack_start (GTK_BOX (hbox8), entPort, FALSE, FALSE, 3);
  gtk_entry_set_text (GTK_ENTRY (entPort), "8080");

  lblServer = gtk_label_new_with_mnemonic ("_Server");
  gtk_widget_set_name (lblServer, "lblServer");
  gtk_widget_show (lblServer);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntbGameMode), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntbGameMode), 1), lblServer);

  vbox8 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox8, "vbox8");
  gtk_widget_show (vbox8);
  gtk_container_add (GTK_CONTAINER (ntbGameMode), vbox8);

  lblTmpServerAddressClient = gtk_label_new_with_mnemonic ("Server _Address:");
  gtk_widget_set_name (lblTmpServerAddressClient, "lblTmpServerAddressClient");
  gtk_widget_show (lblTmpServerAddressClient);
  gtk_box_pack_start (GTK_BOX (vbox8), lblTmpServerAddressClient, FALSE, FALSE, 0);

  entServerAddressClient = gtk_entry_new ();
  gtk_widget_set_name (entServerAddressClient, "entServerAddressClient");
  gtk_widget_show (entServerAddressClient);
  gtk_box_pack_start (GTK_BOX (vbox8), entServerAddressClient, FALSE, FALSE, 0);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox9, "hbox9");
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox8), hbox9, TRUE, TRUE, 0);

  lblTmpPortClient = gtk_label_new_with_mnemonic ("_Port:");
  gtk_widget_set_name (lblTmpPortClient, "lblTmpPortClient");
  gtk_widget_show (lblTmpPortClient);
  gtk_box_pack_start (GTK_BOX (hbox9), lblTmpPortClient, FALSE, FALSE, 0);

  alignment7 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment7, "alignment7");
  gtk_widget_show (alignment7);
  gtk_box_pack_start (GTK_BOX (hbox9), alignment7, TRUE, TRUE, 0);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment7), 3, 3, 3, 0);

  entPortClient = gtk_entry_new ();
  gtk_widget_set_name (entPortClient, "entPortClient");
  gtk_widget_show (entPortClient);
  gtk_container_add (GTK_CONTAINER (alignment7), entPortClient);
  gtk_entry_set_text (GTK_ENTRY (entPortClient), "8080");

  lblClient = gtk_label_new_with_mnemonic ("_Client");
  gtk_widget_set_name (lblClient, "lblClient");
  gtk_widget_show (lblClient);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntbGameMode), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntbGameMode), 2), lblClient);
  gtk_misc_set_alignment (GTK_MISC (lblClient), 0.49, 0.5);

  lblAboutInfo = gtk_label_new (_("Crack-Attack! 1.1.15-cvs-00\nLooking for competition?\nJoin our community channel!\n#crack-attack on irc.freenode.net"));
  gtk_widget_show (lblAboutInfo);
  gtk_container_add (GTK_CONTAINER (ntbGameMode), lblAboutInfo);
  gtk_label_set_justify (GTK_LABEL (lblAboutInfo), GTK_JUSTIFY_CENTER);

  lblAbout = gtk_label_new_with_mnemonic ("_About");
  gtk_widget_show (lblAbout);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntbGameMode), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntbGameMode), 3), lblAbout);

  frmGraphicsSettings = gtk_frame_new (NULL);
  gtk_widget_set_name (frmGraphicsSettings, "frmGraphicsSettings");
  gtk_widget_show (frmGraphicsSettings);
  gtk_box_pack_start (GTK_BOX (vbox1), frmGraphicsSettings, FALSE, TRUE, 3);
  gtk_frame_set_shadow_type (GTK_FRAME (frmGraphicsSettings), GTK_SHADOW_IN);

  alignment10 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment10, "alignment10");
  gtk_widget_show (alignment10);
  gtk_container_add (GTK_CONTAINER (frmGraphicsSettings), alignment10);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment10), 0, 0, 12, 0);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_widget_set_name (table1, "table1");
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (alignment10), table1);

  lblResolution = gtk_label_new_with_mnemonic ("_Resolution:");
  gtk_widget_set_name (lblResolution, "lblResolution");
  gtk_widget_show (lblResolution);
  gtk_table_attach (GTK_TABLE (table1), lblResolution, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lblResolution), 0, 0.5);

  lblQuality = gtk_label_new_with_mnemonic ("_Quality:");
  gtk_widget_set_name (lblQuality, "lblQuality");
  gtk_widget_show (lblQuality);
  gtk_table_attach (GTK_TABLE (table1), lblQuality, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lblQuality), 0, 0.5);

  alignment4 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment4, "alignment4");
  gtk_widget_show (alignment4);
  gtk_table_attach (GTK_TABLE (table1), alignment4, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 3, 3);

  cmbResolution = gtk_combo_box_new_text ();
  gtk_widget_set_name (cmbResolution, "cmbResolution");
  gtk_widget_show (cmbResolution);
  gtk_container_add (GTK_CONTAINER (alignment4), cmbResolution);
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbResolution), "640x480");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbResolution), "800x600");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbResolution), "1024x768");
	gtk_combo_box_append_text (GTK_COMBO_BOX (cmbResolution), "1280x1024");
	gtk_combo_box_append_text (GTK_COMBO_BOX (cmbResolution), "1600x1200");
	gtk_combo_box_set_active (GTK_COMBO_BOX (cmbResolution), 0);

  alignment5 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment5, "alignment5");
  gtk_widget_show (alignment5);
  gtk_table_attach (GTK_TABLE (table1), alignment5, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 3, 3);

  cmbQuality = gtk_combo_box_new_text ();
  gtk_widget_set_name (cmbQuality, "cmbQuality");
  gtk_widget_show (cmbQuality);
  gtk_container_add (GTK_CONTAINER (alignment5), cmbQuality);
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbQuality), "Normal");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbQuality), "Reduced");
  gtk_combo_box_append_text (GTK_COMBO_BOX (cmbQuality), "Extremely Reduced");
	gtk_combo_box_set_active (GTK_COMBO_BOX (cmbQuality), 0);

  lblGraphicsSettings = gtk_label_new ("<b>Graphics Settings</b>");
  gtk_widget_set_name (lblGraphicsSettings, "lblGraphicsSettings");
  gtk_widget_show (lblGraphicsSettings);
  gtk_frame_set_label_widget (GTK_FRAME (frmGraphicsSettings), lblGraphicsSettings);
  gtk_label_set_use_markup (GTK_LABEL (lblGraphicsSettings), TRUE);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox2, "vbox2");
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox2, "hbox2");
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, TRUE, 3);

  lblPlayerName = gtk_label_new_with_mnemonic ("_Player name:");
  gtk_widget_set_name (lblPlayerName, "lblPlayerName");
  gtk_widget_show (lblPlayerName);
  gtk_box_pack_start (GTK_BOX (hbox2), lblPlayerName, FALSE, FALSE, 0);

  alignment11 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment11, "alignment11");
  gtk_widget_show (alignment11);
  gtk_box_pack_start (GTK_BOX (hbox2), alignment11, TRUE, TRUE, 0);

  entPlayerName = gtk_entry_new ();
  gtk_widget_set_name (entPlayerName, "entPlayerName");
  gtk_widget_show (entPlayerName);
  gtk_container_add (GTK_CONTAINER (alignment11), entPlayerName);
  gtk_entry_set_text (GTK_ENTRY (entPlayerName), g_get_user_name());

  frmGameMode = gtk_frame_new (NULL);
  gtk_widget_set_name (frmGameMode, "frmGameMode");
  gtk_widget_show (frmGameMode);
  gtk_box_pack_start (GTK_BOX (vbox2), frmGameMode, TRUE, TRUE, 3);
  gtk_frame_set_shadow_type (GTK_FRAME (frmGameMode), GTK_SHADOW_IN);

  alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment3, "alignment3");
  gtk_widget_show (alignment3);
  gtk_container_add (GTK_CONTAINER (frmGameMode), alignment3);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox5, "vbox5");
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (alignment3), vbox5);

  hbox7 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox7, "hbox7");
  gtk_widget_show (hbox7);
  gtk_box_pack_start (GTK_BOX (vbox5), hbox7, TRUE, TRUE, 0);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox5, "hbox5");
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (hbox7), hbox5, TRUE, TRUE, 0);

  rbtnNormal = gtk_radio_button_new_with_mnemonic (NULL, "_Normal");
  gtk_widget_set_name (rbtnNormal, "rbtnNormal");
  gtk_widget_show (rbtnNormal);
  gtk_box_pack_start (GTK_BOX (hbox5), rbtnNormal, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rbtnNormal), rbtnNormal_group);
  rbtnNormal_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rbtnNormal));

  rbtnXtreme = gtk_radio_button_new_with_mnemonic (NULL, "_X-treme");
  gtk_widget_set_name (rbtnXtreme, "rbtnXtreme");
  gtk_widget_show (rbtnXtreme);
  gtk_box_pack_start (GTK_BOX (hbox7), rbtnXtreme, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rbtnXtreme), rbtnNormal_group);
  rbtnNormal_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rbtnXtreme));

  lblGameMode = gtk_label_new ("<b>Game Mode</b>");
  gtk_widget_set_name (lblGameMode, "lblGameMode");
  gtk_widget_show (lblGameMode);
  gtk_frame_set_label_widget (GTK_FRAME (frmGameMode), lblGameMode);
  gtk_label_set_use_markup (GTK_LABEL (lblGameMode), TRUE);

  hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox10, "hbox10");
  gtk_widget_show (hbox10);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox10, FALSE, TRUE, 3);

  alignment12 = gtk_alignment_new (0, 1, 1, 1);
  gtk_widget_set_name (alignment12, "alignment12");
  gtk_widget_show (alignment12);
  gtk_box_pack_start (GTK_BOX (hbox10), alignment12, TRUE, TRUE, 0);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox3, "hbox3");
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment12), hbox3);

  btnStart = gtk_button_new ();
  gtk_widget_set_name (btnStart, "btnStart");
  gtk_widget_show (btnStart);
  gtk_box_pack_start (GTK_BOX (hbox3), btnStart, TRUE, TRUE, 3);

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_set_name (alignment2, "alignment2");
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (btnStart), alignment2);

  hbox4 = gtk_hbox_new (FALSE, 2);
  gtk_widget_set_name (hbox4, "hbox4");
  gtk_widget_show (hbox4);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox4);

  image3 = gtk_image_new_from_stock ("gtk-media-forward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_set_name (image3, "image3");
  gtk_widget_show (image3);
  gtk_box_pack_start (GTK_BOX (hbox4), image3, FALSE, FALSE, 0);

  label8 = gtk_label_new_with_mnemonic ("Start _Game");
  gtk_widget_set_name (label8, "label8");
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox4), label8, TRUE, TRUE, 0);

  btnQuit = gtk_button_new_from_stock ("gtk-quit");
  gtk_widget_set_name (btnQuit, "btnQuit");
  gtk_widget_show (btnQuit);
  gtk_box_pack_start (GTK_BOX (hbox3), btnQuit, TRUE, TRUE, 3);

  g_signal_connect ((gpointer) btnStart, "clicked",
                    G_CALLBACK (on_btnStart_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnQuit, "clicked",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) cmbQuality, "changed",
                    G_CALLBACK (on_cmbQuality_changed),
                    NULL);

  gtk_label_set_mnemonic_widget (GTK_LABEL (lblAI), cmbAI);
  gtk_label_set_mnemonic_widget (GTK_LABEL (lblPort), entPort);
  gtk_label_set_mnemonic_widget (GTK_LABEL (lblTmpServerAddressClient), entServerAddressClient);
  gtk_label_set_mnemonic_widget (GTK_LABEL (lblTmpPortClient), entPortClient);
  gtk_label_set_mnemonic_widget (GTK_LABEL (lblResolution), cmbResolution);
  gtk_label_set_mnemonic_widget (GTK_LABEL (lblQuality), cmbQuality);
  gtk_label_set_mnemonic_widget (GTK_LABEL (lblPlayerName), entPlayerName);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (winCrackAttackSplash, winCrackAttackSplash, "winCrackAttackSplash");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment8, "alignment8");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, frmGraphicsPreview, "frmGraphicsPreview");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, imgLogo, "imgLogo");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblGraphicsPreview, "lblGraphicsPreview");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment9, "alignment9");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, ntbGameMode, "ntbGameMode");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox6, "vbox6");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblAI, "lblAI");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cmbAI, "cmbAI");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblSolo, "lblSolo");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox7, "vbox7");
  /*GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblTmpServerAddress, "lblTmpServerAddress");*/
  /*GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblServerAddress, "lblServerAddress");*/
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblPort, "lblPort");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entPort, "entPort");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblServer, "lblServer");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox8, "vbox8");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblTmpServerAddressClient, "lblTmpServerAddressClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entServerAddressClient, "entServerAddressClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblTmpPortClient, "lblTmpPortClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment7, "alignment7");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entPortClient, "entPortClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblClient, "lblClient");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblAboutInfo, "lblAboutInfo");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblAbout, "lblAbout");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, frmGraphicsSettings, "frmGraphicsSettings");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment10, "alignment10");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, table1, "table1");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblResolution, "lblResolution");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblQuality, "lblQuality");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cmbResolution, "cmbResolution");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment5, "alignment5");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, cmbQuality, "cmbQuality");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblGraphicsSettings, "lblGraphicsSettings");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblPlayerName, "lblPlayerName");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment11, "alignment11");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, entPlayerName, "entPlayerName");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, frmGameMode, "frmGameMode");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment3, "alignment3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, rbtnNormal, "rbtnNormal");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, rbtnXtreme, "rbtnXtreme");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, lblGameMode, "lblGameMode");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox10, "hbox10");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment12, "alignment12");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, btnStart, "btnStart");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, image3, "image3");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, label8, "label8");
  GLADE_HOOKUP_OBJECT (winCrackAttackSplash, btnQuit, "btnQuit");

  return winCrackAttackSplash;
}

GtkWidget*
create_winNetworking (void)
{
  GtkWidget *winNetworking;
  GtkWidget *vbox10;
  GtkWidget *alignment13;
  GtkWidget *scrtxtOutput;
  GtkWidget *txtOutput;
  GtkWidget *alignment14;
  GtkWidget *btnCancel;

  winNetworking = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (winNetworking, "winNetworking");
  gtk_widget_set_size_request (winNetworking, 310, 195);
  gtk_window_set_title (GTK_WINDOW (winNetworking), "Crack Attack Server");
  gtk_window_set_modal (GTK_WINDOW (winNetworking), TRUE);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (winNetworking), TRUE);

  vbox10 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox10, "vbox10");
  gtk_widget_show (vbox10);
  gtk_container_add (GTK_CONTAINER (winNetworking), vbox10);

  alignment13 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_set_name (alignment13, "alignment13");
  gtk_widget_show (alignment13);
  gtk_box_pack_start (GTK_BOX (vbox10), alignment13, TRUE, TRUE, 0);
  gtk_widget_set_size_request (alignment13, 57, -1);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment13), 5, 5, 5, 5);

  scrtxtOutput = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_name (scrtxtOutput, "scrtxtOutput");
  gtk_widget_show (scrtxtOutput);
  gtk_container_add (GTK_CONTAINER (alignment13), scrtxtOutput);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrtxtOutput), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrtxtOutput), GTK_SHADOW_IN);

  txtOutput = gtk_text_view_new ();
  gtk_widget_set_name (txtOutput, "txtOutput");
  gtk_widget_show (txtOutput);
  gtk_container_add (GTK_CONTAINER (scrtxtOutput), txtOutput);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (txtOutput), FALSE);

  alignment14 = gtk_alignment_new (0.5, 0.5, 0, 1);
  gtk_widget_set_name (alignment14, "alignment14");
  gtk_widget_show (alignment14);
  gtk_box_pack_start (GTK_BOX (vbox10), alignment14, FALSE, FALSE, 0);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment14), 5, 5, 5, 5);

  btnCancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_set_name (btnCancel, "btnCancel");
  gtk_widget_show (btnCancel);
  gtk_container_add (GTK_CONTAINER (alignment14), btnCancel);

#ifndef _WIN32
  g_signal_connect ((gpointer) btnCancel, "clicked",
                    G_CALLBACK (on_btnCancel_clicked),
                    GTK_WIDGET(winNetworking));
  g_signal_connect ((gpointer) winNetworking, "destroy",
                    G_CALLBACK (on_winNetworking_destroy),
                    NULL);
#endif

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (winNetworking, winNetworking, "winNetworking");
  GLADE_HOOKUP_OBJECT (winNetworking, vbox10, "vbox10");
  GLADE_HOOKUP_OBJECT (winNetworking, alignment13, "alignment13");
  GLADE_HOOKUP_OBJECT (winNetworking, scrtxtOutput, "scrtxtOutput");
  GLADE_HOOKUP_OBJECT (winNetworking, txtOutput, "txtOutput");
  GLADE_HOOKUP_OBJECT (winNetworking, alignment14, "alignment14");
  GLADE_HOOKUP_OBJECT (winNetworking, btnCancel, "btnCancel");

  return winNetworking;
}
