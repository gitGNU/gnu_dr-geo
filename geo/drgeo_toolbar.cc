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

#include "drgeo_toolbar.h"
#include "drgeo_gtkdrawable.h"

drgeoToolbar::drgeoToolbar (geoView *view):
  drgeoControl (view)
{
  GladeXML *xml;
  GtkWidget *w;

  // Build the toolbars

  // take a reference of each bar item so we can :
  // . adjust their sensitivity
  // . attach a referecence of this class instance


  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","menuBar", NULL);
  setTopControlerWidget (xml);

  fetchWidgetParent (xml,&p_toolbar, "menuBar",
		     (gpointer) this);
  fetchWidgetParent (xml,&p_zoomEntry, "zoomEntry",
		     (gpointer) this);
  gtk_entry_set_text (GTK_ENTRY (p_zoomEntry), "100%");
  glade_xml_signal_autoconnect (xml);
  g_object_unref (G_OBJECT (xml));
  

  /* short cut bar */
  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","shortcutBar", NULL);
  setShortcutControlerWidget (xml);
  fetchWidgetParent (xml,&p_shortcutBar, "shortcutBar",
		     (gpointer) this);
  glade_xml_signal_autoconnect (xml);
  g_object_unref (G_OBJECT (xml));


  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","pointBar", NULL);
  glade_xml_signal_autoconnect (xml);
  setPointControlerWidget (xml);
  fetchWidgetParent (xml,&p_pointBar, "pointBar", 
		     (gpointer) this);
  w = glade_xml_get_widget (xml, "toolbar");
  gtk_toolbar_set_tooltips (GTK_TOOLBAR (w), true);
  g_object_unref (G_OBJECT (xml));
  
  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","curveBar", NULL);
  glade_xml_signal_autoconnect (xml);
  setCurveControlerWidget (xml);
  fetchWidgetParent (xml,&p_curveBar, "curveBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml));

  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","transformationBar", NULL);
  glade_xml_signal_autoconnect (xml);
  setTransformationControlerWidget (xml);
  fetchWidgetParent (xml,&p_transformationBar, 
		     "transformationBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml));

  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","numericBar", NULL);
  glade_xml_signal_autoconnect (xml);
  setNumericControlerWidget (xml);
  fetchWidgetParent (xml,&p_numericBar, "numericBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml));

  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","macroBar", NULL);
  glade_xml_signal_autoconnect (xml);
  setMacroControlerWidget (xml);
  fetchWidgetParent (xml,&p_macroBar, "macroBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml));

  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeo2.glade","otherBar", NULL);
  glade_xml_signal_autoconnect (xml);
  setOtherControlerWidget (xml);
  fetchWidgetParent (xml,&p_otherBar, "otherBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml));

}

GtkWidget *
drgeoToolbar::controlerWidget ()
{
  return p_toolbar;
}

GtkWidget *
drgeoToolbar::shortcutWidget ()
{
  return p_shortcutBar;
}

void
drgeoToolbar::showToolbar (drgeoControlToolbar tool)
{
  switch (tool)
    {
    case POINT_TOOLBAR:
      gtk_widget_show (p_pointBar);
      break;
    case CURVE_TOOLBAR:
      gtk_widget_show (p_curveBar);
      break;
    case TRANSFORMATION_TOOLBAR:
      gtk_widget_show (p_transformationBar);
      break;
    case NUMERIC_TOOLBAR:
      gtk_widget_show (p_numericBar);
      break;
    case MACRO_TOOLBAR:
      gtk_widget_show (p_macroBar);
      break;
    case OTHER_TOOLBAR:
      gtk_widget_show (p_otherBar);
      break;
    }
}

void
drgeoToolbar::hideToolbar (drgeoControlToolbar tool)
{
  switch (tool)
    {
    case POINT_TOOLBAR:
      gtk_widget_hide (p_pointBar);
      break;
    case CURVE_TOOLBAR:
      gtk_widget_hide (p_curveBar);
      break;
    case TRANSFORMATION_TOOLBAR:
      gtk_widget_hide (p_transformationBar);
      break;
    case NUMERIC_TOOLBAR:
      gtk_widget_hide (p_numericBar);
      break;
    case MACRO_TOOLBAR:
      gtk_widget_hide (p_macroBar);
      break;
    case OTHER_TOOLBAR:
      gtk_widget_hide (p_otherBar);
      break;
    }
}

void
drgeoToolbar::setZoomFactor (drgeoZoomFactor zoom)
{

  g_signal_handlers_block_matched ((gpointer) p_zoomEntry,
				   G_SIGNAL_MATCH_FUNC,
				   0, 0, NULL,
				   (gpointer) on_zoom_valeur_changed,
				   NULL);

  switch (zoom)
    {
    case z100:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "100%");
      break;
    case z200:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "200%");
      break;
    case z175:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "175%");
      break;
    case z150:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "150%");
      break;
    case z125:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "125%");
      break;
    case z75:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "75%");
      break;
    case z50:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "50%");
      break;
    case z25:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "25%");
      break;
    }
  g_signal_handlers_unblock_matched ((gpointer) p_zoomEntry,
				     G_SIGNAL_MATCH_FUNC,
				     0, 0, NULL,
				     (gpointer) on_zoom_valeur_changed,
				     NULL);
}

