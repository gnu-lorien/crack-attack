#include <gtk/gtk.h>


void
on_rbtnSingle_activate                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_rbtnClient_activate                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_rbtnServer_activate                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_rbtnSingle_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_rbtnServer_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_rbtnClient_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_Xtreme_toggled                      (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btnStart_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnHelp_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_cbtnLowGraphics_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
gboolean 
on_winCrackAttackSplash_delete_event   (GtkWindow *window,
                                        gpointer  user_data);
void
on_640by480_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_800by600_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_1024by768_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_1280by1024_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_1600by1200_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_ai_none_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_ai_easy_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_ai_medium_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_ai_hard_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
