/*
 *  Dr Genius interactive geometry software
 * (C) Copyright Hilaire Fernandes  2001
 * 
 * 
 * 
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public Licences as by published
 * by the Free Software Foundation; either version 2; or (at your option)
 * any later version
 *
 * This program is distributed in the hope that it will entertaining,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILTY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Publis License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef DRGENIUS_CONFIG_H
#define DRGENIUS_CONFIG_H

#include <libguile.h>

#include <gtk/gtk.h>
#include <gmodule.h>

/*
  Helper functions
*/
static void drgeoConfigSetGen(gchar *attr, gchar *value, gchar *conf);
static void drgeoConfigSet(gchar *attr, const gchar *value);
static void drgeoConfigSetDialog(gchar *attr, gchar *value);
gchar * drgeoConfigGet (gchar *attr);
static GtkWidget * drgeoConfigWidget (gchar * item, 
				      gchar *attr, GtkBuilder *xml);

void loadUserPreferences ();
void saveUserPreferences ();
void updateDialogFromUserPreferences (GtkObject * d);
void updateUserPreferencesFromDialog (GtkObject * d);
void initPreferencesBox ();


extern "C" {
  G_MODULE_EXPORT void on_propertyBox_response (GtkWidget *d, gint r, 
						gpointer data);
  G_MODULE_EXPORT void on_numeric_changed (GtkWidget * widget, gpointer data);
  G_MODULE_EXPORT void on_undo_changed (GtkWidget * widget, gpointer data);
  G_MODULE_EXPORT void on_figureName_changed (GtkWidget * widget, 
					      gpointer data);
  G_MODULE_EXPORT void on_htmlViewer_changed (GtkWidget * widget, 
					      gpointer data);
  G_MODULE_EXPORT void on_figureFile_changed (GtkWidget * widget, 
					      gpointer data);
  G_MODULE_EXPORT void on_sessionFile_changed (GtkWidget * widget, 
					       gpointer data);
  G_MODULE_EXPORT void on_flydrawFile_changed (GtkWidget * widget, 
					       gpointer data);
  G_MODULE_EXPORT void on_latexFile_changed (GtkWidget * widget, 
					     gpointer data);
  G_MODULE_EXPORT void on_psFile_changed (GtkWidget * widget, gpointer data);
}

#endif /* DRGENIUS_CONFIG_H */