void
drgeoToolbar::updateUserInterface ()
{
  drgeoFigure *figure;

  drgeoControl::updateUserInterface ();
  /* Now specific update to the toolbar class */
  figure = p_view->figure ();
  hideTool (pointScId, !figure->getToolState (pointId));
  hideTool (segmentScId, !figure->getToolState (segmentId));
  hideTool (lineScId, !figure->getToolState (lineId));
  hideTool (distanceScId, !figure->getToolState (distanceId));
  hideTool (lookItemScId, !figure->getToolState (lookItemId));
  hideTool (propertyItemScId, !figure->getToolState (propertyItemId));
  hideTool (moveItemScId, !figure->getToolState (moveItemId));
  hideTool (deleteItemScId, !figure->getToolState (deleteItemId));
}

/*****************************************/
/* Handlers of the toolbar controlers    */
/*****************************************/
gint
on_subToolbar_clicked (GtkWidget * w, GdkEventButton * evt, gpointer data)
{
  gtk_widget_hide (w);
  return TRUE;
}

void
on_drgeoPointMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (POINT_TOOLBAR);
}

void
on_drgeoCurveMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (CURVE_TOOLBAR);
}

void
on_drgeoTransformationMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (TRANSFORMATION_TOOLBAR);
}

void
on_drgeoNumericMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (NUMERIC_TOOLBAR);
}

void
on_drgeoMacroMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (MACRO_TOOLBAR);
}

void
on_drgeoOtherMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (OTHER_TOOLBAR);
}

void 
on_zoom_valeur_changed (GtkEditable * w, gpointer data)
{
  drgeoFigure *figure;
  drgeoGtkDrawable *area;
  drgeoControl *control;
  gfloat zoom;

  control = (drgeoControl *) (gtk_object_get_data (GTK_OBJECT (w), "parent"));
  area = control->drawable ();

  figure = area->getFigure();
  
  zoom = strtod (gtk_entry_get_text (GTK_ENTRY (w)), NULL);
  
  area->setScale (zoomToScale (zoom));
  area->refresh ();

  if (zoom == 200.)
    control->view ()->setZoomFactorMenu (z200);
  else if (zoom == 175.)
    control->view ()->setZoomFactorMenu (z175);
  else if (zoom == 150.)
    control->view ()->setZoomFactorMenu (z150);
  else if (zoom == 125.)
    control->view ()->setZoomFactorMenu (z125);
  else if (zoom == 100.)
    control->view ()->setZoomFactorMenu (z100);
  else if (zoom == 75.)
    control->view ()->setZoomFactorMenu (z75);
  else if (zoom == 50.)
    control->view ()->setZoomFactorMenu (z50);
  else if (zoom == 25.)
    control->view ()->setZoomFactorMenu (z25);
}



