/*
 *  Dr Genius interactive geometry software
 * (C) Copyright Hilaire Fernandes  2002
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

#ifndef DRGEO_ADPATDIALOG_H
#define DRGEO_ADPATDIALOG_H

#include <gtk/gtk.h>
#include <gmodule.h>
#include "geo_view.h"

void adaptDrgeoDialog (geoView * child);


extern "C" {
/* Callback for the UIAdpatDialog */
G_MODULE_EXPORT void on_pointMenuHide_clicked (GtkWidget * w, 
					       gpointer data);
G_MODULE_EXPORT void on_curveMenuHide_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_transformationMenuHide_clicked (GtkWidget * w, 
							gpointer data);
G_MODULE_EXPORT void on_numericMenuHide_clicked (GtkWidget * w, 
						 gpointer data);
G_MODULE_EXPORT void on_macroMenuHide_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_otherMenuHide_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_toolHide_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT gint on_uiAdaptDialog_delete_event (GtkWidget * w, 
						    GdkEventAny * e,
						    gpointer data);
G_MODULE_EXPORT void on_uiAdaptDialog_response (GtkWidget * widget, gint b, 
						gpointer data); 

}

gboolean getSensitiveState (GtkWidget * w);
gboolean getSensitiveStateByName (gchar * widgetName);
void setSensitiveState (gchar * button, gboolean state);

void on_relockDialog_response (GtkWidget *dialog, gint r, GtkWidget *entry);
void on_lockDialog_response (GtkWidget *dialog, gint r, GtkWidget *entry);
void on_unlockDialog_response (GtkWidget *dialog, gint r, GtkWidget *entry);

#endif /* DRGEO_ADPATDIALOG_H */
