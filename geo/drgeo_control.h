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

#ifndef DRGEO_CONTROL_H
#define DRGEO_CONTROL_H

#include <gmodule.h>


#include "geo_view.h"
#include "drgeo_figure.h"

enum drgeoControlToolbar 
{
  POINT_TOOLBAR,
  CURVE_TOOLBAR,
  TRANSFORMATION_TOOLBAR,
  NUMERIC_TOOLBAR,
  MACRO_TOOLBAR,
  OTHER_TOOLBAR
};

class drgeoGtkDrawable;


class drgeoControl
{
 public:
  drgeoControl (geoView *view);
  ~drgeoControl ();
  void reconcileControler ();
  drgeoGtkDrawable *drawable ();
  void hideTool (drgeoToolId i, gboolean state);
  virtual GtkWidget *controlerWidget () = 0;
  virtual void hideToolbar (drgeoControlToolbar tool);
  virtual void setZoomFactor (drgeoZoomFactor zoom) = 0;
  virtual void updateUserInterface ();
  geoView * view ();
 protected:
  void setTopControlerWidget (GtkBuilder *xml);
  void setPointControlerWidget (GtkBuilder *xml);
  void setCurveControlerWidget (GtkBuilder *xml);
  void setTransformationControlerWidget (GtkBuilder *xml);
  void setNumericControlerWidget (GtkBuilder *xml);
  void setMacroControlerWidget (GtkBuilder *xml);
  void setOtherControlerWidget (GtkBuilder *xml);
  void setShortcutControlerWidget (GtkBuilder *xml);
  // the basic tools,common to menu and toolbar
  GtkWidget * p_menuItem[DRGEO_TOOLS_NUMBER+DRGEO_TOOLS_SHORTCUT];
  geoView *p_view;
};




// HELPERS

void fetchWidgetParent (GtkBuilder *xml, GtkWidget * *w, gchar *name, 
			gpointer parent);


extern "C" {
/*********************************/
/* Handlers of the controlers    */
/*********************************/
G_MODULE_EXPORT void drgeo_free_point_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_middle_point_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_intersection_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_mark_point_cb (GtkWidget * widget, gpointer);

G_MODULE_EXPORT void drgeo_line_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_half_line_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_segment_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_vector_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_circle_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_arc_circle_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_locus_point_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_polygon_cb (GtkWidget * widget, gpointer);

G_MODULE_EXPORT void drgeo_parallel_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_orthogonal_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_reflexion_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_symmetry_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_translation_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_rotation_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_scale_cb (GtkWidget * widget, gpointer);

G_MODULE_EXPORT void drgeo_numeric_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_angle_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_coordinates_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_script_cb (GtkWidget * widget, gpointer);

G_MODULE_EXPORT void drgeo_macro_build_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_macro_play_cb (GtkWidget * widget, gpointer);

G_MODULE_EXPORT void drgeo_delete_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_look_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_property_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_select_cb (GtkWidget * widget, gpointer);

G_MODULE_EXPORT void drgeo_zoom_25_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_50_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_75_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_100_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_125_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_150_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_175_cb (GtkWidget * widget, gpointer);
G_MODULE_EXPORT void drgeo_zoom_200_cb (GtkWidget * widget, gpointer);

}

#endif /* DRGEO_CONTROL_H */
