/* Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2003
 * hilaire@ofset.org 
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

#ifndef DRGEO_TOOLBAR_H
#define DRGEO_TOOLBAR_H

#include <gtk/gtk.h>
#include <gmodule.h>
#include "drgeo_control.h"

class drgeoToolbar:public drgeoControl
{
 public:
  drgeoToolbar (geoView *view);
  GtkWidget *controlerWidget ();
  GtkWidget *shortcutWidget ();
  void showToolbar (drgeoControlToolbar tool);
  void hideToolbar (drgeoControlToolbar tool);
  void setZoomFactor (drgeoZoomFactor zoom);
  void updateUserInterface ();
 private:
  GtkWidget *p_toolbar, *p_pointBar, *p_curveBar, *p_transformationBar,
    *p_numericBar, *p_macroBar, *p_otherBar, *p_zoomEntry,
    *p_shortcutBar;
};


extern "C" {
// Sepcific callback for the toolbar
G_MODULE_EXPORT gint on_subToolbar_clicked (GtkWidget * w, 
					    GdkEventButton * evt, 
					    gpointer data);

G_MODULE_EXPORT void on_drgeoPointMenu_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_drgeoCurveMenu_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_drgeoTransformationMenu_clicked (GtkWidget * w, 
							 gpointer data);
G_MODULE_EXPORT void on_drgeoNumericMenu_clicked (GtkWidget * w, 
						  gpointer data);
G_MODULE_EXPORT void on_drgeoMacroMenu_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_drgeoOtherMenu_clicked (GtkWidget * w, gpointer data);
G_MODULE_EXPORT void on_zoom_valeur_changed (GtkEditable * w, gpointer data);
}

#endif /*DRGEO_TOOLBAR_H */
